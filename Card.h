/**
 *	Card.h
 *	Function	: Declares all the enums for struct Card and return codes for
 *				  this program. Also contains declaration for struct Card and
 *			      relavant functions.
 *	Author		: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#pragma once

#include <string>
using namespace std;

// the suits are valued in alphabetical order
enum suits { clubs, diamonds, hearts, spades };

// ranks has "ace" as highest value
enum ranks {
	two, three, four, five, six, seven, eight, nine,
	ten, jack, queen, king, ace
};

// all the return codes declared in one enum so that no two case have same value
enum returnCode {
	success, openingFailure, emptyFile, unknownFileException, wrongHandSize,
	onlyShuffle, wrong_usage, unknown, game_already_started,
	unknown_game, no_game_in_progress, already_playing, player_not_added,
	not_enough_cards, no_game_name
};

typedef struct Card {
	bool operator< (const struct Card &) const;
	bool operator== (const struct Card &) const;
	string toString();
	suits suit;
	ranks rank;
	bool faceDown;

private:
	string rankToString();
	string suitToString();
}Card;


ostream& operator<< (ostream &os, const struct Card &);