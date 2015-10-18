/*! \file combinatoricx.cxx
 *  This file contains some commonly used functions over integers.
 */
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

/*! \struct card_t
 *  \brief This struct represents a playing card from a 52-cards deck.
 *  \var suit is the card's suit.
 *  \var denom is the card's denomination.
 */
struct card_t {
    unsigned char suit;
    
    unsigned char denom;
    
    card_t() : suit(' '), denom(' ') {}
    
    card_t(unsigned char suit_, unsigned char denom_) : suit(suit_), denom(denom_) {}
};

/*! \fn bool card_cmp(card_t a, card_t b)
 *  \brief This is a helper function used to sort cards based on their denomination.
 *  \param a The card prior in the sequence.
 *  \param b The card following in the sequence.
 *  \returns true if a must precede b.
 *
 *  Helps `std::sort()` to sort a collection of `card_t` based on card's denomination.
 */
bool card_cmp(card_t a, card_t b) { return a.denom < b.denom; }

/*! \fn std::vector<card_t> parse_cards(std::string raw)
 *  \brief Parses hand (5 cards) from string.
 *  \param raw Taw string containing 5 card codes.
 *  \returns A vector with parsed cards.
 *
 *  Given a string with five card codes will create a sorted vector with five
 *  cards.  Cards are sorted by their denomination.
 */
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

/*! \fn std::vector<unsigned char> group_cards(std::vector<card_t> cards)
 *  \brief Groups cards by denomination.
 *  \param cards Vector containing a sorted sequence of cards (not modified).
 *  \returns A vector with stretches of cards of the same denomination.
 *
 *  This function implements something very similar to run-length encoding.
 *  Even though this is a canonical group-by functionality, this function
 *  exploits the fact that the input sequence is sorted, so it is trivially
 *  linear.
 */
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

/*! \class hand
 *  \brief This class provides operations on poker hand.
 *
 *  This class provides methods assessing the value of one's poker hand.
 */
class hand {
    std::vector<card_t> cards;
    
    std::vector<unsigned char> stretch;

  public:
  
    /*! \fn hand(std::string raw)
     *  \brief Creates new poker hand.
     *  \param raw Raw poker hand.
     *
     *  Constructor will parse the hand from the given string and initialize its
     *  fields based on the parsing results.
     */
    hand(std::string raw) :
            cards(parse_cards(raw)),
            stretch(group_cards(cards)) { }

    /*! \fn bool n_of_a_kind(int n) const
     *  \brief Verifies if the hand has `n` cards of the same denomination.
     *  \param n Number of same cards needed to be found.
     *  \returns true iff the hand has this exact number of cards of the same
     *  denomination.
     *
     *  Looks for at least one stretch of the required size.  Note that the hand
     *  that contains 3 cards with the same denomination will not be considered
     *  as having 2 cards of the same denomination, unless it also contains
     *  exactly 2 cards of the same denomination.  Other methods rely on this
     *  behavior.
     */
    bool n_of_a_kind(int n) const {
        return std::find(stretch.begin(), stretch.end(), n) != stretch.end();
    }

    /*! \fn bool royal_flush() const
     *  \brief Verifies whether the hand has a T,J,Q,K,A sequence all of the
     *  same suit.
     *  \returns true iff the hand contains the ``Royal Flush'' combination.
     */
    bool royal_flush() const {
        return cards[0].denom == 10 && straight_flush();
    }

    /*! \fn bool straight_flush() const
     *  \brief Verifies whether the hand has five cards of consequent
     *  denomination, all of same suit.
     *  \returns true iff the hand contains the ``Straight Flush'' combination.
     */
    bool straight_flush() const { return straight() && flush(); }

    /*! \fn bool four_of_a_kind() const
     *  \brief Verifies whether the hand contains four card of the same
     *  denomination.
     *  \returns true iff the hand contains the ``Four Of A Kind'' combination.
     */
    bool four_of_a_kind() const { return n_of_a_kind(4); }

    /*! \fn bool full_house() const
     *  \brief Verifies whether the hand contains a pair and that other three
     *  cards are of the same denomination.
     *  \returns true iff the hand contains the ``Full House'' combination.
     */
    bool full_house() const {
        if (one_pair()) {
            return (stretch[0] == 2, stretch[1] == 3 ||
                    stretch[1] == 2, stretch[0] == 3);
        }
        return false;
    }

