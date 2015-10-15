import re

def cards_cmp(a, b):
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
    val = card[0]
    try:
        return { 'T': 10, 'J': 11, 'Q': 12, 'K': 13, 'A': 14 }[val]
    except KeyError:
        return int(val)

class Hand(object):
    
    def __init__(self, cards):
        cards.sort(cards_cmp)
        self._handlers = [self.royal_flush, self.straight_flush,
                          self.four_of_a_kind, self.full_house, self.flush,
                          self.straight, self.three_of_a_kind,
                          self.two_pairs, self.one_pair]
        self._cards = cards
        self._stretches = []
        self._value = None

    def n_of_a_kind(self, n):
        if not self._stretches:
            counter = 1
            for a, b in zip(self._cards, self._cards[1:] + ['X']):
                if a[0] != b[0]:
                    self._stretches.append(counter)
                    counter = 1
                else: counter += 1
        return n in self._stretches
        
    def royal_flush(self):
        return self._cards[0][0] == 'T' and self.straight_flush()

    def straight_flush(self):
        return self.straight() and self.flush()
    
    def four_of_a_kind(self):
        return self.n_of_a_kind(4)
    
    def full_house(self):
        if self.one_pair():
            return self._stretches == [2, 3] or self._stretches == [3, 2]
        return False
    
    def flush(self):
        return bool(re.match(r'.(.)(.\1){4}', ''.join(self._cards)))
    
    def straight(self):
        prev = denom(self._cards[0])
        for card in self._cards[1:]:
            if not denom(card) == prev + 1:
                return False
            prev += 1
        return True
    
    def three_of_a_kind(self):
        return self.n_of_a_kind(3)
    
    def two_pairs(self):
        if self.n_of_a_kind(2):
            return self._stretches.count(2) == 2
        return False
    
    def one_pair(self):
        return self.n_of_a_kind(2)

    def value(self):
        if not self._value:
            for v, f in zip(range(24, 10, -1), self._handlers):
                if f():
                    self._value = v
                    break
            if not self._value:
               self._value = denom(self._cards[-1]) 
        return self._value

    def scoring_cards(self, score):
        result = []
        if score in [15, 16, 17, 20, 21]:
            j = 0
            for i in self._stretches:
                if i > 1:
                    result += self._cards[j:j + i]
                j += i
        else:
            result = self._cards[:]
        return result

    def break_tie(self, other):
        val = self.value()
        scoring_a = reversed(self.scoring_cards(val))
        scoring_b = reversed(other.scoring_cards(val))
        for a, b in zip(scoring_a, scoring_b):
            da, db = denom(a), denom(b)
            if da != db:
                return da - db
        return 0

    def __lt__(self, other):
        val_a, val_b = self.value(), other.value()
        if val_a == val_b:
            return self.break_tie(other) < 0
        return val_a < val_b

    def __le__(self, other):
        val_a, val_b = self.value(), other.value()
        if val_a == val_b:
            return self.break_tie(other) <= 0
        return val_a <= val_b
    
    def __eq__(self, other):
        if self.value() == other.value():
            return self.break_tie(other) == 0
        return False
    
    def __ne__(self, other):
        return not self.__eq__(other)
    
    def __gt__(self, other):
        return not self.__le__(other)
    
    def __ge__(self, other):
        return not self.__lt__(other)
