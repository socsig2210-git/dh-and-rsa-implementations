#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <gmp.h>

void print_description(void);
void init_numbers(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
void clear_numbers(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);

int main(int argc, char **argv)
{   
    // Argument parsing
    char const *output_dir, *p, *g, *key_a, *key_b;
    FILE* output;

    int opt;
    while((opt = getopt(argc, argv, "o:p:g:a:b:h")) != -1){
        switch(opt){
            case 'o':
                output_dir = optarg;
                break;
            case 'p':
                p = optarg;
                break;
            case 'g':
                g = optarg;
                break;
            case 'a':
                key_a = optarg;
                break;
            case 'b':
                key_b = optarg;
                break;
            case 'h':
                if (argc > 2)
                {
                    printf("Err: Invalid Arguments, use -h for help\n");
                    return -1;
                }
                
                print_description();
                return 0;
            case ':':
                printf("Err: Option needs a value\n, use -h for help");
                break;
            case '?':
                printf("Err: Unkownk option: %c, use -h for help\n", optopt);
                break;
        }
    }

    // Check if all arguments have a value
    if(output_dir==NULL || p==NULL || g==NULL || key_a==NULL || key_b==NULL){
        printf("Err: Invalid Arguments, use -h for help\n");
        return -1;
    }

    // Initialize multi-precision variables
    mpz_t private_key_a, private_key_b, prime, base, public_key_A, public_key_B, secret_key_A, secret_key_B; 

    init_numbers(private_key_a, private_key_b, public_key_A, public_key_B, secret_key_A,
        secret_key_B, prime, base);

    // set values from arguments
    mpz_set_str(private_key_a, key_a, 0);
    mpz_set_str(private_key_b, key_b, 0);   
    mpz_set_str(prime, p, 0);
    mpz_set_str(base, g, 0);

    // check if arguments are > 0
    if(mpz_sgn(private_key_a)!=1 || mpz_sgn(private_key_b)!=1 
        || mpz_sgn(prime)!=1 || mpz_sgn(base)!=1)
    {
        printf("Err: Arguments should be greater than 0\n");
        clear_numbers(private_key_a, private_key_b, public_key_A, public_key_B, secret_key_A,
            secret_key_B, prime, base);
        return -1;
    }

    // private key a, b should be < prime
    if(mpz_cmp(prime, private_key_a) < 0 || mpz_cmp(prime, private_key_b) < 0){
        printf("Err: -a & -b arguments should be less than the -p argument\n");
        clear_numbers(private_key_a, private_key_b, public_key_A, public_key_B, secret_key_A,
            secret_key_B, prime, base);
        return -1;
    }

    // check if vars prime and base are prime numbers
    if(mpz_probab_prime_p(prime, 50)!=2){
        printf("Err: -p argument should be a prime number\n");
        clear_numbers(private_key_a, private_key_b, public_key_A, public_key_B, secret_key_A,
            secret_key_B, prime, base);
        return -1;
    }

    // Compute public keys
    mpz_powm(public_key_A, base, private_key_a, prime);
    mpz_powm(public_key_B, base, private_key_b, prime);

    // Compute and check common secret
    mpz_powm(secret_key_A, public_key_B, private_key_a, prime);
    mpz_powm(secret_key_B, public_key_A, private_key_b, prime);

    if(mpz_cmp(secret_key_A, secret_key_B) != 0){
        printf("Err: DH Algorithm Failed\n");
        clear_numbers(private_key_a, private_key_b, public_key_A, public_key_B, secret_key_A,
            secret_key_B, prime, base);
        return -1;
    }

    // Create output file
    output = fopen(output_dir, "w");
    gmp_fprintf(output, "%Zd, %Zd, %Zd", public_key_A, public_key_B, secret_key_A);
    fclose(output);

    // Clear multi-precision variables
    clear_numbers(private_key_a, private_key_b, public_key_A, public_key_B, secret_key_A,
        secret_key_B, prime, base);

    return 0;
}

void init_numbers(mpz_t private_key_a, mpz_t private_key_b, mpz_t public_key_A,
    mpz_t public_key_B, mpz_t secret_key_A, mpz_t secret_key_B, mpz_t prime, mpz_t base)
{
    mpz_init(private_key_a);
    mpz_init(private_key_b);
    mpz_init(public_key_A);
    mpz_init(public_key_B);
    mpz_init(secret_key_A);
    mpz_init(secret_key_B);
    mpz_init(prime);
    mpz_init(base);
}

void clear_numbers(mpz_t private_key_a, mpz_t private_key_b, mpz_t public_key_A,
    mpz_t public_key_B, mpz_t secret_key_A, mpz_t secret_key_B, mpz_t prime, mpz_t base)
{
    mpz_clear(private_key_a);
    mpz_clear(private_key_b);
    mpz_clear(public_key_A);
    mpz_clear(public_key_B);
    mpz_clear(secret_key_A);
    mpz_clear(secret_key_B);
    mpz_clear(prime);
    mpz_clear(base);
}

void print_description(void){
    printf("Usage: ./dh_assign_1 [ARGS]\n");
    printf("\nAn implementation of the Diffie-Hellman algorithm\n");
    printf("\nThis tool, based on the 2 private keys and the 2 prime numbers given as input,\n");
    printf("creates 2 public keys by implementing the algorithm. Both of the keys and their\n");
    printf("common secret key are then stored in an output file\n");
    printf("\nMandatory arguments [ARGS]:\n");
    printf("  -a, private key a\n");
    printf("  -b, private key b\n");
    printf("  -g, prime number used as base\n");
    printf("  -p, prime number uses as mod\n");
    printf("  -o <file>, path to output file\n\n");
}