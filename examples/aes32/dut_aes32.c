#include <stdlib.h>
#include <stdint.h>
#include <string.h> 
#include "rijndael-alg-fst.h"

#define DUDECT_IMPLEMENTATION
#include "dudect.h"

static uint32_t rk[44] = {0};

uint8_t do_one_computation(uint8_t *data) {
  uint8_t in[16] = {0};
  uint8_t out[16] = {0};
  uint8_t ret = 0;

  memcpy(in, data, 16);

  rijndaelEncrypt(rk, 10, in, out);

  ret ^= out[0];
  /* return some computation output to try to tame a clever optimizing compiler */
  return ret;
}

void prepare_inputs(dudect_config_t *c, uint8_t *input_data, uint8_t *classes) {
  randombytes(input_data, c->number_measurements * c->chunk_size);
  for (size_t i = 0; i < c->number_measurements; i++) {
    classes[i] = randombit();
    if (classes[i] == 0) {
      memset(input_data + (size_t)i * c->chunk_size, 0x00, c->chunk_size);
    } else {
      // leave random
    }
  }
}

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  dudect_config_t config = {
     .chunk_size = 16,
     .number_measurements = 1e6,
  };
  dudect_ctx_t ctx;

  uint8_t cipherKey[16] = {0};
  rijndaelKeySetupEnc(rk, cipherKey, 128);

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

