import math
import bisect

def primes_seq(n):
    """
    Generates a sequence of primes up to n.
    """
    seq = range(n + 1)
    root = int(math.sqrt(n))
 
    for i in range(2, root + 1):
        if seq[i]:
            seq[2 * i::i] = [None] * (n // i - 1)
 
    return filter(None, seq[2:])

def is_next_prime(n, sieve):
    """
    Tests whether n is prime.  Sieve should contain all primes less than n.
    """
    root = int(math.sqrt(n))
    for p in sieve:
        if p > root:
            break
        if n % p == 0:
            return False
    return True

def is_prime(n, sieve):
    """
    Returns True if the number is a prime.  Sieve should contain some prime
    numbers to start the testing (the more the better).
    """
    if n < sieve[-1]:
        pos = bisect.bisect_left(sieve, n)
        return sieve[pos] == n, sieve
    root = int(math.sqrt(n))
    for s in sieve:
        if s > root:
            return True, sieve
        if n % s == 0:
            return False, sieve
    last = sieve[-1]
    while last < root:
        next_prime(sieve)
        last = sieve[-1]
        if last == n:
            return True, sieve
        if n % last == 0:
            return False, sieve
    return True, sieve

def next_prime(sieve):
    """
    Given the sieve containing primes searches for the prime greater than the
    last element of the sieve, such that there are no primes less than this
    one which are not in the sieve.
    """
    last = sieve[-1] + 2
    while not is_next_prime(last, sieve):
        last += 2
    sieve.append(last)

def primes(n = None):
    """
    Iterator.  Generates primes.
    """
    sieve = primes_seq(n or 10)
    m = 0
    while not n or m < n:
        try:
            yield sieve[m]
            m += 1
        except IndexError:
            next_prime(sieve)
    else:
        raise StopIteration()

def odd_composite():
    """
    Iterator.  Generates odd composite numbers.
    """
    last = 9
    for p in primes():
        if p <= last:
            last = p + 2
            continue
        for n in range(last, p - 1, 2):
            yield n
        last = p + 2

def factors(n):
    """
    Generates all prime factors of n.
    """
    fact = []
    for p in primes():
        if p > n: break
        while n % p == 0:
            fact.append(p)
            n //= p
    return fact
