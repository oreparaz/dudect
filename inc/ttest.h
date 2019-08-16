typedef struct {
  double mean[2];
  double m2[2]; // m2, once divided by n-1 provides the sample variance
  double n[2];
} t_ctx;

void t_push(t_ctx *ctx, double x, uint8_t class);
double t_compute(t_ctx *ctx);
void t_init(t_ctx *ctx);
