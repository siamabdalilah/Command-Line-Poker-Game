/**
 *	Game.h
 *	Function: Declares the Game abstract base and its member
 *			  variables and functions.
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#pragma once
#include "Player.h"
#include <memory>

class Game {
protected:
	unsigned int pot;
	size_t dealer;
	static shared_ptr<Game> gameptr;
	Deck deck;
	vector<shared_ptr<Player>> players;
	void remove(shared_ptr<Player> play);
	void print_player(shared_ptr<Player>);
	void generateDeck();

	virtual void bet();
	void clearCumulativeBets();
	void clearFoldStatus();
	void printOtherPlayers(int pl, int siz);

	Game();
public:
	
	static shared_ptr<Game> instance();
	static void start_game(const string& type); 
	static void stop_game();
	void add_player(const string& playerName);
	shared_ptr<Player> find_player(const string& name);
	size_t num_players();

	virtual int before_turn(Player&) = 0;
	virtual int turn(Player&) = 0;
	virtual int after_turn(Player&) = 0;
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;
	virtual void stop();
};