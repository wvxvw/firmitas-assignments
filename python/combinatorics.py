import heapq

from primes import factors

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
