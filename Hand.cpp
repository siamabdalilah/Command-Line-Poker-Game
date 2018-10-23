/**
 *	Hand.cpp
 *	Function: Defines the functions declared in Hand.h.
 *			  Also declared rank for poker hands and size of
 *			  a valid hand.
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#include "stdafx.h"
#include "Hand.h"
#include "Card.h"
#include <stdexcept>

int Hand::size() const {
	return cards.size();
}
bool Hand::operator== (Hand &hand) const {
	if (size() != hand.size()) {
		return false;
	}

	for (int i = 0; i < hand.size(); ++i) {
		// ! operator used instead of implementing !=
		if (!(cards[i] == hand.cards[i])) {
			return false;
		}
	}
	return true;
}
bool Hand::operator< (const Hand &hand) const {
	// First checks if any of the first cards are different
	// Iterates over the size of smaller of the two Hand objects
	// If all the first cards are same, compares size
	int size = (this->size() > hand.size()) ? hand.size() : this->size();
	for (int i = 0; i < size; ++i) {
		if (cards[i] < hand.cards[i]) {
			return true;
		}
		else if (cards[i] == hand.cards[i]) {
			continue;
		}
		else {
			return false;
		}
	}
	return (size == this->size());
}
Card& Hand::operator[] (size_t loc) {
	// if location is within range of hand, returns card at loc
	// else throws out of range exception
	if (loc >= 0 && loc < cards.size()) {
		return cards[loc];
	}
	throw out_of_range("location not in range for hand");
}
void Hand::add_card(Card c) {
	cards.push_back(c);
}
Card Hand::remove(size_t loc) {
	// erases card at loc
	vector<Card>::iterator iter = cards.begin() + loc;
	Card temp = cards[loc];
	cards.erase(iter);
	return temp;
}
string Hand::toString() {
	// appends the string representations of all the cards with a space
	// in between
	string result = "";
	for (auto iter = cards.begin(); iter < cards.end(); ++iter) {
		result += iter->toString() + " ";
	}
	return result;
}
handRank Hand::getHand() {
	
	bool isFlush = true, isStraight = true;
	bool is4ofK = false, is3ofK = false, is2p = false, is1p = false;
	int rankCount[13];
	for (int i = 0; i < 13; ++i) {
		rankCount[i] = 0;
	}
	int twoCount = 0;

	// first calculates the number of times a given rank appears
	// in the hand and stores in rankCount
	// At the same time checks if the hand is a flush or straight

	rankCount[cards[0].rank]++;
	for (int i = 1; i < validHandSize; ++i) {
		if (cards[i].rank != cards[i - 1].rank + 1) {
			isStraight = false;
		}
		if (cards[i].suit != cards[i - 1].suit) {
			isFlush = false;
		}
		rankCount[cards[i].rank]++;
	}

	// flush and straight means straight flush
	if (isFlush && isStraight) {
		return straightFlush;
	}
	if (isFlush) {
		// to remove ambiguity
		return handRank::flush;
	}
	if (isStraight) {
		return straight;
	}

	for (int i = 0; i < 13; ++i) {
		// if one rank appears 4 times its 4 of a kind
		if (rankCount[i] == 4) {
			is4ofK = true;
			break;
		}
		// if one rank appears 3 times its 3 of a kind
		else if (rankCount[i] == 3) {
			is3ofK = true;
		}

		// checks if a given rank appers twice
		else if (rankCount[i] == 2) {
			// if yes and is also 3ofk, it means this is full house
			is1p = true;
			if (is3ofK) {
				break;
			}
			++twoCount;
			// if number of ranks appearing is 2, 2p is true
			if (twoCount == 2) {
				is2p = true;
				break;
			}
		}

	}

	if (is4ofK) {
		return fourOfKind;
	}
	if (is3ofK && is1p) {
		return fullHouse;
	}
	if (is3ofK) {
		return threeOfKind;
	}
	if (is2p) {
		return twoPairs;
	}
	if (is1p) {
		return onePair;
	}
	return na;

}
string Hand::handToString() {
	handRank rank = getHand();
	switch (rank) {
	case straightFlush:
		return "straight flush";
	case fourOfKind:
		return "four of a kind";
	case fullHouse:
		return "full house";
	case handRank::flush:
		return "flush";
	case straight:
		return "straight";
	case threeOfKind:
		return "three of a kind";
	case twoPairs:
		return "two pairs";
	case onePair:
		return "one pair";
	}

	return "no rank";
}

void Hand::clear() {
	cards.clear();
}

int Hand::getCount(const Hand &card, int rankCount) {
	// counts which rank appears a given number of times
	int temp[13];

	for (int i = 0; i < 13; ++i) {
		temp[i] = 0;
	}
	for (int i = 0; i < 5; ++i) {
		temp[card.cards[i].rank]++;
	}
	int i = 0;
	for (i = 13; i >= 0; --i) {
		if (temp[i] == rankCount) {
			break;
		}
	}

	return i;
}
int Hand::getCount(const Hand &card, int rankCount, int notI) {
	int temp[13];
	// counts which rank appears a given number of times
	// except the specified notI

	for (int i = 0; i < 13; ++i) {
		temp[i] = 0;
	}
	for (int i = 0; i < 5; ++i) {
		temp[card.cards[i].rank]++;
	}
	int i = 0;
	for (i = 0; i < 13; i++) {
		// checks so that the rank returning is not notI
		if (temp[i] == rankCount && i != notI) {
			break;
		}
	}

	return i;
}
int Hand::getCount(const Hand &card, int rankCount, int notI1, int notI2) {
	int temp[13];

	for (int i = 0; i < 13; ++i) {
		temp[i] = 0;
	}
	for (int i = 0; i < 5; ++i) {
		temp[card.cards[i].rank]++;
	}
	int i = 0;
	for (i = 0; i < 13; i++) {
		if (temp[i] == rankCount && i != notI1 && i != notI2) {
			break;
		}
	}

	return i;
}


ostream& operator<< (ostream &os, Hand& hand) {
	os << hand.toString();
	return os;
}
Hand& operator<< (Hand &hand, Deck &d) {
	hand.cards.push_back(d.getDeck().back());
	d.getDeck().pop_back();
	//sort(hand.cards.begin(), hand.cards.end());
	return hand;
}

bool poker_rank(Hand  & h1, Hand & h2) {
	if (h1.size() != validHandSize || h2.size() != validHandSize) {
		cout << "Can't assign poker hand to hand." << endl;
		throw wrongHandSize;
	}
	handRank h1rank = h1.getHand();
	handRank h2rank = h2.getHand();
	if (h1rank > h2rank) {
		return false;
	}

	if (h1rank < h2rank) {
		return true;
	}


	else {
		int firstHandRank, firstHandRank2;
		int secondHandRank, secondHandRank2;

		switch (h1rank) {
		case straightFlush:
			// compare highest card
			if (h1.cards[5] < h2.cards[5]) {
				return true;
			}
			break;
		case fourOfKind:
			// compare the card that appears 4 times
			firstHandRank = Hand::getCount(h1, 4);
			secondHandRank = Hand::getCount(h2, 4);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			break;

		case fullHouse:
			// compare the card that appears 3 times
			firstHandRank = Hand::getCount(h1, 3);
			secondHandRank = Hand::getCount(h2, 3);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}
			else {
				// if tie not broken, compare the rank that appears twice.
				firstHandRank = Hand::getCount(h1, 2);
				secondHandRank = Hand::getCount(h2, 2);
				if (firstHandRank < secondHandRank) {
					return true;
				}
			}
			break;
		case handRank::flush:
			// compare cards from highest to lowest in the hand
			// return if tie is broken
			for (int i = 4; i >= 0; --i) {
				if (h1.cards[i] < h2.cards[i]) {
					return true;
				}
				else if (h2.cards[i] < h1.cards[i]) {
					return false;
				}
			}
			break;
		case straight:
			// compare the high card
			if (h1.cards[5] < h2.cards[5]) {
				return true;
			}
			break;
		case threeOfKind:
			// compare rank that appears 3 times
			firstHandRank = Hand::getCount(h1, 3);
			secondHandRank = Hand::getCount(h2, 3);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}
			// then compare rank that appears 1 time
			firstHandRank = Hand::getCount(h1, 1);
			secondHandRank = Hand::getCount(h2, 1);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}

			// if tie not broken, find the other card that appears once and compare
			firstHandRank = Hand::getCount(h1, 1, firstHandRank);
			secondHandRank = Hand::getCount(h2, 1, secondHandRank);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}

			break;
		case twoPairs:
			// similar comparison idea as above
			firstHandRank = Hand::getCount(h1, 2);
			secondHandRank = Hand::getCount(h2, 2);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}

			firstHandRank = Hand::getCount(h1, 2, firstHandRank);
			secondHandRank = Hand::getCount(h2, 2, secondHandRank);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}

			firstHandRank = Hand::getCount(h1, 1);
			secondHandRank = Hand::getCount(h2, 1);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			break;
		case onePair:
			firstHandRank = Hand::getCount(h1, 2);
			secondHandRank = Hand::getCount(h2, 2);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}

			firstHandRank = Hand::getCount(h1, 1);
			secondHandRank = Hand::getCount(h2, 1);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}

			firstHandRank2 = firstHandRank;
			secondHandRank2 = secondHandRank;
			firstHandRank = Hand::getCount(h1, 1, firstHandRank);
			secondHandRank = Hand::getCount(h2, 1, secondHandRank);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}

			firstHandRank = Hand::getCount(h1, 1, firstHandRank, firstHandRank2);
			secondHandRank = Hand::getCount(h2, 1, secondHandRank, secondHandRank2);
			if (firstHandRank < secondHandRank) {
				return true;
			}
			else if (firstHandRank > secondHandRank) {
				return false;
			}
			break;
		case na:
			for (int i = 4; i >= 0; --i) {
				if (h1.cards[i] < h2.cards[i]) {
					return true;
				}
				else if (h2.cards[i] < h1.cards[i]) {
					return false;
				}
			}
			break;
		}

		return false;
	}
}
Hand Hand::bestHand() {
	// first get a vector of alll possible 5 card combos
	vector<Hand> setOfHands = comb(*this, 5);

	// the cards in hand are not maintained in sorted order for this lab.
	// so first we need to sort all cards within each hand
	for (unsigned int i = 0; i < setOfHands.size(); ++i) {
		sort(setOfHands[i].cards.begin(), setOfHands[i].cards.end());
	}
	// then sort the hands with poker_rank
	sort(setOfHands.begin(), setOfHands.end(), poker_rank);
	// return the highest hand
	return setOfHands[setOfHands.size() - 1];
}
vector<Hand> Hand::comb(Hand& hand, int k) {
	// base case: if k is one, just return the elements in new hands
	if (k == 1) {
		vector<Hand> vec;
		for (int i = 0; i < hand.size(); ++i) {
			Hand h;
			h.cards.push_back(hand.cards[i]);
			vec.push_back(h);
		}
		return vec;
	}

	vector<Hand> vec ;

	// for all parts of hand such that there is at least 1 element for the recursive call
	for (int i = 0; i < hand.size() - 2; ++i) {
		
		Hand h;
		// insert all cards from hand with index 1 past i to the end in h
		h.cards.insert(h.cards.end(), hand.cards.begin() + i + 1, hand.cards.end());
		// call comb() on h and assign to vec2
		vector<Hand> vec2 = comb(h, k - 1);
		
		// put i-th card of hand in all elements in vec2
		for (unsigned int j = 0; j < vec2.size(); ++j) {
			vec2[j].cards.push_back(hand.cards[i]);
		}

		// insert all elements of vec2 in vec
		vec.insert(vec.end(), vec2.begin(), vec2.end());
	}

	// return vector of all combos
	return vec;
}