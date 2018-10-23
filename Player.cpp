/**
 *	Player.cpp
 *	Function: Defines functions declared in Player.h
 *
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#include "stdafx.h"
#include "Player.h"
#include <fstream>
#include <sstream>

Player::Player(const char* nam) : name(nam), hand(), chips(20), won(0), lost(0)
, cumulativeBet(0), folded(false) {
	ifstream ifs(name + ".txt");
	if (ifs.is_open()) {
		string line;
		getline(ifs, line);
		istringstream iss(line);
		unsigned int prevChips, win, loss;
	
		// ADD CHIP NUMBER WHEN PLAYERS LEAVE
		if (iss >> win) {
			won = win;
		}
		if (iss >> loss) {
			lost = loss;
		}
		if (iss >> prevChips) {
			chips = prevChips;
		}
	}
}

bool poker_rank_player(shared_ptr<Player> a, shared_ptr<Player> b) {
	// if either of them is singular, return in favor of other
	if (a == 0 && b != 0) {
		return false;
	}
	if (a != 0 && b == 0) {
		return true;
	}

	// else return result of comparing their hands
	return poker_rank(a->hand, b->hand);
}

bool poker_rank_player_SCS(shared_ptr<Player> a, shared_ptr<Player> b) {
	// if either of them is singular, return in favor of other
	if (a == 0 && b != 0) {
		return false;
	}
	if (a != 0 && b == 0) {
		return true;
	}

	Hand bestOfA = a->hand.bestHand();
	Hand bestOfB = b->hand.bestHand();
	// else return result of comparing thei hands
	return poker_rank(bestOfA, bestOfB);
}