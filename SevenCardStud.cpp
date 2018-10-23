/**
 *	SevenCardStud.cpp
 *	Function: The functions declared in SevenCardStud.h
 *			  are defined as per lab specifications.
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */

#include "stdafx.h"
#include "SevenCardStud.h"
#include <sstream>
#include <fstream>
#include <set>
#include <iostream>
//using namespace std;


SevenCardStud::SevenCardStud() {
	generateDeck();
}
int SevenCardStud::before_turn(Player& player) {
	cout << "Name: " << player.name <<
		", Hand: " << player.hand << endl;
	cout << "Do you want to discard any card? (Type position indexed 0 or -1 to not discard): ";


	while (true) {
		string line;

		// used cin.sync() to clear input stream
		cin.sync();

		// used cin to pass by an error
		string q;
		cin >> q;
		getline(cin, line);
		// then appened line with q to get full line
		line = q + " " + line;
		istringstream iss(line);

		int t;
		// used set so that one position is not added multiple times
		set<int> st;
		bool flg = false;

		while (iss >> t) {
			if (t < -1 || t >= players[0]->hand.size()) {
				cout << "Out of range. Try again" << endl;
				flg = true;
				break;
			}

			else {
				st.insert(t);
			}
		}
		if (flg) {
			// scan again
			continue;
		}

		if (iss.rdstate() == ios_base::failbit) {
			cout << "Invalid input. Try again" << endl;
			continue;
		}

		// checks for correct length
		if (st.size() > validHandSize) {
			cout << "Too many cards. Try again" << endl;
			continue;
		}

		if (st.size() > 0) {
			// stops scanning only when there is just 1 -1.
			if (st.size() == 1 && *st.begin() == -1) {
				cout << "No card discarded" << endl << endl;
				break;
			}
			// if the only element is not -1, check for any other -1
			else if (st.size() > 1) {
				bool flag = false;
				for (auto iter = st.begin(); iter != st.end(); iter++) {
					if (*iter == -1) {
						flag = true;
						break;
					}
				}
				// if found, then invalid
				if (flag) {
					cout << "Invalid input. Try again" << endl;
					continue;
				}
			}
		}


		// start erasing from the back of ordered set.
		if (st.size() > 0) {
			for (auto iter = --st.end(); iter != st.begin(); --iter) {
				discardedPile.add_card(player.hand.remove(*iter));
			}
			//remove one more for 0th index
			discardedPile.add_card(player.hand.remove((size_t)*st.begin()));
		}
		cout << endl;
		break;
	}
	return success;

}

int SevenCardStud::turn(Player& player) {
	size_t siz = player.hand.size();
	for (size_t i = 0; i < validHandSize - siz; i++) {
		// try to distribute from deck
		if (deck.size()) {
			player.hand << deck;
		}
		// if not possible distribute from discardedPile
		else if (discardedPile.size()) {
			discardedPile.shuffle();
			player.hand << discardedPile;
		}
		// else return non zero value
		else {
			return not_enough_cards;
		}
	}
	return success;
}

int SevenCardStud::after_turn(Player& player) {
	cout << "Name: " << player.name << ", Hand: " << player.hand << endl;
	return success;
}

void SevenCardStud::first_turn() {
	int siz = players.size();
	int offset = (dealer + 1) % siz;

	for (int i = 0; i < 2; i++) { //faceDown cards
		for (int j = 0; j < siz; j++) {
			if (deck.size() == 0) {
				throw not_enough_cards;
			}
			Hand& hand = players[(j + offset) % siz]->hand << deck;
			Card& card = hand[hand.size() - 1];
			card.faceDown = true;
		}
	}

	for (int j = 0; j < siz; j++) { // faceUp card
		if (deck.size() == 0) {
			throw not_enough_cards;
		}
		Hand& hand = players[(j + offset) % siz]->hand << deck;
		Card& card = hand[hand.size() - 1];
		card.faceDown = false;
	}
	cout << endl << "Round 1: " << endl << endl;
	bet();
}

int SevenCardStud::before_round() {
	deck.shuffle();
	int siz = players.size();
	int offset = (dealer + 1) % siz;

	for (int i = 0; i < siz; ++i) {
		// moving one chip from each player to pot (Ante)
		players[(i + offset) % siz]->chips--;
		pot++;
	}
	try {
		first_turn();
	}
	catch (returnCode ret) {
		if (ret == not_enough_cards) {
			throw not_enough_cards;
		}
	}

	return success;
}

void SevenCardStud::mid_turn() {
	int siz = players.size();
	int offset = (dealer + 1) % siz;

	for (int i = 0; i < siz; ++i) {
		if (deck.size() == 0) {
			throw not_enough_cards;
		}
		Hand& hand = players[(i + offset) % siz]->hand << deck;
		Card& card = hand[hand.size() - 1];
		card.faceDown = false;
	}

	clearCumulativeBets();
	bet();
}

