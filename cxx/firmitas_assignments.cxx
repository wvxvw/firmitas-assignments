#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>

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

struct card_t {
    unsigned char suit;
    
    unsigned char denom;
    
    card_t() : suit(' '), denom(' ') {}
    
    card_t(unsigned char suit_, unsigned char denom_) : suit(suit_), denom(denom_) {}
};

bool card_cmp(card_t a, card_t b) { return a.denom < b.denom; }

std::vector<card_t> parse_cards(std::string raw) {
    std::vector<card_t> result(5);
    int state = 0;
    unsigned char suit, denom;
    std::string denoms("123456789TJQKA");
    std::size_t place = 0;
    std::string normalized = raw[raw.size() - 1] == ' ' ? raw : raw + " ";
    
    for (auto c : normalized) {
        switch (state) {
            case 0:
                denom = denoms.find(c) + 1;
                state = 1;
                break;
            case 1:
                suit = (unsigned char)c;
                state = 2;
                break;
            case 2:
                struct card_t created(suit, denom);
                result[place++] = created;
                state = 0;
        }
    }
    std::sort(result.begin(), result.end(), card_cmp);
    return result;
}

std::vector<unsigned char> group_cards(std::vector<card_t> cards) {
    std::vector<unsigned char> result;
    unsigned char counter = 1;

    for (std::size_t t = 1; t < cards.size(); t++) {
        if (cards[t - 1].denom == cards[t].denom) {
            counter++;
        } else {
            result.push_back(counter);
            counter = 1;
        }
    }
    result.push_back(counter);
    return result;
}

typedef bool (*tester)(void);

class hand {
    std::vector<card_t> cards;
    
    std::vector<unsigned char> stretch;

  public:
    hand(std::string raw) :
            cards(parse_cards(raw)),
            stretch(group_cards(cards)) { }

    bool n_of_a_kind(int n) const {
        return std::find(stretch.begin(), stretch.end(), n) != stretch.end();
    }

    bool royal_flush() const {
        return cards[0].denom == 10 && straight_flush();
    }

    bool straight_flush() const { return straight() && flush(); }

    bool four_of_a_kind() const { return n_of_a_kind(4); }

    bool full_house() const {
        if (one_pair()) {
            return (stretch[0] == 2, stretch[1] == 3 ||
                    stretch[1] == 2, stretch[0] == 3);
        }
        return false;
    }

    bool flush() const {
        unsigned char elt = cards[0].suit;
        return std::count_if(
            cards.begin(), cards.end(),
            [elt](card_t x) -> bool { return x.suit == elt; }) == cards.size();
    }

    bool straight() const {
        for (std::size_t t = 1; t < cards.size(); t++) {
            if (cards[t].denom != cards[t - 1].denom + 1) {
                return false;
            }
        }
        return true;
    }

    bool three_of_a_kind() const { return n_of_a_kind(3); }

    bool two_pairs() const {
        if (n_of_a_kind(2)) {
            return std::count(stretch.begin(), stretch.end(), 2) == 2;
        }
        return false;
    }

    bool one_pair() const { return n_of_a_kind(2); }
    
    int value() const {
        bool result;
        for (std::size_t t = 0; t < 9; t++) {
            switch (t) {
                case 0: result = royal_flush();     break;
                case 1: result = straight_flush();  break;
                case 2: result = four_of_a_kind();  break;
                case 3: result = full_house();      break;
                case 4: result = flush();           break;
                case 5: result = straight();        break;
                case 6: result = three_of_a_kind(); break;
                case 7: result = two_pairs();       break;
                case 8: result = one_pair();        break;
            }
            if (result) {
                return 15 + (8 - t);
            }
        }
        return (int)cards[cards.size() - 1].denom;
    }

    std::vector<card_t> scoring_cards() const {
        std::vector<card_t> result;
        
        switch (value()) {
            case 15: case 16: case 17: case 20: case 21:
                {
                    std::size_t j = 0;
                    for (int s : stretch) {
                        if (s > 1) {
                            result.resize(result.size() + s);
                            std::copy(&cards[j], &cards[j + s], &result[result.size() - s]);
                        }
                        j += s;
                    }
                }
                break;
            default:
                result.reserve(cards.size());
                std::copy(cards.begin(), cards.end(), result.begin());
        }
        std::reverse(result.begin(), result.end());
        return result;
    }
    
    int break_tie(const hand& other) const {
        std::vector<card_t> scoring_a = scoring_cards();
        std::vector<card_t> scoring_b = other.scoring_cards();

        for (std::size_t t = 0; t < scoring_a.size(); t++) {
            if (scoring_a[t].denom != scoring_b[t].denom) {
                return scoring_a[t].denom - scoring_b[t].denom;
            }
        }
        return 0;
    }
    
    bool operator >(const hand& other) const {
        int a = value();
        int b = other.value();
        if (a == b) a += break_tie(other);
        return a > b;
    }
};

int euler_46() {
    primes ps;
    int result, p, c;
    bool found = false;

    while (!found) {
        p = ps.next();
        c = p + 2;
        p = ps.next();
        while (c < p) {
            if (!decomposes_goldbach(c)) {
                result = c;
                found = true;
                break;
            }
            c += 2;
        }
    }
    return result;
}

int euler_50() {
    primes ps(1000000);
    std::vector<int> sums(ps.size());
    int max_prime;
    std::size_t terms = 1;
    std::size_t top;
    
    sums[0] = ps[0];
    for (std::size_t i = 1; i < ps.size(); i++) {
        if (sums[i - 1] + ps[i] > 1000000) {
            top = i;
            break;
        }
        sums[i] = sums[i - 1] + ps[i];
    }
    for (std::size_t i = 0; i < top; i++) {
        for (std::size_t j = terms + i; j < top; j++) {
            int n = sums[j] - sums[i];
            if (j - i > terms && ps.is_prime(n)) {
                terms = j - i;
                max_prime = n;
            }
        }
    }
    return max_prime;
}

int euler_54() {
    std::ifstream in("../etc/p054_poker.txt");
    std::string line;
    int result = 0;
    while (std::getline(in, line)) {
        hand a(line.substr(0, 3 * 5));
        hand b(line.substr(3 * 5, 3 * 10 - 1));
        if (a > b) {
            result++;
        }
    }
    std::cout << "\n";
    return result;
}

int main() {
    std::cout << "euler 46: " << euler_46() << "\n";
    std::cout << "euler 50: " << euler_50() << "\n";
    std::cout << "euler 54: " << euler_54() << "\n";
    return 0;
}
