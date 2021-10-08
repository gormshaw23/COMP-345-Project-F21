#include "Player/Player.h"
#include "Map/map.h"
#include "Cards/Cards.h"

#include <iostream>

int main()
{
    std::cout << "This is the player driver class!\n";
    //std::cout << "Creating sample players...\n";
    std::cout << "***********************************\n";
    std::cout << "Creating player 1...\n";
    Player* player1 = new Player("Bob");
    // I don't care about what kind of territories as we're just demonstrating
    // the number of territories the player owns.
    std::vector<Territory*> provs = player1->getTerritoriesOwned();

    std::cout << "Creating a sample territory...\n";
    std::vector<Territory*> someBorder;
    // create some territories to use
    Territory* prov1 = new Territory
    (
        0, 
        0, 
        player1,
        0,
        0, 
        0, 
        "city 17", 
        someBorder
    );

    // doesn't atter currently that they're duplicates
    player1->getTerritoriesOwned().push_back(prov1);
    player1->getTerritoriesOwned().push_back(prov1);
    player1->getTerritoriesOwned().push_back(prov1);
    std::cout << *player1 << std::endl;

    std::cout << "Player 1's territories to ATTACK: " 
              << player1->toAttack().size() << std::endl;
    std::cout << "***********************************\n";
    std::cout << std::endl;

    std::cout << "Player 1's territories to DEFEND: "
        << player1->toDefend().size() << std::endl;
    std::cout << "***********************************\n";
    std::cout << std::endl;

    std::cout << "Creating player 2...\n";
    std::cout << "Player 2 should have no territories\n";
    Player* player2 = new Player("Eve");

    std::cout << *player2 << std::endl;

    std::cout << "Creating some cards\n";
    std::cout << std::endl;
    Card* card1 = new Card();

    std::cout << "Adding cards to Player 2's hand...\n";
    player2->getCurrentHand()->insertCard_Hand(card1);
    player2->getCurrentHand()->insertCard_Hand(card1);

    std::cout << "Player 2's Hand size:\n"
              << player2->getCurrentHand()->getHandSize() 
              << std::endl;
    std::cout << "***********************************\n";
    std::cout << std::endl;

    std::cout << "Creating player 3 as copy of player 1...\n";
    // player 2 is a copy of player 1
    Player* player3 = new Player(*player1);
    std::cout << *player3 << std::endl;

    std::cout << "Creating player 4...\n";
    Player* player4 = new Player("Alice");
    std::cout << "Making player 4 the same as player 4...\n";
    //*player3 = *player1;
    //std::cout << *player3 << std::endl;
}