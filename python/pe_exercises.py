#!/usr/bin/env python

import math
import heapq
import bisect
import os

from cards import Hand

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

def nub(nums):
    """
    Returns a copy of nums with duplicates removed.  The numbers
    in nums will be solrted in increasing order.
    """
    heap = nums[:]
    heapq.heapify(heap)
    result = [heapq.heappop(heap)]
    while heap:
        lead = heapq.heappop(heap)
        if result[-1] != lead:
            result.append(lead)
    return result

def has_square(n):
    """
    Returns True iff n contains a perfect square as a factor.
    """
    facts = factors(n)
    return len(nub(facts)) < facts
        
def decomposes_goldbach(n):
    """
    Returns True iff n can be decomposed into p + 2 * i^2, where
    p is a prime and i is an integer.
    """
    for p in primes():
        if p > n:
            return False
        rest = n - p
        if rest % 2 == 0 and int(math.sqrt(rest // 2)) ** 2 == rest // 2:
            return True

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
    
    What is the index of the first term in the Fibonacci sequence 
    to contain 1000 digits?
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

def exercise_46():
    """
    It was proposed by Christian Goldbach that every odd composite
    number can be written as the sum of a prime and twice a square.
    
    9 = 7 + 2×12
    15 = 7 + 2×22
    21 = 3 + 2×32
    25 = 7 + 2×32
    27 = 19 + 2×22
    33 = 31 + 2×12
    
    It turns out that the conjecture was false.
    
    What is the smallest odd composite that cannot be written as
    the sum of a prime and twice a square?
    """
    for c in odd_composite():
        if not decomposes_goldbach(c):
            return c

def exercise_50():
    """
    The prime 41, can be written as the sum of six consecutive primes:
    41 = 2 + 3 + 5 + 7 + 11 + 13

    This is the longest sum of consecutive primes that adds to a prime
    below one-hundred.

    The longest sum of consecutive primes below one-thousand that adds
    to a prime, contains 21 terms, and is equal to 953.

    Which prime, below one-million, can be written as the sum of the
    most consecutive primes?
    """
    sieve, prefixes, max_prime, longest_seq = [], [0], 0, 0
    for p in primes():
        if prefixes[-1] + p > 1000000:
            break
        sieve.append(p)
        prefixes.append(prefixes[-1] + p)
    terms = 1
    for i in range(len(prefixes)):
        for j in range(i + terms, len(prefixes)):
            n = prefixes[j] - prefixes[i]
            if j - i > terms:
                is_p, sieve = is_prime(n, sieve)
                if is_p:
                    terms, max_prime = j - i, n
    return max_prime, terms

def exercise_54():
    """
    In the card game poker, a hand consists of five cards 
    and are ranked, from lowest to highest, in the following way:
    
        High Card: Highest value card.
        One Pair: Two cards of the same value.
        Two Pairs: Two different pairs.
        Three of a Kind: Three cards of the same value.
        Straight: All cards are consecutive values.
        Flush: All cards of the same suit.
        Full House: Three of a kind and a pair.
        Four of a Kind: Four cards of the same value.
        Straight Flush: All cards are consecutive values of same suit.
        Royal Flush: Ten, Jack, Queen, King, Ace, in same suit.
    
    The cards are valued in the order:
    2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace.
    
    If two players have the same ranked hands then the rank made up of
    the highest value wins; for example, a pair of eights beats a pair
    of fives (see example 1 below). But if two ranks tie, for example,
    both players have a pair of queens, then highest cards in each
    hand are compared (see example 4 below); if the highest cards tie
    then the next highest cards are compared, and so on.
    
    Consider the following five hands dealt to two players:
    Hand | Player 1 | Player 2 | Winner
    1	 	5H 5C 6S 7S KD
    Pair of Fives
    	 	2C 3S 8S 8D TD
    Pair of Eights
    	 	Player 2
    2	 	5D 8C 9S JS AC
    Highest card Ace
    	 	2C 5C 7D 8S QH
    Highest card Queen
    	 	Player 1
    3	 	2D 9C AS AH AC
    Three Aces
    	 	3D 6D 7D TD QD
    Flush with Diamonds
    	 	Player 2
    4	 	4D 6S 9H QH QC
    Pair of Queens
    Highest card Nine
    	 	3D 6D 7H QD QS
    Pair of Queens
    Highest card Seven
    	 	Player 1
    5	 	2H 2D 4C 4D 4S
    Full House
    With Three Fours
    	 	3C 3D 3S 9S 9D
    Full House
    with Three Threes
    	 	Player 1
    
    The file, poker.txt, contains one-thousand random hands dealt to
    two players. Each line of the file contains ten cards (separated
    by a single space): the first five are Player 1's cards and the
    last five are Player 2's cards. You can assume that all hands are
    valid (no invalid characters or repeated cards), each player's
    hand is in no specific order, and in each hand there is a clear
    winner.
    
    How many hands does Player 1 win?
    """
    # for testing
    if '__file__' not in globals():
        __file__ = '/home/wvxvw/Projects/firmitas/python/pe_exercises.py'
    p = os.path
    parent = p.dirname(p.dirname(__file__))
    with open(p.join(parent, './etc/p054_poker.txt'), 'r') as f:
        wins = 0
        for line in f:
            cards = line.strip().split(" ")
            if Hand(cards[:5]) > Hand(cards[5:]):
                wins += 1
        return wins
