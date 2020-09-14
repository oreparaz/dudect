#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "api.h"
#include "crypto_stream_aes128ctr.h"

#define DUDECT_IMPLEMENTATION
#include "dudect.h"


uint8_t do_one_computation(uint8_t *data) {
  uint8_t in[16] = {0};
  uint8_t out[16] = {0};
  uint8_t key[16] = {0x01,0};
  uint8_t ret = 0;

  memcpy(in, data, 16);
  crypto_stream(out, 16, in, key);
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
     .chunk_size = 16,
     .number_measurements = 1e6,
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
