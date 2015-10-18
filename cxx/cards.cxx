#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

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
