// poker.cpp : Starts the game. First initiates the game.
//			  Then add each player from the command line arguments to the
//			  game and repeatedly calls before_round(), round() and
//	       	  after_round() until there are only two players left.
// Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>

#include "stdafx.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "TexasHoldEm.h"
#include <iostream>
#include <sstream>
using namespace std;

const int MIN_ARGS = 4;
const int FIRST_ARG = 1;
const int MIN_PLAYERS = 1;

int usageRule(string progName, string msg);

int main(int argc, char* argv[])
{
	if (argc < MIN_ARGS) {
		string progName = argv[0];
		string msg = "Game name followed by two or more names of players.";
		return usageRule(progName, msg);
	}
	// put all players into a vector for convenience of restarting game
	vector<string> plrs;
	for (int i = FIRST_ARG + 1; i < argc; ++i) {
		plrs.push_back(argv[i]);
	}
	string gameName = argv[FIRST_ARG];

	while (true) {
		try {
			cout << gameName << endl << endl;
			Game::start_game(gameName);
		}
		catch (returnCode ret) {
			if (ret == unknown_game) {
				cout << "Unknow game format" << endl;
			}

			else if (ret == game_already_started) {
				cout << "There is already an existing game" << endl;
			}

			return ret;
		}
		shared_ptr<Game> gameptr = Game::instance();

		for (unsigned int i = 0; i < plrs.size(); i++) {
			try {
				gameptr->add_player(plrs[i]);
			}
			catch (returnCode ret) {
				if (ret == already_playing) {
					cout << "Player " << argv[i] << " is already playing. Wasn't added." << endl;
				}
			}
		}

		while (gameptr->num_players() > MIN_PLAYERS) {
			try {
				gameptr->before_round();
				gameptr->round();
				gameptr->after_round();
			}
			catch (returnCode ret) {
				// Save stats of remaining players
				gameptr->stop();
				gameptr->stop_game();
				if (ret == not_enough_cards) {
					cout << "Ran out of cards" << endl;
					return ret;
				}
				throw ret;
			}
			catch (...) {
				cout << "Unknown exception occured" << endl;
				return unknown;
			}
		}




		try {
			// Save stats of remaining players
			gameptr->stop();
			gameptr->stop_game();
		}
		catch (returnCode ret) {
			if (ret == no_game_in_progress) {
				cout << "There is no game in progress" << endl;
			}
			else {
				cout << "Unknown error" << endl;
			}
			return ret;
		}
		catch (...) {
			cout << "Unknown error" << endl;
			return unknown;
		}

		cout << "Do you want to play another game? ('yes' or 'no): ";
		string response;
		cin >> response;
		while (response != "yes" && response != "no") {
			cout << "Invalid response. Please try again: ";
			cin >> response;
		}

		if (response == "yes") {
			cout << "Please enter the name of game followed by the names of players: " << endl;
			string line;
			cin.ignore();
			getline(cin, line);
			istringstream iss(line);

			if (!(iss >> gameName)) {
				cout << "No game name detected. Ending program" << endl;
				return no_game_name;
			}

			plrs.clear();
			string pName;
			while (iss >> pName) {
				plrs.push_back(pName);
			}

			while (plrs.size() < MIN_PLAYERS + 1) {
				cout << "There needs to be a minimum of 2 players. Only player names: " << endl;

				getline(cin, line);
				istringstream iss2(line);

				plrs.clear();
				string pName2;
				while (iss2 >> pName2) {
					plrs.push_back(pName2);
				}
			}
			cout << endl;
		}

		else { // "no"
			cout << endl << "Ending program" << endl;
			break;
		}
	}
	return success;
}

int usageRule(string progName, string msg) {
	cout << "Wrong usage\nThe first argument should be: " << progName
		<< endl << "And other arguments should be: " << msg << endl;
	return wrong_usage;
}
