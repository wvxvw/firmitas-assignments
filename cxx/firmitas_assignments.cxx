#include <iostream>
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
    primes(int n) : pos(0), seq(primes_seq(n)) { }
    
    int next() {
        if (pos < seq.size()) {
            return seq[pos++];
        }
        extend();
        return next();
    }
};

bool decomposes_goldbach(int n) {
    return true;
}

int main() {
    std::cout << "Primes:\n";
    primes ps(100);
    int p;
    
    // for (int i : primes_seq(100)) {
    //     std::cout << i << "\n";
    // }
    while (p = ps.next()) {
        std::cout << "ps: " << p << "\n";
        if (p > 150) break;
    }
}
