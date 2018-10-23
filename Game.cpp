/**
 *	Game.cpp
 *	Function: Defines the functions declared in 
 *			  compare two instances of it.
 *
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "TexasHoldEm.h"
#include <fstream>

shared_ptr<Game> Game::gameptr = 0;

Game::Game() : deck(), pot(0), dealer(0) {

}

shared_ptr<Game> Game::instance() {
	return gameptr;
}
void Game::start_game(const string& type) {
	if (gameptr != 0) {
		throw game_already_started;
	}

	if (type != "FiveCardDraw" && type != "SevenCardStud" && type != "TexasHoldEm") {
		throw unknown_game;
	}

	if (type == "FiveCardDraw") {
		gameptr = make_shared<FiveCardDraw>();
	}
	else if (type == "TexasHoldEm") {
		gameptr = make_shared<TexasHoldEm>();
	}
	else {
		gameptr = make_shared<SevenCardStud>();
	}
}

void Game::stop_game() {
	if (gameptr == 0) {
		throw no_game_in_progress;
	}
	gameptr = 0;
	return;
}
void Game::add_player(const string& name) {
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i]->name == name) {
			throw already_playing;
		}
	}


	// TEST THIS
	Player newPlayer(&name[0]);
	if (newPlayer.chips == 0) {
		cout << "This player has no chips. Enter 1 to add player with 20 chips, 0 to cancel: ";
		int resp; cin >> resp;
		while (resp != 0 && resp != 1) {
			cout << "Invalid response. Please try again: ";
			cin >> resp;
		}
		if (resp == 0) {
			throw player_not_added;
		}
		else {
			newPlayer.chips = 20;
		}
	}
	players.emplace_back(make_shared<Player>(newPlayer));
}
shared_ptr<Player> Game::find_player(const string& name) {
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i]->name == name) {
			return players[i];
		}
	}
	return 0;
}
size_t Game::num_players() {
	return players.size();
}
void Game::remove(shared_ptr<Player> play) {
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i] == play) {
			players.erase(players.begin() + i);
			break;
		}
	}
}

void Game::print_player(shared_ptr<Player> player) {
	cout << "Name: " << player->name << ", Won: " << player->won
		<< ", Lost: " << player->lost << ", Chips: " << player->chips <<
		", Hand: "; //  << player->hand << endl;
	if (player->folded) {
		cout << "*Folded*";
	}
	else {
		cout << player->hand;
	}
	cout << endl;

}


void Game::clearCumulativeBets() {
	for (unsigned int i = 0; i < players.size(); ++i) {
		players[i]->cumulativeBet = 0;
	}
}

void Game::clearFoldStatus() {
	for (unsigned int i = 0; i < players.size(); ++i) {
		players[i]->folded = false;
	}
}

void Game::generateDeck() {
	// iterate over elements in ranks and suits
	deck.getDeck().clear();
	for (int rank = two; rank <= ace; rank++) {
		for (int suit = clubs; suit <= spades; suit++) {
			Card newCard;
			newCard.suit = (suits)suit;
			newCard.rank = (ranks)rank;
			deck.add_card(newCard);
		}
	}
}

void Game::stop() {
	for (unsigned int i = 0; i < players.size(); i++) {
		ofstream ofs(players[i]->name + ".txt");
		ofs << players[i]->won << " " << players[i]->lost << " " << players[i]->chips;
	}

}
void Game::printOtherPlayers(int pl, int siz) {
	cout << "Other players' cards:" << endl;
	for (int i = (pl + 1) % siz; i != pl; i = (i + 1) % siz) {
		Hand& hand = players[i]->hand;
		cout << players[i]->name << ": ";
		for (int j = 0; j < hand.size(); ++j) {
			if (hand[j].faceDown) {
				cout << "* ";
			}
			else {
				cout << hand[j].toString() << " ";
			}
		}
		cout << endl;
	}
}

void Game::bet() {
	int siz = players.size();
	int offset = (dealer + 1) % siz;

	bool check = true;
	int pl = offset;
	unsigned int unfolded = siz;
	unsigned int highestBet = 0;

	while (true) {
		if (players[pl]->chips == 0) {
			cout << "Player " << players[pl]->name << " has no chips. Skipping." << endl << endl;
			pl = (pl + 1) % siz;
			if (pl == offset) {
				cout << "All players either called or checked. Ending betting phase." << endl << endl;
			}
			continue;
		}
		else if (players[pl]->folded) {
			cout << "Player " << players[pl]->name << " has folded. Skipping." << endl << endl;
			pl = (pl + 1) % siz;
			if (pl == offset) {
				cout << "All players have called or checked. Ending betting phase." << endl << endl;
				break;
			}
			continue;
		}
		else if (check == true) {
			cout << "Player: " << players[pl]->name << ", Hand: " << players[pl]->hand << endl;
			printOtherPlayers(pl, siz);
			cout << "Do you want to check or bet?: ";
			string response; cin >> response;

			while (response != "check" && response != "bet") {
				cout << "Invalid response. Please try again: ";
				cin >> response;
			}
			if (response == "check") {
				pl = (pl + 1) % siz;
				if (pl == offset) {
					cout << endl << "All players checked. Ending betting phase." << endl << endl;
					break;
				}
				cout << endl;
				continue;
			}
			else { // bet
				check = false;
				cout << "How much do you want to bet? Type 1 or 2: ";

				string s;
				cin >> s;
				const char* st = s.c_str();
				unsigned int resp = (unsigned int)strtoul(st, nullptr, 10);

				if (players[pl]->chips < resp) {
					cout << "Not enough chips. ";
					resp = 0; // Just to continue loop
				}
				while (resp != 1 && resp != 2) {
					cout << "Invalid response. Please try again: " << endl;

					cin >> s;
					resp = (unsigned int)strtoul(st, nullptr, 10);

					cin.clear();
					cin.sync();
					if (players[pl]->chips < resp) {
						cout << "Not enough chips. ";
						resp = 0; // Just to continue loop
						continue;
					}
				}
				players[pl]->chips -= resp;
				players[pl]->cumulativeBet = resp;
				pot += resp;
				pl = (pl + 1) % siz;
				highestBet = resp;
				cout << endl;
			}
		}

		else {
			cout << "Player: " << players[pl]->name << ", Hand: " << players[pl]->hand << endl;
			printOtherPlayers(pl, siz);

			cout << "Do you want to call, fold or raise?: ";
			string response; cin >> response;
			if (response == "raise" && players[pl]->chips < highestBet - players[pl]->cumulativeBet + 1) {
				cout << "Not enough chips for that action. ";
				response = ""; // to go into loop
			}
			while (response != "call" && response != "fold" && response != "raise") {
				cout << "Invalid response. Please try again ('call', 'fold' or 'raise'): ";
				cin >> response;
				if ((response == "raise" && players[pl]->chips < highestBet - players[pl]->cumulativeBet + 1)) {
					cout << "Not enough chips for that action. ";
					response = ""; // to go into loop
				}
			}
			if (response == "fold") {
				players[pl]->folded = true;
				--unfolded;
				if (unfolded == 1) {
					cout << endl << "All but one player folded. Ending betting phase." << endl << endl;
					break;
				}
				cout << endl;
			}

			else if (response == "call") {
				unsigned int callingChips = highestBet - players[pl]->cumulativeBet;
				if (callingChips > players[pl]->chips) {
					pot += players[pl]->chips;
					players[pl]->chips = 0;
					cout << "Player is going all in. All remaining chips added to the pot." << endl << endl;
				}
				else {
					players[pl]->chips -= callingChips;
					players[pl]->cumulativeBet += callingChips;
					pot += callingChips;
				}
				cout << endl;
			}

			else { // raise
				cout << "How much does player want to raise? Type 1 or 2: ";

				string s;
				unsigned int resp;
				cin >> s;
				const char* st = s.c_str();
				resp = (unsigned int)strtoul(st, nullptr, 10);
				unsigned int callingChips = highestBet - players[pl]->cumulativeBet;
				if (resp + callingChips > players[pl]->chips) {
					cout << "Not enough chips. ";
					resp = 0; // just to continue loop
				}
				while (resp != 1 && resp != 2) {
					cout << "Invalid response. Please try again: ";
					cin >> s;
					resp = (unsigned int)strtoul(st, nullptr, 10);
					if (resp + callingChips > players[pl]->chips) {
						cout << "Not enough chips. ";
						resp = 0; // just to continue loop
						continue;
					}
				}
				players[pl]->chips -= (resp + callingChips);
				players[pl]->cumulativeBet += (resp + callingChips);
				pot += (resp + callingChips);
				highestBet += resp;
				offset = pl; // shifting end condition to highest better
				cout << endl;
			}

			pl = (pl + 1) % siz;
			if (pl == offset) {
				cout << "All players have called. Ending betting phase." << endl << endl;
				break;
			}
		}
	}
}