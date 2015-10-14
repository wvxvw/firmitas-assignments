#!/usr/bin/env python

def fib(n):
    total, last = 1, 1
    for i in range(n - 2):
        temp = total
        total += last
        last = temp
    return total

def digits(n):
    n = abs(n)
    result = []
    while n > 0:
        result.append(n % 10)
        n //= 10
    return result

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
