#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DUDECT_IMPLEMENTATION
#include "dudect.h"

int consttime_memcmp(const void *b1, const void *b2, size_t len)
{
	const uint8_t *c1, *c2;
	uint16_t d, r, m;

#if USE_VOLATILE_TEMPORARY
	volatile uint16_t v;
#else
	uint16_t v;
#endif

	c1 = b1;
	c2 = b2;

	r = 0;
	while (len) {
		/*
		 * Take the low 8 bits of r (in the range 0x00 to 0xff,
		 * or 0 to 255);
		 * As explained elsewhere, the low 8 bits of r will be zero
		 * if and only if all bytes compared so far were identical;
		 * Zero-extend to a 16-bit type (in the range 0x0000 to
		 * 0x00ff);
		 * Add 255, yielding a result in the range 255 to 510;
		 * Save that in a volatile variable to prevent
		 * the compiler from trying any shortcuts (the
		 * use of a volatile variable depends on "#ifdef
		 * USE_VOLATILE_TEMPORARY", and most compilers won't
		 * need it);
		 * Divide by 256 yielding a result of 1 if the original
		 * value of r was non-zero, or 0 if r was zero;
		 * Subtract 1, yielding 0 if r was non-zero, or -1 if r
		 * was zero;
		 * Convert to uint16_t, yielding 0x0000 if r was
		 * non-zero, or 0xffff if r was zero;
		 * Save in m.
		 */
		v = ((uint16_t)(uint8_t)r)+255;
		m = v/256-1;

		/*
		 * Get the values from *c1 and *c2 as uint8_t (each will
		 * be in the range 0 to 255, or 0x00 to 0xff);
		 * Convert them to signed int values (still in the
		 * range 0 to 255);
		 * Subtract them using signed arithmetic, yielding a
		 * result in the range -255 to +255;
		 * Convert to uint16_t, yielding a result in the range
		 * 0xff01 to 0xffff (for what was previously -255 to
		 * -1), or 0, or in the range 0x0001 to 0x00ff (for what
		 * was previously +1 to +255).
		 */
		d = (uint16_t)((int)*c1 - (int)*c2);

		/*
		 * If the low 8 bits of r were previously 0, then m
		 * is now 0xffff, so (d & m) is the same as d, so we
		 * effectively copy d to r;
		 * Otherwise, if r was previously non-zero, then m is
		 * now 0, so (d & m) is zero, so leave r unchanged.
		 * Note that the low 8 bits of d will be zero if and
		 * only if d == 0, which happens when *c1 == *c2.
		 * The low 8 bits of r are thus zero if and only if the
		 * entirety of r is zero, which happens if and only if
		 * all bytes compared so far were equal.  As soon as a
		 * non-zero value is stored in r, it remains unchanged
		 * for the remainder of the loop.
		 */
		r |= (d & m);

		/*
		 * Increment pointers, decrement length, and loop.
		 */
		++c1;
		++c2;
		--len;
	}

	/*
	 * At this point, r is an unsigned value, which will be 0 if the
	 * final result should be zero, or in the range 0x0001 to 0x00ff
	 * (1 to 255) if the final result should be positive, or in the
	 * range 0xff01 to 0xffff (65281 to 65535) if the final result
	 * should be negative.
	 *
	 * We want to convert the unsigned values in the range 0xff01
	 * to 0xffff to signed values in the range -255 to -1, while
	 * converting the other unsigned values to equivalent signed
	 * values (0, or +1 to +255).
	 *
	 * On a machine with two's complement arithmetic, simply copying
	 * the underlying bits (with sign extension if int is wider than
	 * 16 bits) would do the job, so something like this might work:
	 *
	 *     return (int16_t)r;
	 *
	 * However, that invokes implementation-defined behaviour,
	 * because values larger than 32767 can't fit in a signed 16-bit
	 * integer without overflow.
	 *
	 * To avoid any implementation-defined behaviour, we go through
	 * these contortions:
	 *
	 * a. Calculate ((uint32_t)r + 0x8000).  The cast to uint32_t
	 *    it to prevent problems on platforms where int is narrower
	 *    than 32 bits.  If int is a larger than 32-bits, then the
	 *    usual arithmetic conversions cause this addition to be
	 *    done in unsigned int arithmetic.  If int is 32 bits
	 *    or narrower, then this addition is done in uint32_t
	 *    arithmetic.  In either case, no overflow or wraparound
	 *    occurs, and the result from this step has a value that
	 *    will be one of 0x00008000 (32768), or in the range
	 *    0x00008001 to 0x000080ff (32769 to 33023), or in the range
	 *    0x00017f01 to 0x00017fff (98049 to 98303).
	 *
	 * b. Cast the result from (a) to uint16_t.  This effectively
	 *    discards the high bits of the result, in a way that is
	 *    well defined by the C language.  The result from this step
	 *    will be of type uint16_t, and its value will be one of
	 *    0x8000 (32768), or in the range 0x8001 to 0x80ff (32769 to
	 *    33023), or in the range 0x7f01 to 0x7fff (32513 to
	 *    32767).
	 *
	 * c. Cast the result from (b) to int32_t.  We use int32_t
	 *    instead of int because we need a type that's strictly
	 *    larger than 16 bits, and the C standard allows
	 *    implementations where int is only 16 bits.  The result
	 *    from this step will be of type int32_t, and its value wll
	 *    be one of 0x00008000 (32768), or in the range 0x00008001
	 *    to 0x000080ff (32769 to 33023), or in the range 0x00007f01
	 *    to 0x00007fff (32513 to 32767).
	 *
	 * d. Take the result from (c) and subtract 0x8000 (32768) using
	 *    signed int32_t arithmetic.  The result from this step will
	 *    be of type int32_t and the value will be one of
	 *    0x00000000 (0), or in the range 0x00000001 to 0x000000ff
	 *    (+1 to +255), or in the range 0xffffff01 to 0xffffffff
	 *    (-255 to -1).
	 *
	 * e. Cast the result from (d) to int.  This does nothing
	 *    interesting, except to make explicit what would have been
	 *    implicit in the return statement.  The final result is an
	 *    int in the range -255 to +255.
	 *
	 * Unfortunately, compilers don't seem to be good at figuring
	 * out that most of this can be optimised away by careful choice
	 * of register width and sign extension.
	 *
	 */
	return (/*e*/ int)(/*d*/
	    (/*c*/ int32_t)(/*b*/ uint16_t)(/*a*/ (uint32_t)r + 0x8000)
	    - 0x8000);
}

