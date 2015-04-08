

LIBNAME = librsaconverter.c

check_priv_key: check_priv_key.c $(LIBNAME)
	gcc -o $@ -pthread -ldl -lcrypto $+

privkey_from_p_q: privkey_from_p_q.c $(LIBNAME)
	gcc -o $@ -pthread -ldl -lcrypto $+
