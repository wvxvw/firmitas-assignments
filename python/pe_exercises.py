#!/usr/bin/env python

def fib(n):
    """
    It is possible to compute N'th Fibonacci number analytically,
    but I was asked not to do this, and to keep it simple. So, here
    you have the iterative version.
    """
    total, last = 1, 1
    for i in range(n - 2):
        temp = total
        total += last
        last = temp
    return total

def digits(n):
    """
    The digits of the argument, not counting the sign.
    """
    n = abs(n)
    result = []
    if n > 0:
        while n > 0:
            result.append(n % 10)
            n //= 10
    else:
        result.append(n)
    return result

def is_prime(n, sieve):
    """
    Tests whether n is prime.  Sieve should contain all primes less than n.
    """
    for p in sieve:
        if n % p == 0:
            return False
    return True

def next_prime(sieve):
    """
    Given the sieve containing primes searches for the prime greater than the
    last element of the sieve, such that there are no primes less than this
    one which are not in the sieve.
    """
    last = sieve[-1] + 2
    while not is_prime(last, sieve):
        last += 2
    sieve.append(last)

def primes(n = None):
    """
    Iterator.  Generates primes.
    """
    sieve = [2, 3, 5, 7, 11, 13, 17, 19, 23, 27]
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
    
def exercise_25():
    """
    The Fibonacci sequence is defined by the recurrence relation:
    
        Fn = Fn−1 + Fn−2, where F1 = 1 and F2 = 1.
    
    Hence the first 12 terms will be:
    
        F1 = 1
        F2 = 1
        F3 = 2
        F4 = 3
        F5 = 5
        F6 = 8
        F7 = 13
        F8 = 21
        F9 = 34
        F10 = 55
        F11 = 89
        F12 = 144
    
    The 12th term, F12, is the first term to contain three digits.
    
    What is the index of the first term in the Fibonacci sequence to contain 1000 digits?
    """
    # Start with some initial guess: fib(1000) and double the size
    # until we find a number with more than 1000 digits once done,
    # subtract the half of the last increment, depending on whether we
    # overshoot or undershoot subtract or add the quarter of the last
    # increment and so on, until we find two adjacent Fibonacci
    # numbers such that the first has at most 999 digits and the
    # second has 1000 or more digits.  This will ensure that we
    # calculate at most log(1000) ~= 10 operations.  Afterwards,
    # because there could be several Fibonacci numbers of the same
    # length, we'll find the smallest one using linear search (this
    # won't take more than log(2^4) = 4 operations.
    n = 1000
    guess = fib(n)
    while len(digits(guess)) < 1000:
        n *= 2
        guess = fib(n)
    m = n // 4
    n -= m
    while m > 1:
        guess = fib(n)
        m //= 2
        if len(digits(guess)) > 1000:
            n -= m
        else:
            n += m
    while len(digits(guess)) >= 1000:
        n -= 1
        guess = fib(n)
    return n + 1