/* target function to check for constant time */
int check_tag(uint8_t *x, uint8_t *y, size_t len) {
  return consttime_memcmp(x, y, len);
}

#define SECRET_LEN_BYTES (16)

uint8_t secret[SECRET_LEN_BYTES] = {0, 1, 2, 3, 4, 5, 6, 42};

/* this will be called over and over */
uint8_t do_one_computation(uint8_t *data) {
  /* simulate totally bogus MAC check in non-constant time */
  return check_tag(data, secret, SECRET_LEN_BYTES);
}

/* called once per number_measurements */
void prepare_inputs(dudect_config_t *c, uint8_t *input_data, uint8_t *classes) {
  randombytes(input_data, c->number_measurements * c->chunk_size);
  for (size_t i = 0; i < c->number_measurements; i++) {
    /* it is important to randomize the class sequence */
    classes[i] = randombit();
    if (classes[i] == 0) {
      memset(input_data + (size_t)i * c->chunk_size, 0x00, c->chunk_size);
    } else {
      // leave random
    }
  }
}

int run_test(void) {
  dudect_config_t config = {
      .chunk_size = SECRET_LEN_BYTES,
      #ifdef MEASUREMENTS_PER_CHUNK
      .number_measurements = MEASUREMENTS_PER_CHUNK,
      #else
      .number_measurements = 500,
      #endif
  };
  dudect_ctx_t ctx;

  dudect_init(&ctx, &config);

  /*
  Call dudect_main() until
   - returns something different than DUDECT_NO_LEAKAGE_EVIDENCE_YET, or
   - you spent too much time testing and give up
  Recommended that you wrap this program with timeout(2) if you don't
  have infinite time.
  For example this will run for 20 mins:
    $ timeout 1200 ./your-executable
  */
  dudect_state_t state = DUDECT_NO_LEAKAGE_EVIDENCE_YET;
  while (state == DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
    state = dudect_main(&ctx);
  }
  dudect_free(&ctx);
  return (int)state;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  run_test();
}
