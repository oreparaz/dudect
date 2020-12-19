#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DUDECT_IMPLEMENTATION
#include "dudect.h"

/* target function to check for constant time */
int check_tag(uint8_t *x, uint8_t *y, size_t len) {
  return memcmp(x, y, len);
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