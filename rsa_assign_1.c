#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include <gmp.h>


void generate_keys(void);
int main(int argc, char **argv)
{   
    // Argument parsing
    char const *input_dir, *output_dir, *key_file_dir, *g, *d, *e;
    FILE* output, input, key_file;

    int opt;
    while((opt = getopt(argc, argv, "i:o:k:gdeh")) != -1){
        switch(opt){
            case 'i':
                input_dir = optarg;
                break;
            case 'o':
                output_dir = optarg;
                break;
            case 'k':
                key_file_dir = optarg;
                break;
            case 'g':
                generate_keys();
                return 0;
                break;
            case 'd':
                break;
            case 'e':
                break;
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
}

void generate_keys(void){
    srand(time(NULL));

    mpz_t prime_q, prime_p, n, lambda, tmp;

    mpz_init(prime_p);
    mpz_init(prime_q);
    mpz_init(n);
    mpz_init(lambda);
    mpz_init(tmp);

    mpz_set_ui(prime_p, rand());
    mpz_set_ui(prime_q, rand());

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
    
    FILE *private_dir, *public_dir;
    public_dir = fopen("public.key", "w");
    private_dir = fopen("private.key", "w");

    gmp_fprintf(private_dir, "%Zd", prime_p);
    gmp_fprintf(public_dir, "%Zd", prime_q);

    fclose(public_dir);
    fclose(private_dir);
}

void generate_random(){
    
}