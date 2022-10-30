#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <gmp.h>

#define KEY_LENGTH 8*sizeof(size_t) // 64 bits per key

enum mode {key_generation, encryption, decryption};

void generate_keys(void);
void encrypt(char const*, char const*, char const*);
void decrypt(char const*, char const*, char const*);
long file_length(FILE*);

int main(int argc, char **argv)
{   
    // Argument parsing
    char const *input_dir, *output_dir, *key_file_dir;
    int flag=0;
    enum mode proc_mode;

    // TODO: Fix method invocation after while loop to check specific proccesses
    int opt;
    while((opt = getopt(argc, argv, "gdehi:o:k:")) != -1){
        switch(opt){
            case 'i':
                input_dir = optarg;
                if(access(input_dir, F_OK)!=0){   // doesnt work with windows os
                    printf("Err: Invalid input directory\n");
                    return -1;
                } 
                break;
            case 'o':
                output_dir = optarg;
                break;
            case 'k':
                key_file_dir = optarg;
                break;
            case 'g':
                if (argc > 2 || flag!=0){
                    printf("Err: Invalid Arguments, use -h for help\n");
                    return -1;
                }
                else{
                    flag=1;
                    proc_mode = key_generation;
                    break;
                }
                generate_keys();
                return 0;
                break;
            case 'd':
                if (argc!=8 || flag!=0){
                    printf("Err: Invalid Arguments, use -h for help\n");
                    return -1;
                }
                else{
                    flag=1;
                    proc_mode = decryption;
                    break;
                }
            case 'e':
                if (argc!=8 || flag!=0){
                    printf("Err: Invalid Arguments, use -h for help\n");
                    return -1;
                }
                else{
                    flag=1;
                    proc_mode = encryption;
                    break;
                }
            case 'h':
                if (argc > 2){
                    printf("Err: Invalid Arguments, use -h for help\n");
                    return -1;
                }
                printf("Description of rsa_assign tool\n");
                return 0;
            case ':':
                printf("Err: Option needs a value\n, use -h for help");
                return -1;
                break;
            case '?':
                printf("Err: Unkownk option: %c, use -h for help\n", optopt);
                return -1;
                break;
        }
    }

    if(flag==0){
        printf("Err: Invalid Arguments, use -h for help\n");
        return -1;
    }

    switch (proc_mode)
    {
    case encryption:
        if(input_dir==NULL || output_dir==NULL || key_file_dir==NULL){
            printf("Err: Invalid Arguments, use -h for help\n");
            return -1;
        }
        else{
            encrypt(input_dir, output_dir, key_file_dir);
            return 0;
        }
    case decryption:
        if(input_dir==NULL || output_dir==NULL || key_file_dir==NULL){
            printf("Err: Invalid Arguments, use -h for help\n");
            return -1;
        }
        else{
            decrypt(input_dir, output_dir, key_file_dir);
            return 0;
        }
    case key_generation:
        generate_keys();
        return 0;
    default:
        return -1;
    }
}

void encrypt(char const* plaintext_dir, char const* ciphertext_dir, char const* key_file_dir){
    FILE* plaintext_file, *ciphertext_file, *key_file;
    mpz_t power, mod, base, cipher_char;

    mpz_init2(power, KEY_LENGTH);
    mpz_init2(mod, KEY_LENGTH);
    mpz_init2(cipher_char, KEY_LENGTH);
    mpz_init2(base, sizeof(char));
    size_t key_buffer[2];

    if(access(key_file_dir, F_OK)!=0){   // doesnt work with windows os
        printf("Err: Invalid key file directory\n");
        return;
    }

    // read encryption key from key_file
    key_file = fopen(key_file_dir, "r");
    fread(key_buffer, sizeof(size_t), 2, key_file);
    mpz_import(mod, 1, 1, sizeof(size_t), 0, 0, &key_buffer[0]);
    mpz_import(power, 1, 1, sizeof(size_t), 0, 0, &key_buffer[1]);
    fclose(key_file);
    
    plaintext_file = fopen(plaintext_dir, "r");
    ciphertext_file = fopen(ciphertext_dir, "w");

    long file_l = file_length(plaintext_file);

    char plaintext_char;
    size_t cipher;
    for (size_t i = 0; i < file_l; i++){
        plaintext_char=fgetc(plaintext_file);
        mpz_set_ui(base, plaintext_char);
        // printf("%c, %d\n", c, c); // REMOVE
        mpz_powm(cipher_char, base, power, mod);
        mpz_export(&cipher, NULL, 1, sizeof(size_t), 0, 0, cipher_char);
        fwrite(&cipher, sizeof(size_t), 1, ciphertext_file);
    }

    // while ((c=fgetc(plaintext_file))!=EOF){
    //     mpz_set_ui(base, c);
    //     // printf("%c, %d\n", c, c); // REMOVE
    //     mpz_powm(cipher_char, base, power, mod);
    //     mpz_export(&cipher, NULL, 1, sizeof(size_t), 0, 0, cipher_char);
    //     fwrite(&cipher, sizeof(size_t), 1, ciphertext_file);
    // }


    fclose(plaintext_file);
    fclose(ciphertext_file);
}

