/**
 *	Hand.h
 *	Function: Declares the Hand class and relevant functions. 
 *			  Also declared rank for poker hands and size of 
 *			  a valid hand.
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#pragma once

#include "Deck.h"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

enum handRank {
	na, onePair, twoPairs, threeOfKind, straight, flush, fullHouse,
	fourOfKind, straightFlush
};

const int validHandSize = 5;

class Hand {
	friend ostream& operator<< (ostream &os, Hand &hand);
	friend Hand& operator<<(Hand&,  Deck &);
	friend Deck;

public:
	int size() const;
	bool operator== (Hand&) const;
	bool operator< (const Hand &) const;
	string toString();
	string handToString();
	Card& operator[] (size_t loc);
	Card remove(size_t);
	// empties the hand
	void clear();
	void add_card(Card c);
	Hand bestHand();
private:
	vector<Card> cards;
	
	// returns the rank of a valid hand 
	handRank getHand();
	// declared as friend to Hand to give access to local variables
	friend bool poker_rank(Hand &, Hand &);

	// functions to get number the rank of the card that appears rankCount
	// number of times in a hand. The "notI"s mean to check ranks except
	// notI
	static int getCount(const Hand &card, int rankCount);
	static int getCount(const Hand &card, int rankCount, int notI);
	static int getCount(const Hand &card, int rankCount, int notI1, int notI2);

	vector<Hand> comb(Hand& hand, int k);
};

// Compared two hands according to their poker rank
bool poker_rank(Hand & h1, Hand & h2);

ostream& operator<< (ostream &os, Hand &);
Hand& operator<< (Hand &hand, Deck &deck);

