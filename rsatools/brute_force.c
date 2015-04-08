
// Copyright (c) 2015 Max Weller

// To use:
// ./brute_force N group_count group_index [logname]
//    N  modulus (hex)
//    group_count   amount of parallel processes
//    group_index    index of this process [1 .. group_count]
//    logname          name of log file, default brutefor.log
//

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <openssl/bn.h>

#define LOG(fmt, args...) do{format_date_time();printf("%s " fmt, date_time, ##args); if(log != NULL)fprintf(log,"%s " fmt, date_time, ##args); }while(0)

#include "wurzel.c"

BIGNUM * parse_bn(char* num) {
  BIGNUM * out = BN_new();
  if (num[0] == '0' && num[1] == 'x') {
    num += 2;
    BN_hex2bn(&out, num);
  } else {
    BN_dec2bn(&out, num);
  }
  return out;
}

char date_time[26];
void format_date_time() {
    time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(date_time, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

int main(int argc, char** argv) {
  BN_CTX *ctx = BN_CTX_new();
  BN_CTX_start(ctx);

  BIGNUM * n = parse_bn(argv[1]);
  BIGNUM *  group_count = parse_bn(argv[2]);
  BIGNUM *  group_index = parse_bn(argv[3]);
  char* logname = "brutefor.log";
  if (argc>=5) logname = argv[4];
  FILE * log = fopen(logname, "a");
  
  LOG("Starting brute force process %s/%s ...\n", 
        BN_bn2dec(group_index), BN_bn2dec(group_count));
  LOG("N = %s\n", BN_bn2dec(n));
  
  BIGNUM * sqr = BN_new();
  BN_sqrt(sqr, n, ctx);
  LOG("SQR = %s\n", BN_bn2dec(sqr));
  
  BIGNUM * lot_size = BN_new();
  BIGNUM * rest = BN_new();
  BN_div(lot_size, rest, sqr, group_count, ctx);
  //BN_add(lot_size, lot_size, rest);
  BIGNUM * start_p = BN_new();
  BIGNUM * end_p = BN_new();
  BN_mul(start_p, lot_size, group_index, ctx);
  BN_sub(start_p, sqr, start_p);
  BN_add(end_p, start_p, lot_size);
  
  if (!BN_is_odd(start_p)) BN_add_word(start_p, 1);
  BN_div_word(lot_size, 2);
  
  BIGNUM * ctr = BN_new();
  BN_copy(ctr, lot_size);
  
  LOG("Handling lot from %s to %s (size %s)\n", 
  BN_bn2dec(start_p), BN_bn2dec(end_p), BN_bn2dec(lot_size));
  
  BN_div_word(lot_size, 10000);//percentage stuff...
  
  int progr = 900000;
  while(!BN_is_zero(ctr)) {
    
    BN_mod(rest, n, start_p, ctx);
    if (BN_is_zero(rest) && !BN_is_one(start_p)) {
      printf("\033[2K Running ... \033[32;1m Possible factorization found! \033[0m \n"); fflush(stdout);
      LOG("Possible factorization found!\n");
      LOG("P = %s\n", BN_bn2dec(start_p));
      
      BIGNUM * q = BN_new();
      BN_div(q, NULL, n, start_p, ctx);
      LOG("Q = %s\n", BN_bn2dec(q));
      
      return 0;
    }
    
    BN_add_word(start_p, 2);
    
    BN_sub_word(ctr, 1);
    
    if (progr-- == 0) {
      progr = 900000;
      BN_div(rest, NULL, ctr, lot_size, ctx);
      float f = ((float)atoi(BN_bn2dec(rest)))/100;
      printf("\033[2K Running ... \033[35;1m %02.2f %%    %s \033[0m \r", f, BN_bn2dec(ctr)); fflush(stdout);
    }
  }
  
  printf("\033[2K Running ... \033[32;1m no match in lot \033[0m \n"); fflush(stdout);

  return 1;
  
}


