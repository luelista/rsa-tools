
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
  BIGNUM *n;
  n = BN_new();
  BIGNUM* e = BN_new();
  BIGNUM* d = BN_new();
  
//  printf("n: %s\n", argv[1]);
  fprintf(stderr, "d[-2:]: %s", argv[2]+strlen(argv[2])-2);
  BN_hex2bn(&n, argv[1]);
  BN_hex2bn(&d, argv[2]);
  BN_dec2bn(&e, "65537");
  fprintf (stderr, "...");
  int ok=CheckRsaSfmKey(n, e, d);
  
  if (ok) {
    fprintf(stderr, "\033[32mOK\033[39m\n");
    printasn1(n,e,d);
    
  } else {
    fprintf(stderr, "Incorrect");
  }
  
  fprintf(stderr, "\n");
  
  return ok;
  
}