void decrypt(char const* ciphertext_dir, char const* plaintext_dir, char const* key_file_dir){
    FILE* plaintext_file, *ciphertext_file, *key_file;
    mpz_t power, mod, base, plaintext_p_char;

    mpz_init2(power, KEY_LENGTH);
    mpz_init2(mod, KEY_LENGTH);
    mpz_init2(plaintext_p_char, sizeof(char));
    mpz_init2(base, sizeof(char));
    size_t key_buffer[2];

    if(access(key_file_dir, F_OK)!=0){   // doesnt work with windows os
        printf("Err: Invalid key file directory\n");
        return;
    }

    // read encryption key from key_file
    key_file = fopen(key_file_dir, "r");
    fread(key_buffer, sizeof(size_t), 2, key_file);
    mpz_import(mod, 1, 1, sizeof(size_t), 0, 0, &key_buffer[0]);
    mpz_import(power, 1, 1, sizeof(size_t), 0, 0, &key_buffer[1]);
    fclose(key_file);
    
    ciphertext_file = fopen(ciphertext_dir, "r");
    plaintext_file = fopen(plaintext_dir, "w");

    long file_l = file_length(ciphertext_file);

    // TODO: Check if 255 ascii is last char outside of m1
    // TODO: DECRYPT/ENCRYPT CHUNKS NOT CHAR BY CHAR
    size_t cipher;
    char plaintext_char;
    for (size_t i = 0; i < file_l/sizeof(size_t); i++)
    {
        fread(&cipher, sizeof(size_t), 1, ciphertext_file);
        mpz_set_ui(base, cipher);
        // printf("%lu\n", cipher); // REMOVE
        mpz_powm(plaintext_p_char, base, power, mod);
        mpz_export(&plaintext_char, NULL, 1, sizeof(char), 0, 0, plaintext_p_char);
        // printf("%c\n", plaintext_char); // REMOVE
        fwrite(&plaintext_char, sizeof(char), 1, plaintext_file);
    }
    
    
    // do{    
        
    // }
    // while (!feof(ciphertext_file));
    
    fclose(plaintext_file);
    fclose(ciphertext_file);
}

void generate_keys(void){
    srand(time(NULL)); // init randomization

    mpz_t prime_q, prime_p, e, n, d, lambda, tmp;

    // TODO: CHECK Randomeness
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    gmp_randseed_ui(rand_state, rand()); // random seed

    mpz_init2(prime_p, KEY_LENGTH/2);
    mpz_init2(prime_q, KEY_LENGTH/2);
    mpz_init2(n, KEY_LENGTH);
    mpz_init2(e, KEY_LENGTH);
    mpz_init2(d, KEY_LENGTH);
    mpz_init2(lambda, KEY_LENGTH);
    mpz_init2(tmp, KEY_LENGTH);

    mpz_urandomb(prime_p, rand_state, KEY_LENGTH/2);
    mpz_urandomb(prime_q, rand_state, KEY_LENGTH/2);
    // mpz_set_ui(tmp, INT_MAX); // REMOVE
    // gmp_printf("%Zd, %Zd, %Zd\n", prime_p, prime_q, tmp);

    // p,q primes
    mpz_nextprime(prime_p, prime_p);
    mpz_nextprime(prime_q, prime_q);

    // n=p*q
    mpz_mul(n, prime_p, prime_q);

    // lambda(n) = (p-1)*(q-1)
    mpz_sub_ui(tmp, prime_p, 1);
    mpz_set(lambda, tmp);
    mpz_sub_ui(tmp, prime_q, 1);
    mpz_mul(lambda, lambda, tmp);
    // gmp_printf("%Zd, %Zd, %Zd\n", prime_p, prime_q, lambda); // REMOVE

    // find e as coprime of lambda(n) && e < lambda(n)
    do
    {
        mpz_urandomb(tmp, rand_state, KEY_LENGTH);
        mpz_mod(e, tmp, lambda); // find random e < lambda(n)
        mpz_nextprime(e, e);     // find next prime of e
        mpz_gcd(tmp, e, lambda); // compute gcd of e,lambda
        // gmp_printf("%Zd, %Zd\n", e, tmp); // REMOVE
    } while (mpz_cmp(e,lambda)>=0 || mpz_cmp_ui(tmp, 1)!=0); // check if next_prime(e) still < lambda && gcd ==1
    
    // compute d as the inverse of e mod lambda(n)
    mpz_invert(d, e, lambda);

    FILE *private_dir, *public_dir;
    size_t buffer[2];
    
    // create public key
    mpz_export(&buffer[0], NULL, 1, sizeof(size_t), 0, 0, n);
    mpz_export(&buffer[1], NULL, 1, sizeof(size_t), 0, 0, e);

    // REMOVE
    // printf("From Export: n: %lu, e: %lu\n", (unsigned long) buffer[0], (unsigned long) buffer[1]);
    // printf("From Export: n: %lu, e: %lu\n", sizeof(buffer[0]), sizeof(buffer[1]));

    // store public key
    public_dir = fopen("public.key", "w");
    fwrite(buffer, sizeof(size_t), 2, public_dir);
    fclose(public_dir);

    // change 2nd number at buffer from e to d and store to private key
    mpz_export(&buffer[1], NULL, 1, sizeof(size_t), 0, 0, d);

    private_dir = fopen("private.key", "w");
    fwrite(buffer, sizeof(size_t), 2, private_dir);
    fclose(private_dir);

    // Clear variables
    mpz_clear(prime_p);
    mpz_clear(prime_q);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(lambda);
    mpz_clear(tmp);
    gmp_randclear(rand_state);
}

long file_length(FILE* file){
    size_t size;
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);

    return size;
}