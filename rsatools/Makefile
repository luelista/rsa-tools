

LIBNAME = librsaconverter.c

check_priv_key: check_priv_key.c $(LIBNAME)
	gcc -o $@ -pthread -ldl -lcrypto $+
