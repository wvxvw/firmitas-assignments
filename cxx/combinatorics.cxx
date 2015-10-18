#include <gmpxx.h>

mpz_class fib(int n) {
    mpz_class total = 1;
    mpz_class last = 1;
    while (n > 3) {
        mpz_class temp = total;
        total += last;
        last = temp;
        n--;
    }
    return total;
}
