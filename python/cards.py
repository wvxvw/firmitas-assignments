import re

def cards_cmp(a, b):
    """
    Sorting helper, returns zero if a == b, a positive integer if
    a > b and a negative integer otherwise.
    """
    val_a, val_b = a[0], b[0]
    for val, denom in zip(['T', 'J', 'Q', 'K', 'A'], range(10, 15)):
        if val_a == val:
            val_a = denom
        if val_b == val:
            val_b = denom
    val_a = int(val_a)
    val_b = int(val_b)
    return val_a - val_b

def denom(card):
    """
    The denominaton of the given card.  The denomination is defined as
    follows:
        2 through 10 - the cards value
        J            - 11
        Q            - 12
        K            - 13
        A            - 14
    """
    val = card[0]
    try:
        return { 'T': 10, 'J': 11, 'Q': 12, 'K': 13, 'A': 14 }[val]
    except KeyError:
        return int(val)

class Hand(object):
    """
    Stores a poker hand.  Poker hand contains five cards from the deck
    of 52 playing cards.  This class also stores information on
    how many cards of the same denomination are there in the hand.
    """
    def __init__(self, cards):
        """
        Creates new poker hand.  The argument must be a list with cards
        represented as strings.
        """
        cards.sort(cards_cmp)
        self._handlers = [self.royal_flush, self.straight_flush,
                          self.four_of_a_kind, self.full_house, self.flush,
                          self.straight, self.three_of_a_kind,
                          self.two_pairs, self.one_pair]
        self._cards = cards
        self._stretches = []
        self._value = None

    def n_of_a_kind(self, n):
        """
        Returns True if the hand contains exactly n cards of the same
        denomination.
        """
        if not self._stretches:
            counter = 1
            for a, b in zip(self._cards, self._cards[1:] + ['X']):
                if a[0] != b[0]:
                    self._stretches.append(counter)
                    counter = 1
                else: counter += 1
        return n in self._stretches
        
    def royal_flush(self):
        """
        Returns True if all cards in the hand are of the same suit and
        the smallest card is a ten.
        """
        return self._cards[0][0] == 'T' and self.straight_flush()

    def straight_flush(self):
        """
        Similar to royal flush, except cards should be of consequtive 
        denominations (no need to start at 10).
        """
        return self.straight() and self.flush()
    
    def four_of_a_kind(self):
        """
        Retruns True if the hand contains exactly four cards of the same 
        denomination.
        """
        return self.n_of_a_kind(4)
    
    def full_house(self):
        """
        Returns True if the hand contains a pair and a triple of the same 
        denomination.
        """
        if self.one_pair():
            return self._stretches == [2, 3] or self._stretches == [3, 2]
        return False
    
    def flush(self):
        """
        Returns True if all cards in the hand come from the same suit.
        """
        return bool(re.match(r'.(.)(.\1){4}', ''.join(self._cards)))
    
    def straight(self):
        """
        Returns True if cards' values have no ``gaps'' in between.
        """
        prev = denom(self._cards[0])
        for card in self._cards[1:]:
            if not denom(card) == prev + 1:
                return False
            prev += 1
        return True
    
    def three_of_a_kind(self):
        """
        Returns True if the hand has a triple.
        """
        return self.n_of_a_kind(3)
    
    def two_pairs(self):
        """
        Returns True if the hand has two different pairs.
        """
        if self.n_of_a_kind(2):
            return self._stretches.count(2) == 2
        return False
    
    def one_pair(self):
        """
        Returns True if the hand has one pair.
        """
        return self.n_of_a_kind(2)

    def value(self):
        """
        Computes the value of the hand.  Hands are assigned value based on
        some special combinations, such as ``Royal Flush'', ``Three Of A Kind''
        etc, or by taking the denomination of the highest-ranked card.

        See the documentation of other methods of this class for more details
        on the meaning of special combinations.
        """
        if not self._value:
            for v, f in zip(range(24, 10, -1), self._handlers):
                if f():
                    self._value = v
                    break
            if not self._value:
               self._value = denom(self._cards[-1]) 
        return self._value

    def scoring_cards(self, score):
        """
        When two hands have the same value, this procedure is used to compute
        the cards which may potentially affect the tie break.  The cards are
        returned in decreasing order (i.e. the first card should be 
        considered first to break the tie, then the second and so on).
        """
        result = []
        if score in [15, 16, 17, 20, 21]:
            j = 0
            for i in self._stretches:
                if i > 1:
                    result += self._cards[j:j + i]
                j += i
        else:
            result = self._cards[:]
        return reversed(result)

    def break_tie(self, other):
        """
        When two hands have the same value, this is used to break ties.  Note that
        ties can still occur because the suit plays no role in this process.
        """
        val = self.value()
        scoring_a = self.scoring_cards(val)
        scoring_b = other.scoring_cards(val)
        for a, b in zip(scoring_a, scoring_b):
            da, db = denom(a), denom(b)
            if da != db:
                return da - db
        return 0

    def __lt__(self, other):
        """
        Implements the less than operator.
        """
        val_a, val_b = self.value(), other.value()
        if val_a == val_b:
            return self.break_tie(other) < 0
        return val_a < val_b

    def __le__(self, other):
        """
        Implements the less than or equal operator.
        """
        val_a, val_b = self.value(), other.value()
        if val_a == val_b:
            return self.break_tie(other) <= 0
        return val_a <= val_b
    
    def __eq__(self, other):
        """
        Implements the equality operator.
        """
        if self.value() == other.value():
            return self.break_tie(other) == 0
        return False
    
    def __ne__(self, other):
        """
        Implements the not equal operator.
        """
        return not self.__eq__(other)
    
    def __gt__(self, other):
        """
        Implements the greater than operator.
        """
        return not self.__le__(other)
    
    def __ge__(self, other):
        """
        Implements the greater or equal operator.
        """
        return not self.__lt__(other)
