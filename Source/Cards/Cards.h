
#pragma once 
#include "Common/CommonTypes.h"
#include <iostream>
#include <vector>
#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#define REINFORCEMENT_SIZE 5

enum class EOrderType;
enum class ECardTypes;

class Card {

public: 

	/* Accesors and mutators */

	ECardTypes getCardType() const; 
	void setCardType(ECardTypes inCardType); 

	/* ctors */
	Card(ECardTypes inCardType); 
	Card(Card& cType); 
	~Card();

	/* ops */
	Card operator=(Card& cType);

	/* Insertion operator */
	friend std::ostream& operator<<(std::ostream& o, Card& cType);

protected:
	// to prevent instantiation of default cards
	Card();
private:
	ECardTypes cardType;
};


class Deck {
public: 
	/* Accesors and mutators */

	int getCurrentCard();
	void setCurrentCard(int);

	int getDeckSize() const;
	void setDeckSize(int inNewDeckSize); 

	/* ctors */
	Deck();
	Deck(int);
	Deck(const Deck& aDeck);
	~Deck();

	/* Shuffle method */
	void ShuffleDeck();

	/* operators */
	Deck operator=(Deck& aDeck);

	/* Insertion operator overloading function*/
	friend std::ostream& operator<<(std::ostream& o, Deck& aDeck);

	/* Special method draw() */
	Card* drawCard_Deck();
	
	/* Special method insert() */
	void insertCard_Deck(Card* inNewCard); 

	/* Show the deck method */
	void showDeck(); 

private:
	std::vector<Card*> deck;
	int currentCard; /* not sure if i need this later */
	int deckSize;	 
};

/* Hand Class */
class Hand {
public: 
	/* ctors */
	Hand(); 
	Hand(int inInitialHandSize);
	Hand(const Hand& aHand);
	~Hand(); 

	/* operators */
	Hand operator=(Hand& aHand);

	/* Accesors and mutators */
	int getHandSize() const;
	void setHandSize(int inNewHandSize);

	/* Show Hand method */
	void showHand();

	/* Insert Cards inside the Hand */
	void insertCard_Hand(Card* inNewCard);

	/* Draw cards into the deck */
	Card* drawCard_Hand();

	/* Special play() method */
	EOrderType play(Card*);

	std::vector<Card*>& getHand();
	const std::vector<Card*>& getHand() const;

private: 
	std::vector<Card*> hand; 
	int handSize; 
};

/* Dummy vector to put in Orders */

static std::vector<Card*>* newOrder;
static int* orderSize;