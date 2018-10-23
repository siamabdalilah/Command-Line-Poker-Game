/**
*	Deck.cpp
*	Function: Defines the functions declared in deck.h
*			  Extended to have an add_card() method. 
*	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
*/

#include "stdafx.h"
#include "Deck.h"
#include "Card.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>


Deck::Deck(char* fileName) {
	returnCode ret = load(fileName);
	if (ret != success) {
		throw ret;
	}
}
returnCode Deck::load(char* fileName) {

	try {
		ifstream ifs(fileName);
		if (!ifs.is_open()) {
			cout << "File could not be opened." << endl;
			return openingFailure;
		}

		// return if empty file
		string line, word;
		getline(ifs, line);
		if (line == "") {
			cout << "File is empty" << endl;
			return emptyFile;
		}

		do {
			// ignore line completely if its empty. no error message generated
			if (onlySpaces(line)) {
				continue;
			}

			// checks for comment. If found, remove the comment
			size_t pos = line.find_first_of("//");
			if (pos != string::npos) {
				line = line.substr(0, pos);
			}

			istringstream iss(line);
			Card card;

			// scan every word on "line"
			while (iss >> word) {
				if (correctLength(word) && correctRank(word) && correctSuit(word)) {
					// suitCode is the index of word where the suit is mentioned
					int suitCode = word.length() - 1;

					card.rank = wordToRank(word[firstChar], suitCode);
					card.suit = wordToSuit(word[suitCode]);
					//cout << "dbbsd" << endl;
					deck.push_back(card);
				}
			}
		} while (getline(ifs, line));

		ifs.close();
	}
	catch (...) {
		return unknownFileException;
	}
	return success;
}

void Deck::shuffle() {
	// mostly copied over from c++ reference
	random_device random;
	mt19937 randy(random());
	std::shuffle(deck.begin(), deck.end(), randy);
}
int Deck::size() const {
	return deck.size();
}
vector<Card>& Deck::getDeck() {
	return deck;
}
void Deck::add_card(Card card) {
	deck.push_back(card);
}

bool Deck::correctLength(string word) {
	int len = word.length();
	if (len == 2 || len == 3) {
		return true;
	}

	cout << "Invalid defintion. Skipping" << endl << endl;
	return false;
}
bool Deck::correctRank(string word) {
	// check to see if rank is within the acceptable set
	if (word.length() == 3 && word[0] == '1' && word[1] == '0') {
		return true;
	}
	if (word.length() == 2 && ((word[0] >= '2' && word[0] <= '9')
		|| word[0] == 'k' || word[0] == 'K' || word[0] == 'a' || word[0] == 'A'
		|| word[0] == 'q' || word[0] == 'Q' || word[0] == 'j' || word[0] == 'J')) {
		return true;
	}

	cout << "Invalid defintion. Skipping" << endl << endl;
	return false;
}
bool Deck::correctSuit(string word) {
	// check to see if suit is within the acceptable set
	char suit = word[word.length() - 1];
	if (suit == 'C' || suit == 'c' || suit == 'D'
		|| suit == 'd' || suit == 'H' || suit == 'h'
		|| suit == 'S' || suit == 's') {
		return true;
	}

	cout << "Invalid defintion. Skipping" << endl << endl;
	return false;
}
bool Deck::onlySpaces(string line) {
	// check if line has only spaces
	for (int i = 0; line[i] != '\0'; ++i) {
		if (line[i] != ' ') {
			return false;
		}
	}
	return true;
}

ranks Deck::wordToRank(char name, int code) {
	ranks rank;
	if (code == 2) {
		rank = ten;
	}
	else {
		switch (name) {
		case '2':
			rank = two;
			break;
		case '3':
			rank = three;
			break;
		case '4':
			rank = four;
			break;
		case '5':
			rank = five;
			break;
		case '6':
			rank = six;
			break;
		case '7':
			rank = seven;
			break;
		case '8':
			rank = eight;
			break;
		case '9':
			rank = nine;
			break;
		case 'J':
			rank = jack;
			break;
		case 'j':
			rank = jack;
			break;
		case 'Q':
			rank = queen;
			break;
		case 'q':
			rank = queen;
			break;
		case 'K':
			rank = king;
			break;
		case 'k':
			rank = king;
			break;
		case 'A':
			rank = ace;
			break;
		case 'a':
			rank = ace;
			break;

		}
	}

	return rank;
}
suits Deck::wordToSuit(char name) {
	suits suit;
	switch (name) {
	case 'C':
		suit = clubs;
		break;
	case 'c':
		suit = clubs;
		break;
	case 'D':
		suit = diamonds;
		break;
	case 'd':
		suit = diamonds;
		break;
	case 'H':
		suit = hearts;
		break;
	case 'h':
		suit = hearts;
		break;
	case 'S':
		suit = spades;
		break;
	case 's':
		suit = spades;
		break;
	}

	return suit;
}

ostream& operator<< (ostream& os,  Deck &d) {
	vector<Card> temp = d.getDeck();
	for (size_t i = 0; i < d.getDeck().size(); i++) {
		os << temp[i].toString() << endl;
	}
	return os;
}