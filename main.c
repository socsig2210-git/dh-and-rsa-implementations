#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>


// void foo (mpz_t result, const mpz_t param, unsigned long n)
// {
//     unsigned long i;
//     mpz_mul_ui (result, param, n);
//     for (i = 1; i < n; i++)
//         mpz_add_ui (result, result, i*7);
// }

// int main(int argc, char const *argv[])
// {
//     for (int i=0; i<argc; i++){
//         printf("%d. %s\n", i, argv[i]);
//     }

//     printf("%d\n", argc);
//     return 0;
// }

int main(void){
    mpz_t a;
    mpz_init(a);
    mpz_set_ui(a, 10);
    gmp_printf("%Zd\n", a);
}