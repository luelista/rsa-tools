
void printasn1(BIGNUM* n, BIGNUM* e, BIGNUM* d) {

    BIGNUM* p = BN_new();
    BIGNUM* q = BN_new();
    BIGNUM* dp = BN_new();
    BIGNUM* dq = BN_new();
    BIGNUM* u = BN_new();
    SfmToCrt(n,e,d,p,q,dp,dq,u);
    
    puts("asn1=SEQUENCE:rsa_key");
    puts("[rsa_key]");
    puts("version=INTEGER:0");
    printf("modulus=INTEGER:%s\n", BN_bn2dec(n));
    printf("pubExp=INTEGER:%s\n", BN_bn2dec(e));
    printf("privExp=INTEGER:%s\n", BN_bn2dec(d));
    printf("p=INTEGER:%s\n", BN_bn2dec(p));
    printf("q=INTEGER:%s\n", BN_bn2dec(q));
    printf("e1=INTEGER:%s\n", BN_bn2dec(dp));
    printf("e2=INTEGER:%s\n", BN_bn2dec(dq));
    printf("coeff=INTEGER:%s\n", BN_bn2dec(u));

}

