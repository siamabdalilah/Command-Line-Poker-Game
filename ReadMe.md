## Command Line Poker game
**Author:** Siam Abd Al-Ilah <[siam.abdal-ilah@wustl.edu](mailto:siam.abdal-ilah@wustl.edu)>

The betting has been simplified such that each new players start at 20 chips and can bet only 1-2 chips in each round. Going "all in" is not implemented. The variations of poker that are implemented are FiveCardDraw SevenCardStud and TexasHoldEm. The main function takes the type of the game as first argument and then names of at least two players. Players can leave or join back after each round. The chip information for each player is stored in txt files.  

This was written on Visual Studio, so all the source files have "stdafx.h" included.


<!-- CSE332S Lab 4
Name: Siam Abd Al-Ilah
WUSTL key: siam.abdal-ilah


Summary of Files:

Lab4.cpp:
	Contains the main function and usage function. Takes in the type of game and
	names of starting player and calls Game::start_game() to start the game. Then
	adds the players to the game and repeatedly calls the round methods until there
	two or more players. Once that is over, calls stop_game() to stop game. Then it
	gives the user the option to coninue playing a different game and if properly
	formatted input is received it goes on to start a new game or else returns.

Card.h:
	Contains declarations for the struct Card and relevant methods and also some
	enums. Encapsulates behavior of cards. Modified to add a bool field to indicate
	whether the card is faceDown.

Card.cpp:
	Contains definitions of the functions declared in Card.h and is largely unchanged
	from previous labs.

Deck.h:
	Contains declaration for Deck class. Mostly unchanged from previous labs except
	the newly added add_card() function and a default constructor.

Deck.cpp:
	Contains definitons of functions declared in Deck.h. Some methods in this class
	have not been used in this lab but I kept them as they were.

Hand.h:
	Contains decalaration of Hand class and relavant methods. Modified to add two
	new key functions bestHand() and comb(). The purpose of comb is to take in a hand
	of length 7 and return all possible 5 card hands with those 7 cards. bestHand() calls
	comb to get that set of hands and sorts them with poker_rank. Then the highest hand is
	used to determine the hand of players in SevenCardStud and TexasHoldEm.

Hand.cpp:
	Functions declared in Hand.h are defined here. The defintion for Hand& operator(Hand&,
	Deck&) which was declared in Deck.h is also in this file.

Player.h:
	Contains declaration for struct Player and a bool function to compare two player
	objects. The struct contains the name, hand and number of wins and losses of the
	player. The constructor takes in name of the player.

Player.cpp
	The comparison functions checks if either points to null and if not calls the
	poker_rank functions on the hands.
	The constructor tries to open a file with the name of the player and if succeed,
	copies number of wins and losses from the file. If no file exists, it will
	initialize wins and losses to 0.

Game.h
	Declared a Game abstract base class. Contains static pointer (all pointers are shared_ptr)
	game, vector of pointers to player, and a deck. Contains virtual methods for the actual
	game to leave room for different types of implementations of the game. The class is modified
	to include a betting component that allows players to either bet, check, call, fold or raise
	the stakes in specific phases of the game.

Game.cpp:
	Contains a base class constructor, and implementations for all non-virtual methods.

FiveCardDraw.h:
	This represents a particular implementation of the Game base class. Declared the class
	called FiveCardDraw which has public inheritance from the Game bass class. The implementation
	was for a standard five card poker game. The class contained a discardPile Deck object and a
	size_t member variable to identify the dealer. Modified to add a betting phase between each turn.

FiveCardDraw.cpp:
	Contains defintions for the methods declared in FiveCardDraw.h and all virtual methods from
	the base Game class. The functions for before/after/round() and before before/after/turn()
	were implemented according to lab specifications. FiveCardDraw overrides the inherited bet()
	method so that other players' cards are not showed during betting phase.

SevenCardStud.h:
	Contains declaration for SevenCardStud class. Some parts were copied over. Some methods like
	before_turn(), after_turn() are never used but still there. I understand that this is bad
	practice.

SevenCardStud.cpp:
	Contains definitions for functions declared in SevenCardStud. There are new "turn" methods that
	creates the new 5 turn behaviour instead of 2 turns like in FiveCardDraw. Some cards are dealt
	faceUp and some are dealt faceDown. There is a betting phase between each turn. Best possible 5
	card hand is selected when deciding the winner.

TexasHoldEm.h:
	Contains declaration for TexasHoldEm class. Mostly similar to SevenCardStud.h apart from minor
	modifications with the turns.

TexasHoldEm.cpp:
	Contains definitions for functions declared in SevenCardStud. There are new "turn" methods that
	creates the new 4 turn behaviour specific to this game. Some cards are dealt faceUp and shared
	by all players. Others are faceDown and visible only to players who have them.


Encountered Errors:
	I did not get any hard to fix compilation error in this lab. One of the more difficult parts
	to implement was sanitizing the values for scanning unsigned ints. Some Googling involved.
	Other than that, there were logic error that were somewhat hard to spot.

Testing:
	This lab was hard to test thoroughly because there were so many possible sequences of inputs
	so I did the best I could. I tried all three games with different number of players and checked
	to see if the number of chips and hands were reasonable. I also tried shifting games at the
	end of one game with both properly and improperly formatted input and fixed all the errors
	I encountered.

	 -->
