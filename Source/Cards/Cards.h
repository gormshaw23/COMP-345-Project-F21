
#pragma once 
#include "Common/CommonTypes.h"
#include <iostream>
#include <vector>
#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
using namespace std; 

enum class EOrderType;

class Card {

public: 
	enum cardType {
		Bomb,
		Reinforcement,
		Blockade,
		Airlift,
		Diplomacy
	};

	/* Accesors and mutators */

	cardType getNewCardType(); 
	void setNewCardType(cardType); 

	/* Default Constructor */

	Card();

	/* Destructor */

	~Card();

	/* Param constructor */

	Card(cardType); 

	/* Copy constructor */

	Card(Card& cType); 

	/* Assignment operator overloading constructor */

	Card operator=(Card& cType);

	/* Insertion operator overloading function*/

	friend ostream& operator<<(ostream& o, Card& cType);

private:

	cardType* newCardType;
	
	
};


class Deck {

public: 

	

	/* Accesors and mutators */

	int getCurrentCard();
	void setCurrentCard(int);

	int getDeckSize();
	void setDeckSize(int); 

	/* Default Constructor */

	Deck();

	/* Default Constructor */

	Deck(int);

	/* Shuffle method */

	void ShuffleDeck();  

	/* Destructor */

	~Deck();

	/* Copy constructor */

	Deck(const Deck& aDeck);

	/* Assignment operator overloading constructor */

	Deck operator=(Deck& aDeck);

	/* Insertion operator overloading function*/

	friend ostream& operator<<(ostream& o, Deck& aDeck);

	/* Special method draw() */

	Card* drawCard_Deck();
	
	/* Special method insert() */

	void insertCard_Deck(Card*); 

	/* Show the deck method */

	void showDeck(); 

	
	
private:
	vector<Card*>* newDeck;
	int* currentCard; /* not sure if i need this later */
	int* deckSize;
	
	 
};

/* Hand Class */

class Hand {
public: 
	/* Accesors and mutators */
	int getHandSize();
	void setHandSize(int);

	/* Default constructor */

	Hand(); 

	/* Destructor */

	~Hand(); 

	/* Param constructor */

	Hand(int); 

	/* Copy constructor */

	Hand(const Hand& aHand);

	/* Assignment operator overloading constructor */

	Hand operator=(Hand& aHand);

	/* Show Hand method */

	void showHand();

	/* Insert Cards inside the Hand */

	void insertCard_Hand(Card*);

	/* Draw cards into the deck*/

	Card* drawCard_Hand();

	/* Special play() method */

	EOrderType play(Card*);

private: 
	vector<Card*>* newHand; 
	int* handSize; 
};

/* Dummy vector to put in Orders */

	static vector<Card*>* newOrder;
	static int* orderSize;