
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
    BIGNUM* p = BN_new();
    BIGNUM* q = BN_new();
    BIGNUM* dp = BN_new();
    BIGNUM* dq = BN_new();
    BIGNUM* u = BN_new();
    SfmToCrt(n,e,d,p,q,dp,dq,u);
    
    puts("asn1=SEQUENCE:rsa_key");
    puts("[rsa_key]");
    puts("# Input values");
    printf("modulus=INTEGER:%s\n", BN_bn2dec(n));
    printf("pubExp=INTEGER:%s\n", BN_bn2dec(e));
    printf("privExp=INTEGER:%s\n", BN_bn2dec(d));
    puts("# Results");
    printf("p=INTEGER:%s\n", BN_bn2dec(p));
    printf("q=INTEGER:%s\n", BN_bn2dec(q));
    printf("e1=INTEGER:%s\n", BN_bn2dec(dp));
    printf("e2=INTEGER:%s\n", BN_bn2dec(dq));
    printf("coeff=INTEGER:%s\n", BN_bn2dec(u));
    
  } else {
    fprintf(stderr, "Incorrect");
  }
  
  fprintf(stderr, "\n");
  
  return ok;
  
}
