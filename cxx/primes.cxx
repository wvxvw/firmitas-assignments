/*! \file primes.cxx
 *  This file contains some functions to compute prime numbers and
 *  sequences of primes.
 */
#include <vector>
#include <cmath>
#include <algorithm>

/*! \fn bool is_zero(int x)
 *  \brief Helper for checking whether the given integer is a zero.
 *  \param x The integer suspected to be zero
 *  \returns true iff `x = 0`.
 *
 *  This function is used to filter out zeros when generating
 *  Eratosphenes sieve.  But, you are welcome to use it wherever you like.
 */
bool is_zero(int x) { return x == 0; }

/*! \fn std::vector<int> primes_seq(int n)
 *  \brief Generates a sequence of prime numbers.
 *  \param n The largest prime in the generaetd sequence will be at most this big.
 *  \returns A vector containing primes in ascending order.
 *
 *  This function is a simple implementation or the sieve of Eratosphenes.  It
 *  at first generates a vector of `n` integers in increasing order, then it looks
 *  into consequtive elements of this vector and:
 *
 *  if the element is a zero, it skips it.
 *
 *  otherwise, for all indices which are multiple of this element, except for the
 *  first one it replaces the elements with zeros.
 *
 *  Finally, it removes all zeros.
 */
std::vector<int> primes_seq(int n) {
    std::vector<int> result(n - 1);
    size_t root = (int)std::sqrt(n);
    int counter = 0;
    
    for (int& i : result) i = counter++;
    for (std::size_t r = 2; r < root; r++) {
        if (result[r] > 0) {
            for (size_t k = r * 2; k < n - 1; k += r) {
                result[k] = 0;
            }
        }
    }
    result[1] = 0;
    result.erase(std::remove_if(result.begin(), result.end(), is_zero),
        result.end());
    return result;
}

/*! \class primes
 *  \brief This class is a container for the sieve of Erathosphenes.
 *
 *  This class is used to iterate or add elements to the sieve of Erathosphenes.
 *  It can also efficiently assert containment (which would be equivalent to
 *  primality test in the given range).
 */
class primes {
    
    std::vector<int> seq;
    
    std::size_t pos;
    
    void extend() {
        int last = seq[seq.size() - 1];
        int root = (int)std::sqrt(last);
        int attempt = last + 2;
        bool found = false;
        
        while (!found) {
            for (int p : seq) {
                if (p > root) {
                    found = true;
                    break;
                }
                if (attempt % p == 0) {
                    attempt += 2;
                    break;
                }
            }
        }
        seq.push_back(attempt);
    }
  public:
    
    /*! \fn primes()
     *  \brief Default constructor.
     *
     *  Creates a sieve of all primes up to 10.
     */
    primes() : pos(0), seq(primes_seq(10)) { }

    /*! \fn primes(int n)
     *  \brief Creates a sieve, where the largest element is at most `n`.
     *
     *  Creates a sieve of with a number of elements up to (and
     *  possibly including) `n`.
     */
    primes(int n) : pos(0), seq(primes_seq(n)) { }

    /*! \fn int next()
     *  \brief Returns the next element of the sieve.
     *  \returns Next prime number.
     *
     *  Once the existing sieve is exhausted, this function will attempt to
     *  extend it with the next prime.
     */
    int next() {
        if (pos < seq.size()) {
            return seq[pos++];
        }
        extend();
        return next();
    }

    /*! \fn bool is_prime(int n) const
     *  \brief Tests whether `n` is prime.
     *  \returns `true` iff `n` is a prime in this sieve.
     *
     *  This test will not try to extend the sieve in case the input
     *  is greater than its greates element.  This means that this method
     *  will give incorect answers for primes outside the range.
     */
    bool is_prime(int n) const {
        return std::binary_search(seq.begin(), seq.end(), n);
    }

    /*! \fn std::size_t size() const
     *  \brief The size of this sieve
     *  \returns the size of this sieve.
     *
     *  The size of the sieve is the number of primes it contains, note that this
     *  will always be less then the largest prime it contains (not to be confused
     *  with the constructor's argument).
     */
    std::size_t size() const { return seq.size(); }

    /*! \fn int& operator [](std::size_t n)
     *  \brief Returns the referenct to the element of the sieve at index `n`.
     *  \returns A reference to an element in the sieve.
     *
     *  The the reference returned is suitable for assignment, so beware!
     */
    int& operator [](std::size_t n) { return seq[n]; }
};

/*! \fn bool decomposes_goldbach(int n)
 *  \brief Asserts Goldbach's conjecture.
 *  \param n The integer to which the conjecture's constraings are applied.
 *  \returns `true` iff the conjecture holds for `n`.
 *
 *  Goldbach conjectured that for every composite integer \f$i\f$ it
 *  holds that \f$i = p + n^2\f$ for some prime \f$p\f$ and an integer \f$n\f$.
 *  This function is an implementation of this conjecture.
 */
bool decomposes_goldbach(int n) {
    primes ps;
    int p;
    bool result;

    while (p = ps.next()) {
        if (p > n) {
            result = false;
            break;
        }
        int rest = n - p;
        if (rest % 2 == 0) {
            int half = rest / 2;
            if (std::pow(std::floor(std::sqrt(half)), 2) == half) {
                result = true;
                break;
            }
        }
    }
    return result;
}
