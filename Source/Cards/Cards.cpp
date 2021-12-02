#include "Cards.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <random>

#include "Order/Orders.h"
#include "Common/CommonTypes.h"

using namespace std; 

/*
* 
 Cards class implementation

*/

ECardTypes Card::getCardType() const {

	return cardType; 

}

void Card::setCardType(ECardTypes cType) {
	cardType = cType; 
}

Card::Card(ECardTypes cType) {
	cardType = cType;
}

Card::Card() 
{
}

Card::~Card() {
}

Card::Card(Card& cType) {
	cardType = cType.cardType;
}

Card Card::operator=(Card& cType) {
	Card temp; 
	temp.cardType = cType.cardType; 
	return temp; 
}


/* Insertion operation overloader */
ostream& operator<<(ostream& os, Card& cType) {

	ECardTypes cardType = cType.getCardType();

	std::string msg = "";
	switch (cardType)
	{
	case ECardTypes::Bomb:
		msg = "Bomb";
		break;
	case ECardTypes::Reinforcement:
		msg = "Reinforcement";
		break;
	case ECardTypes::Blockade:
		msg = "Blockade";
		break;
	case ECardTypes::Airlift:
		msg = "Airlift";
		break;
	case ECardTypes::Diplomacy:
		msg = "Diplomacy";
		break;
	default:
		break;
	}

	os << msg << std::endl;

	return os;
}

/*
	Deck class implementation
*/

/*
	Getters and setters 
*/
int Deck::getCurrentCard()
{
	return currentCard;
}

void Deck::setDeckSize(int someCards)
{
	deckSize = someCards; 
}
int Deck::getDeckSize() const
{
	return deckSize;
}

void Deck::setCurrentCard(int cCard)
{
	currentCard = cCard;
}

Deck::Deck()
{
	deckSize = 0;
	currentCard = 0;
}

// fix later, similar to Card 

Deck::Deck(const Deck& aDeck)
{
	for (auto& card : aDeck.deck)
	{
		deck.push_back(new Card(*card)); 
	}

	deckSize = deck.size();
	currentCard = 0;
}

Deck::Deck(int someCards)
{	
	/* Deck must be multiple of 5 or not empty due to the nature of the card types */
	while (someCards == 0 || someCards % 5 != 0 ) {
		cout << "Please enter a multiple of 5 to create a deck. " << endl;
		cin >> someCards;
	}

	deckSize = someCards;

	cout << "Debug: Size of deck is: " << getDeckSize() << endl; 

	/* Populating the deck with Card objects */

	cout << "Debug: Check if deck can receive card objects " << endl; 

	for (int i = 0; i < getDeckSize(); i++) {
		int j = i % 5;

		switch (j)
		{
		case 0:
			deck.push_back(new Card(ECardTypes::Bomb));
			break;
		case 1:
			deck.push_back(new Card(ECardTypes::Reinforcement));
			break;
		case 2:
			deck.push_back(new Card(ECardTypes::Blockade));
			break;
		case 3:
			deck.push_back(new Card(ECardTypes::Airlift));
			break;
		default:
			deck.push_back(new Card(ECardTypes::Diplomacy));
			break;
		}

		if (deck[i] != nullptr)
		{
			cout << deck[i] << endl;
		} 
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
	std::shuffle(deck.begin(), deck.end(), randomizer);
}

/* Draw method */
Card* Deck::drawCard_Deck() {
	Card* topOfDeck = deck.back();
	deck.pop_back();
	this->ShuffleDeck(); 
	deckSize--;
	return topOfDeck; 
}

/* Insert method */
void Deck::insertCard_Deck(Card* someCards)
{
	deck.push_back(someCards); 
	deckSize++;
}

/* Show all contents in the deck */
void Deck::showDeck() {
	for (auto& card : deck)
	{
		if (card != nullptr)
		{
			cout << card << endl;
		}
	}
}

Deck::~Deck()
{
	for (auto& i : deck) {
		delete i;
	}
	deck.clear();
	deckSize = 0;
}

Deck Deck::operator=(Deck& aDeck)
{
	Deck temp;
	for (auto& card : aDeck.deck)
	{
		temp.deck.push_back(new Card(*card));
	}
	temp.deckSize = temp.deck.size();
	return temp;
}

/* Insertion operation overloader */
ostream& operator<<(ostream& os, Deck& aDeck)
{
	// TODO: insert return statement here
	return os;
}

int Hand::getHandSize() const
{
	return handSize;
}

void Hand::setHandSize(int someCards)
{
	handSize = someCards; 
}

Hand::Hand()
{
	handSize = 0;
}

Hand::Hand(int someCards)
{
	handSize = someCards;
}

Hand::~Hand()
{
	for (auto& card : hand) {
		delete card;
	}

	hand.clear();
	hand.shrink_to_fit();
	handSize = 0;
}

Hand::Hand(const Hand& aHand)
{
	for (auto& card : aHand.hand)
	{
		hand.push_back(new Card(*card));
	}

	handSize = hand.size();
}

Hand Hand::operator=(Hand& aHand)
{
	Hand temp;
	for (auto& card : aHand.hand)
	{
		temp.hand.push_back(new Card(*card));
	}
	temp.handSize = temp.hand.size();
	return temp;
}

void Hand::showHand() {
	for (auto& card : hand)
	{
		if (card != nullptr)
		{
			cout << *card << endl;
		}
	}
}

void Hand::insertCard_Hand(Card* someCards)
{
	hand.push_back(someCards);
	handSize++;
}

Card* Hand::drawCard_Hand()
{
	Card* topOfHand = hand.back();
	hand.pop_back();
	handSize--;
	return topOfHand;
}

/*
* Modified play function that returns an order type,
* for an order to be issued
*/
EOrderType Hand::play(Card* someCards) {
	switch (someCards->getCardType()) {
	case ECardTypes::Bomb:
		return EOrderType::Bomb;
		break;
	case ECardTypes::Blockade:
		return EOrderType::Blockade;
		break;
	case ECardTypes::Airlift:
		return EOrderType::Airlift;
		break;
	case ECardTypes::Diplomacy:
		return EOrderType::Negotiate;
		break;
	default:
		break;
	}
}


std::vector<Card*>& Hand::getHand()
{
	return this->hand;
}

const std::vector<Card*>& Hand::getHand() const
{
	return this->hand;
}




