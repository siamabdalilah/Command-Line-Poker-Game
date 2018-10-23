/**
 *	FiveCardDraw.cpp
 *	Function: The functions declared in FiveCardDraw.h
 *			  are defined as per lab specifications.
 *	Author	: Siam Abd Al-Ilah <siam.abdal-ilah@wustl.edu>
 */


#include "stdafx.h"
#include "FiveCardDraw.h"
#include <sstream>
#include <fstream>
#include <set>
#include <iterator>
using namespace std;


FiveCardDraw::FiveCardDraw() {
	generateDeck();
}
int FiveCardDraw::before_turn(Player& player) {
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
			if (t < -1 || t >= validHandSize) {
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

		if (st.size() > 0){ 
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

int FiveCardDraw::turn(Player& player) {
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

int FiveCardDraw::after_turn(Player& player) {
	cout << "Name: " << player.name << ", Hand: " << player.hand << endl;
	return success;
}

int FiveCardDraw::before_round() {
	deck.shuffle();
	int siz = players.size();
	int offset = (dealer + 1) % siz;

	for (int i = 0; i < validHandSize; i++) {
		for (int j = 0; j < siz; j++) {
			players[(j + offset) % siz]->hand << deck;
		}
	}

	for (int i = 0; i < siz; ++i) {
		// moving one chip from each player to pot
		players[(i + offset) % siz]->chips--;
		pot++;
	}
	
	bet();

	for (int i = 0; i < siz; i++) {
		if (players[(i + offset) % siz]->folded) {
			cout << "Player " << players[(i + offset) % siz]->name << " has folded. Skipping." << endl << endl;
		}
		else {
			before_turn(*players[(i + offset) % siz]);
		}
	}

	return success;
}

int FiveCardDraw::round() {
	int siz = players.size();
	int offset = (dealer + 1) % siz;

	for (int i = 0; i < siz; i++) {
		int ret = turn(*players[(i + offset) % siz]);
		if (ret) {
			return ret;
		}
		after_turn(*players[(i + offset) % siz]);
	}
	cout << endl;

	clearCumulativeBets();
	bet();

	return success;
}
int FiveCardDraw::after_round() {
	vector<shared_ptr<Player>> temp(players);
	sort(temp.begin(), temp.end(), poker_rank_player);
	unsigned int siz = temp.size();

	unsigned int numWinners = 1;
	temp[siz - 1]->won++;
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
	for (unsigned int j = 0; j < numWinners; j++) {
		temp[siz - 1 - j]->chips += winnings;
	}
	if (pot % numWinners != 0) {
		for (unsigned int j = 0; j < pot % numWinners; j++) {
			temp[siz - 1 - j]->chips++;
		}
	}
	
	// print players
	for (int j = siz - 1; j >= 0; --j) {
		print_player(temp[j]);
		temp[j]->hand.clear();
	}

	// also clear discardedPile
	discardedPile.getDeck().clear();

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

void FiveCardDraw::bet() {
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

