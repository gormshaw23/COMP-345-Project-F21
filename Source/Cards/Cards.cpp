#include "Cards.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <random>
using namespace std; 

/*
* 
 Cards class implementation

*/

Card::cardType Card::getNewCardType() {

	return *newCardType; 

}

void Card::setNewCardType(Card::cardType cType) {

	*newCardType = cType; 

}

Card::Card(Card::cardType cType) {

	newCardType = new cardType(cType); 

}

Card::Card() {

	newCardType = new cardType(Bomb);
}

Card::~Card() {

	delete newCardType;
	newCardType = nullptr;
}

Card::Card(Card& cType) {

	newCardType = new cardType();
	*newCardType = *(cType.newCardType); 

}

// might cause problems later? 

Card Card::operator=(Card& cType) {

	Card temp; 
	temp.newCardType = cType.newCardType; 
	return temp; 

}


/* Insertion operation overloader */

ostream& operator<<(ostream& os, Card& cType) {

		Card::cardType card_type = cType.getNewCardType();

	if (card_type == Card::Bomb) { os << "Bomb" << endl; }

	else if (card_type == Card::Reinforcement) { os << "Reinforcement" << endl; }

	else if (card_type == Card::Blockade) { os << "Blockade" << endl; }

	else if (card_type == Card::Airlift) { os << "Airlift" << endl; }

	else if (card_type == Card::Diplomacy) { os << "Diplomacy" << endl; }

	return os;

}



/*
*
 Deck class implementation

*/

/*
Getters and setters 

*/
int Deck::getCurrentCard()
{
	return *currentCard;
}

void Deck::setDeckSize(int someCards)
{
	*deckSize = someCards; 
}
int Deck::getDeckSize()
{
	return *deckSize;
}

void Deck::setCurrentCard(int cCard)
{
	*currentCard = cCard;
}



Deck::Deck()
{

}

// fix later, similar to Card 

Deck::Deck(const Deck& aDeck)
{
	newDeck = new vector<Card*>(); 
	for (auto i : *(aDeck.newDeck))
	{
		newDeck->push_back(new Card(*i)); 
	}

	deckSize = new int();
	*deckSize = *(aDeck.deckSize);
}

Deck::Deck(int someCards)
{
	
	/* Deck must be multiple of 5 or not empty due to the nature of the card types */
	while (someCards == 0 || someCards % 5 != 0 ) {

		cout << "Please enter a multiple of 5 to create a deck. " << endl;
		cin >> someCards; 

	}

	newDeck = new vector<Card*>();
	deckSize = new int(someCards);


	cout << "Debug: Size of deck is: " << getDeckSize() << endl; 

	/* Populating the deck with Card objects */

	cout << "Debug: Check if deck can receive card objects " << endl; 

	for (int i = 0; i < getDeckSize(); i++) {

		if (i % 5 == 0) {

			newDeck->push_back(new Card(Card::Bomb));
			
		}
		else if (i % 5 == 1) {

			newDeck->push_back(new Card(Card::Reinforcement));

		}
		else if (i % 5 == 2) {

			newDeck->push_back(new Card(Card::Blockade)); 

		}
		else if (i % 5 == 3) {

			newDeck->push_back(new Card(Card::Airlift));
			
		}
		else {
			
			newDeck->push_back(new Card(Card::Diplomacy));
			
		}
		 
		cout << *newDeck->at(i) << endl; 
	}
		
	this->ShuffleDeck(); 

	cout << "Debug: Check if deck can be shuffled " << endl;

	this->showDeck(); 
}

/* Shuffle method */

void Deck::ShuffleDeck() {
	auto randomClock = std::chrono::high_resolution_clock::now();
	auto seed = randomClock.time_since_epoch().count();
	auto randomizer = std::default_random_engine(seed);
	std::shuffle(newDeck->begin(), newDeck->end(), randomizer);
}
/* Draw method */

Card* Deck::drawCard_Deck() {

	Card* topOfDeck = newDeck->back();
	newDeck->pop_back(); 
	this->ShuffleDeck(); 
	(*deckSize)--; 
	return topOfDeck; 

}

/* Insert method */

void Deck::insertCard_Deck(Card* someCards)
{
	newDeck->push_back(someCards); 
	(*deckSize)++; 
}

/* Show all contents in the deck */

void Deck::showDeck() {

	for (int i = 0; i < getDeckSize(); i++) {

		cout << *newDeck->at(i) << endl;
	}
}

Deck::~Deck()
{

	for (auto i : *newDeck) {
		delete i;
	}
	newDeck->clear();

	delete newDeck;
	newDeck = nullptr;

	delete deckSize; 
	deckSize = nullptr; 

}


Deck Deck::operator=(Deck& aDeck)
{
	Deck temp;
	temp.newDeck = aDeck.newDeck;
	temp.deckSize = aDeck.deckSize;
	return temp;

}

/* Insertion operation overloader */

//ostream& operator<<(ostream& o, Deck& aDeck)
//{
//	// TODO: insert return statement here
//}



int Hand::getHandSize()
{
	return *handSize; 

}

void Hand::setHandSize(int someCards)
{
	*handSize = someCards; 
}

Hand::Hand()
{
}

Hand::Hand(int someCards)
{

	newHand = new vector<Card*>();
	handSize = new int();
 
}

Hand::~Hand()
{

	for (auto i : *newHand) {
		delete i;
	}
	newHand->clear();

	delete newHand;
	newHand = nullptr;

	delete handSize;
	handSize = nullptr;
}

Hand::Hand(const Hand& aHand)
{
	newHand = new vector<Card*>();
	for (auto i : *(aHand.newHand))
	{
		newHand->push_back(new Card(*i));
	}

	handSize = new int();
	*handSize = *(aHand.handSize);
}


Hand Hand::operator=(Hand& aHand)
{
	Hand temp;
	temp.newHand = aHand.newHand;
	temp.handSize = aHand.handSize;
	return temp;
}

void Hand::showHand() {

	for (int i = 0; i < getHandSize(); i++) {

		cout << *newHand->at(i) << endl;
	}
}

void Hand::insertCard_Hand(Card* someCards)
{
	newHand->push_back(someCards);
	(*handSize)++;
}

Card* Hand::drawCard_Hand()
{
	Card* topOfHand = newHand->back();
	newHand->pop_back();
	(*handSize)--;
	return topOfHand;
}

void Hand::play(Card* someCards) {

	newOrder = new vector<Card*>();
	orderSize = new int();

	newOrder->push_back(someCards);
	(*orderSize)++; 
 

	for (int i = 0; i < *orderSize; i++) {

		cout << *newOrder->at(i) << endl;
	}


	




	/* Destroying the Order list pointers*/

	delete newOrder;
	newOrder = nullptr;

	delete orderSize;
	orderSize = nullptr;
}







