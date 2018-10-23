/**
 *	Deck.h
 *	Function: Declares the class Deck and relevant functions. Forward
 *			  declared the Hand class within deck.
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#pragma once
#include "Card.h"
#include <vector>
using namespace std;


class Deck {
public:
	Deck() = default;
	Deck(char* fileName);
	returnCode load(char* fileName);
	void shuffle(); 
	int size() const;
	vector<Card>& getDeck();
	void add_card(Card card);

private:
	class Hand;

	vector<Card> deck;
	const int firstChar = 0;

	// condition functions
	static bool correctLength(string word);
	static bool correctRank(string word);
	static bool correctSuit(string word);
	static bool onlySpaces(string line);

	// functions to get enums from word
	static ranks wordToRank(char name, int code);
	static suits wordToSuit(char name);
	
	friend Hand& operator<< (Hand& hand, Deck& deck);

};


ostream& operator<< (ostream &os,  Deck&);
