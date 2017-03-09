

LIBNAME = librsaconverter.c

all: check_priv_key privkey_from_p_q brute_force

check_priv_key: check_priv_key.c $(LIBNAME)
	gcc -o $@ -pthread -ldl -lcrypto $+

privkey_from_p_q: privkey_from_p_q.c $(LIBNAME)
	gcc -o $@ -pthread -ldl -lcrypto $+

brute_force: brute_force.c
	gcc -o $@ -pthread -ldl -lcrypto $+

