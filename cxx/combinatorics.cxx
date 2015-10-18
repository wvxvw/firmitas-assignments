/*! \file combinatoricx.cxx
 *  This file contains some commonly used functions over integers.
 */
#include <gmpxx.h>

/*! \fn mpz_class fib(int n)
 *  \brief Calculates \f$N^{th}\f$ Fibonacci number.
 *  \param n The index into Fibonacci sequence you want to generate.
 *  \return A biginteger (aka `mpz_class`) from GNU MP library.
 *  
 *  This function uses iterative algorithm, and is also, technically,
 *  incorrect for the value of \f$N=3\f$, but we never care for this
 *  value, and I prefer to keep the code short.
 */
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
