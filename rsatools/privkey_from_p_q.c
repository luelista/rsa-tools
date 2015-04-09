
// Copyright (c) 2015 Max Weller

// To use:
// ./check_priv_key N D > test.cfg
//    N  modulus (hex)
//    D  privateExponent (hex)
//
// openssl asn1parse -genconf test.cfg -out newkey.der
// openssl rsa -in newkey.der -inform der -text -check
// openssl rsa -in newkey.der -inform der -out newkey.pem

#include <string.h>
#include <openssl/bn.h>

#include "printasn1.c"

int main(int argc, char** argv) {
  BN_CTX *ctx = BN_CTX_new();
  BN_CTX_start(ctx);

  BIGNUM * p = BN_new();
  BIGNUM * q = BN_new();
  BIGNUM * e = BN_new();
  
  BN_hex2bn(&p, argv[1]);
  BN_hex2bn(&q, argv[2]);
  BN_dec2bn(&e, "65537");

  BIGNUM * n = BN_new();
  BIGNUM * d = BN_new();
  BIGNUM * phi = BN_new();
  BN_mul(n, p, q, ctx);
  BN_sub_word(p, 1);
  BN_sub_word(q, 1);
  BN_mul(phi, p, q, ctx);
  BN_mod_inverse(d, e, phi, ctx);

  //fprintf (stderr, "d=%s \n", BN_bn2hex(d));

  printasn1(n,e,d);
  return 0;
  
}


