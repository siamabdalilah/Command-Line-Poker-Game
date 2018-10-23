/**
 *	Player.h
 *	Function: Declares the Player class and a bool function to
 *			  compare two instances of it.
 *			  
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#pragma once
#include "Hand.h"
#include <memory>


typedef struct Player {
	string name;
	Hand hand;
	unsigned int chips;
	unsigned int won;
	unsigned int lost;
	unsigned int cumulativeBet;
	bool folded;

	Player(const char* name);

}Player;

bool poker_rank_player(shared_ptr<Player> a, shared_ptr<Player> b);
bool poker_rank_player_SCS(shared_ptr<Player> a, shared_ptr<Player> b);