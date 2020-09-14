#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h> // memcmp
#include "donna.h"

#define DUDECT_IMPLEMENTATION
#include "dudect.h"

uint8_t do_one_computation(uint8_t *data) {
  uint8_t out[32] = {0};
  const uint8_t secret[32] = {1,2,3};
  uint8_t ret = 0;
  const uint8_t basepoint[32] = {9};
  curve25519_donna(out, data, basepoint);
  ret ^= out[0];
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
     .chunk_size = 32,
     .number_measurements = 1e5,
  };
  dudect_ctx_t ctx;

  dudect_init(&ctx, &config);

  dudect_state_t state = DUDECT_NO_LEAKAGE_EVIDENCE_YET;
  while (state == DUDECT_NO_LEAKAGE_EVIDENCE_YET) {
    state = dudect_main(&ctx);
  }
  dudect_free(&ctx);
  return (int)state;
}