#define _DEBUG
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

#include "Cards/Cards.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std; 

int main() {

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	int userInputInt;
	Card* myCard;
	 

	cout << "Welcome to the CardsDriver. We are testing out the functionality of the class. " << endl; 
	cout << "Please enter a multiple of 5 to create a deck. " << endl; 

	cin >> userInputInt;
	Deck myDeck = Deck(userInputInt);

	cout << "Deck created. \nPlease enter the number of cards you would like to draw in the hand. " << endl; 

	cin >> userInputInt; 
	while (userInputInt < 1 || userInputInt > myDeck.getDeckSize()) {
				cout << " Please enter a valid number from 1 to " << myDeck.getDeckSize() << ": ";
				cin >> userInputInt;
			}

	Hand myHand = Hand(userInputInt);

	for (int i = 0; i < userInputInt; i++) {
				myCard = myDeck.drawCard_Deck();

				myHand.insertCard_Hand(myCard);
			}

	cout << "The hand now has " << myHand.getHandSize() << " cards. Here it is: " << endl;
	myHand.showHand();

	cout << "The deck now has " << myDeck.getDeckSize() << " cards. Here it is: " << endl;
	myDeck.showDeck(); 

	cout << "The hand is ready to send out orders. Play cards?\n(1 for yes: 0 for no)" << endl; 
	cin >> userInputInt; 

	while (userInputInt != 1) {
		cout << "The hand is ready to send out orders. Play cards?\n(1 for yes: 0 for no)" << endl; 
		cin >> userInputInt; 

	}

	/* fixed variable to loop the current hand */

	int handSize = myHand.getHandSize(); 

	cout << "Here is the order list: " << endl;

	for (int i = 0; i < handSize; i++) {

		myCard = myHand.drawCard_Hand();
		myDeck.insertCard_Deck(myCard);
		myHand.play(myCard); 


	}

	cout << "The hand now has " << myHand.getHandSize() << " cards. Here it is: " << endl;
	myHand.showHand();

	cout << "The deck now has " << myDeck.getDeckSize() << " cards. Here it is: " << endl;
	myDeck.showDeck();











	

} 