    /*! \fn bool flush() const
     *  \brief Verifies whether all cards in the hand are of the same suit.
     *  \returns true iff the hand contains the ``Flush'' combination.
     */
    bool flush() const {
        unsigned char elt = cards[0].suit;
        return std::count_if(
            cards.begin(), cards.end(),
            [elt](card_t x) -> bool { return x.suit == elt; }) == cards.size();
    }

    /*! \fn bool straight() const
     *  \brief Verifies that there are no two cards of the same denomination and
     *  that the difference between the highest-valued card and the lowest-valued
     *  one is exactly five.
     *  \returns true iff the hand contains the ``straight'' combination.
     */
    bool straight() const {
        for (std::size_t t = 1; t < cards.size(); t++) {
            if (cards[t].denom != cards[t - 1].denom + 1) {
                return false;
            }
        }
        return true;
    }

    /*! \fn bool three_of_a_kind() const
     *  \brief Verifies whether the hand contains exactly three cards of the
     *  same denomination.
     *  \returns true iff the hand contains the ``Three Of A Kind'' combination.
     */
    bool three_of_a_kind() const { return n_of_a_kind(3); }

    /*! \fn bool two_pairs() const
     *  \brief Verifies whether the hand contains two different pairs of cards
     *  of the same denomination.
     *  \returns true iff the hand contains the ``Two Pairs'' combination.
     */
    bool two_pairs() const {
        if (n_of_a_kind(2)) {
            return std::count(stretch.begin(), stretch.end(), 2) == 2;
        }
        return false;
    }

    /*! \fn bool one_pair() const
     *  \brief Verifies whether the hand contains a pair of cards
     *  of the same denomination.
     *  \returns true iff the hand contains the ``One Pair'' combination.
     */
    bool one_pair() const { return n_of_a_kind(2); }

    /*! \fn int value() const
     *  \brief Calculates the value of the hand.
     *  \returns An integer in range [3,23] indicating the total value of the hand.
     *
     *  The value of the hand is determined by it either having one of the eight
     *  known special combinations, or by its highest-ranked card.  The combinations
     *  are ranked as follows:
     *  - *One Pair*:        15
     *  - *Two Pairs*:       16
     *  - *Three Of A Kind*: 17
     *  - *Straight*:        18
     *  - *Flush*:           19
     *  - *Full House*:      20
     *  - *Four Of A Kind*:  21
     *  - *Straight Flush*:  22
     *  - *Royal Flush*:     23
     *  .
     */
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

    /*! \fn std::vector<card_t> scoring_cards() const
     *  \brief Returns only the cards that can affect a tie-break.
     *  \returns A vector of cards which can affect a tie-break.
     *
     *  Ties are broken by first considering the cards that participate in
     *  forming one of the special combinations \see hand::value.  The cards
     *  are also considered in the order from highest to lowest value.
     */
    std::vector<card_t> scoring_cards() const {
        std::vector<card_t> result;
        
        switch (value()) {
            case 15: case 16: case 17: case 20: case 21:
                {
                    std::size_t j = 0;
                    for (int s : stretch) {
                        if (s > 1) {
                            result.resize(result.size() + s);
                            std::copy(&cards[j], &cards[j + s],
                                      &result[result.size() - s]);
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

    /*! \fn int break_tie(const hand& other) const
     *  \brief Computes amount of points by which one hand outweighs the other.
     *  \returns The number of points by which this hand outweighs the other.
     *
     *  Once the tie is detected, this function is given the `other` hand, which
     *  has the same score, but might have different cards supporting that score.
     *  Ties are broken by first looking into the highest-ranked cards in the
     *  special combinations and later by looking at the remaining cards.
     *
     *  Note that it is still possible to draw (we don't establish any rules which
     *  assign weights to suits).
     */
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

    /*! \fn bool operator >(const hand& other) const
     *  \brief Establishes whether this hand's value is greater than the
     *  value of the other hand.
     *  \returns `true` iff the value of this hand is greate than that of the other.
     *
     *  Invoking this operator will set in motion `hand::value()` and possibly
     *  `hand::break_tie()` in order to establish the winning hand.
     */
    bool operator >(const hand& other) const {
        int a = value();
        int b = other.value();
        if (a == b) a += break_tie(other);
        return a > b;
    }
};