void SevenCardStud::last_turn() {
	int siz = players.size();
	int offset = (dealer + 1) % siz;

	for (int i = 0; i < siz; ++i) {
		if (deck.size() == 0) {
			throw not_enough_cards;
		}
		Hand& hand = players[(i + offset) % siz]->hand << deck;
		Card& card = hand[hand.size() - 1];
		card.faceDown = true;
	}

	clearCumulativeBets();
	bet();
}

int SevenCardStud::round() {
	int siz = players.size();
	int offset = (dealer + 1) % siz;

	for (int i = 0; i < 3; ++i) {
		cout << endl << "Round " << i + 2 << ": " << endl << endl;
		try {
			mid_turn();
		}
		catch (returnCode ret) {
			throw ret;
		}

	}

	cout << endl << "Round 5: " << endl << endl;
	try {
		last_turn();
	}
	catch (returnCode ret) {
		throw ret;
	}
	

	return success;
}

int SevenCardStud::after_round() {
	vector<shared_ptr<Player>> temp(players);
	sort(temp.begin(), temp.end(), poker_rank_player_SCS);
	unsigned int siz = temp.size();


	unsigned int numWinners = 1;
	temp[temp.size() - 1]->won++;
	cout << endl;
	int i = siz - 2;

	// ranks players with poker_rank
	// then clears hand

	// first checks for wins
	for (; i >= 0; --i) {
		if (!(temp[i]->hand == temp[i + 1]->hand)) {
			break;
		}
		if (!temp[i]->folded) {
			temp[i]->won++;
			numWinners++;
		}
		else {
			temp[i]->lost++;
		}
	}
	
	// then for losses
	for (; i >= 0; --i) {
		temp[i]->lost++;
	}
	
	// Add winnings to players
	unsigned int winnings = pot / numWinners;
	for (unsigned int j = 0; j < numWinners; ++j) {
		temp[siz - 1 - j]->chips += winnings;
	}
	if (pot % numWinners != 0) {
		for (unsigned int j = 0; j < pot % numWinners; ++j) {
			temp[siz - 1 - j]->chips++;
		}
	}

	// print players
	for (int j = siz - 1; j >= 0; --j) {
		print_player(temp[j]);
		temp[j]->hand.clear();
	}
	
	// then regenerate deck and shuffle
	generateDeck();
	deck.shuffle();

	// clear all cumulativeBets and fold status for remaining players.
	clearCumulativeBets();
	clearFoldStatus();

	pot = 0;


	cout << endl << "Does any player want to leave Game?" << endl
		<< "If yes, type name of the player. Else type 'no': ";
	string name;
	cin >> name;

	while (name != "no") {
		if (find_player(name) == 0) {
			cout << "Invalid name. Try again: ";
		}
		else {
			// if player leaves, save info
			ofstream ofs(name + ".txt");
			shared_ptr<Player> play = find_player(name);
			ofs << play->won << " " << play->lost << " " << play->chips;
			remove(play);
			cout << endl << "Player " << name << " removed. Anyone else? Else type 'no': ";
		}
		cin >> name;
	}

	// Check all players with 0 chips
	for (int j = siz - 1; i >= 0; --j) {
		if (players[j]->chips == 0) {
			cout << "Player " << players[j]->name << " has no remaining chips." <<
				endl << "Enter 0 to exit, 1 to add 20 chips: ";
			int resp; cin >> resp;
			while (resp != 0 && resp != 1) {
				cout << "Invalid response. Please try again: ";
				cin >> resp;
			}

			if (resp == 0) {
				ofstream ofs(players[j]->name + ".txt");
				ofs << players[j]->won << " " << players[j]->lost << " " << players[j]->chips;
				remove(players[j]);
				cout << "Player removed." << endl;
			}
			else {
				players[i]->chips = 20;
				cout << "Chips added." << endl;
			}
		}
	}



	// if player joins, check for existing info
	cout << endl << "Does any player want to join the Game?" << endl
		<< "If yes, type the player name. Else type 'no': ";
	cin >> name;
	while (name != "no") {
		try {
			add_player(name);
			cout << endl << "Player " << name << " added. Anyone else? If not, type 'no': ";
		}
		catch (returnCode ret) {
			if (ret == already_playing) {
				cout << "This player already exists. Anyone else? If not, type 'no': " << endl;
			}
			else if (ret == player_not_added) {
				cout << "Player not added. Anyone else? If not, type 'no': " << endl;
			}
		}

		cin >> name;
	}
	cout << endl;

	// making sure dealer is not modded with 0
	if (players.size() != 0) {
		dealer = (dealer + 1) % players.size();
	}
	else {
		dealer = 0;
	}

	cin.sync();
	return success;
}