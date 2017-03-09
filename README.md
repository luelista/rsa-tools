asn1decode.c
=============

Decodes a DER / ASN.1 encoded binary data structure (e.g. usual RSA public, private keys, certificates ...) and dumps it as text to stdout.

Will also run on truncated files, as opposed to OpenSSL's parser, and display all available data.

Contains a minimal DER parser. Only required stdio.h from C standard library.

To compile:

    cd asn1
    make

To use:

    ./asn1decode </path/to/der_encoded.key>


rsa-tools
===================

Tools to do various low-level calculations for RSA private keys, and for messing around with broken DER encoded private keys.

To compile, openssl with headers is required.

    cd rsatools
    make


check_priv_key.c
--------------------

To use:

    ./check_priv_key N D > test.cfg
       N  modulus (hex)
       D  privateExponent (hex)
    
    openssl asn1parse -genconf test.cfg -out newkey.der
    openssl rsa -in newkey.der -inform der -text -check
    openssl rsa -in newkey.der -inform der -out newkey.pem


privkey_from_p_q.c
---------------------

To use:

    ./privkey_from_p_q P Q > test.cfg
    #   P, Q  Private key factors
    
    # e = 65537 hard coded exponent
    
    openssl asn1parse -genconf test.cfg -out newkey.der
    openssl rsa -in newkey.der -inform der -text -check
    openssl rsa -in newkey.der -inform der -out newkey.pem


brute_force.c
-------------------

Brute force factorize a number (e.g. RSA public key) using the OpenSSL BigNum implementation.

Supports using multiple parallel processes by telling each process the count and its own number.


To use:

    ./brute_force N group_count group_index [logname]
       N  modulus (hex)
       group_count   amount of parallel processes
       group_index    index of this process [1 .. group_count]
       logname          name of log file, default brutefor.log


Example with four processes:

    export N=1234567890
    ./brute_force $N 4 1 bf1.log
    ./brute_force $N 4 2 bf2.log
    ./brute_force $N 4 3 bf3.log
    ./brute_force $N 4 4 bf4.log




License
===================

Copyright (C) 2015 Max Weller

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License

