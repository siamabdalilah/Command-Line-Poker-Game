/**
 *	SevenCardStud.h
 *	Function: SevenCardStud class is declared which is
 *			  An implementation of Game abstract base class.
 *			  The member variables and methods are declared.
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#pragma once

#include "Game.h"

class SevenCardStud : public Game {
protected:
	
	Deck discardedPile;
	
public:
	const int validHand = 7;
	SevenCardStud();
	virtual int before_turn(Player&);
	virtual int turn(Player&);
	virtual int after_turn(Player&);
	virtual int before_round();
	virtual int round();
	virtual int after_round();
	
private:
	void first_turn();
	void mid_turn();
	void last_turn();
};
