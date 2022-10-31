This project contains an implementation of the Diffie-Hellman algorithm and the
RSA algorithm written in C.

gcc --version: gcc (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0


-DIffie-Hellman Algorithm

This tool creates 2 public keys based on input by implementing the algorithm,
and stores them, included the common secret key inside a txt file.

Input: prime numbers g(base) & p(mod), private keys a & b, output text directory o
Output: output txt file that contains <public key A>, <public key B>, <shared secret>

ex: ./dh_assign_1 -o output.txt -p 23 -g 9 -a 15 -b 2


-RSA Algorithm

This tool can generate 2 set of keys using the RSA key generation method,
which can be used by the same tool to either encrypt a plaintext or to decrypt a ciphertext.

Input: mode(g,e,d), i for input txt directory, o for output txt directory,
        k for key file directory
Output: 
    - mode(g): randomly generated private.key public.key files
      ex: ./rsa_assign_1 -g

    - mode(e): encrypted file based on input plaintext file and specified key
      ex: ./rsa_assign_1 -i plaintext.txt -o ciphertext.txt -k public.key -e

    - mode(d): decrypted file based on input ciphertext file and specified key
      ex: ./rsa_assign_1 -i ciphertext.txt -o plaintext_out.txt -k private.key -d


Dependencies: 
    - GMP library: sudo apt install libgmp3-dev