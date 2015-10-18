#include <vector>
#include <cmath>
#include <algorithm>

bool is_zero(int x) { return x == 0; }

std::vector<int> primes_seq(int n) {
    std::vector<int> result(n - 1);
    size_t root = (int)std::sqrt(n);
    int counter = 0;
    
    for (int &i : result) i = counter++;
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
    primes() : pos(0), seq(primes_seq(10)) { }
    
    primes(int n) : pos(0), seq(primes_seq(n)) { }
    
    int next() {
        if (pos < seq.size()) {
            return seq[pos++];
        }
        extend();
        return next();
    }
    
    bool is_prime(int n) {
        return std::binary_search(seq.begin(), seq.end(), n);
    }

    std::size_t size() { return seq.size(); }

    int& operator [](std::size_t n) { return seq[n]; }
};

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
