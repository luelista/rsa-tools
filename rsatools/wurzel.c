
// geklaut von:
// https://boringssl.googlesource.com/boringssl.git/+/master/crypto/bn/sqrt.c

#define OPENSSL_PUT_ERROR(a,b,c) fprintf(stderr, "%s %s: %s\n", #a, #b, #c)

int BN_sqrt(BIGNUM *out_sqrt, const BIGNUM *in, BN_CTX *ctx) {
  BIGNUM *estimate, *tmp, *delta, *last_delta, *tmp2;
  int ok = 0, last_delta_valid = 0;
  if (in->neg) {
    OPENSSL_PUT_ERROR(BN, BN_sqrt, BN_R_NEGATIVE_NUMBER);
    return 0;
  }
  if (BN_is_zero(in)) {
    BN_zero(out_sqrt);
    return 1;
  }
  BN_CTX_start(ctx);
  if (out_sqrt == in) {
    estimate = BN_CTX_get(ctx);
  } else {
    estimate = out_sqrt;
  }
  tmp = BN_CTX_get(ctx);
  last_delta = BN_CTX_get(ctx);
  delta = BN_CTX_get(ctx);
  if (estimate == NULL || tmp == NULL || last_delta == NULL || delta == NULL) {
    OPENSSL_PUT_ERROR(BN, BN_sqrt, ERR_R_MALLOC_FAILURE);
    goto err;
  }
  /* We estimate that the square root of an n-bit number is 2^{n/2}. */
  BN_lshift(estimate, BN_value_one(), BN_num_bits(in)/2);
  /* This is Newton's method for finding a root of the equation |estimate|^2 -
   * |in| = 0. */
  for (;;) {
    /* |estimate| = 1/2 * (|estimate| + |in|/|estimate|) */
    if (!BN_div(tmp, NULL, in, estimate, ctx) ||
        !BN_add(tmp, tmp, estimate) ||
        !BN_rshift1(estimate, tmp) ||
        /* |tmp| = |estimate|^2 */
        !BN_sqr(tmp, estimate, ctx) ||
        /* |delta| = |in| - |tmp| */
        !BN_sub(delta, in, tmp)) {
      OPENSSL_PUT_ERROR(BN, BN_sqrt, ERR_R_BN_LIB);
      goto err;
    }
    delta->neg = 0;
    /* The difference between |in| and |estimate| squared is required to always
     * decrease. This ensures that the loop always terminates, but I don't have
     * a proof that it always finds the square root for a given square. */
    if (last_delta_valid && BN_cmp(delta, last_delta) >= 0) {
      break;
    }
    last_delta_valid = 1;
    tmp2 = last_delta;
    last_delta = delta;
    delta = tmp2;
  }
  if (BN_cmp(tmp, in) != 0) {
    OPENSSL_PUT_ERROR(BN, BN_sqrt, BN_R_NOT_A_SQUARE);
    goto err;
  }
  ok = 1;
err:
  if (ok && out_sqrt == in) {
    BN_copy(out_sqrt, estimate);
  }
  BN_CTX_end(ctx);
  return ok;
}