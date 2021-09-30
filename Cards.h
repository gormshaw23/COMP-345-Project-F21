/*
Part 4: Cards deck/hand. 
This part implements a group of C++ classes that includes a deck and hand of Warzone cards. 
*/
#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ ) // test for memory leaks 
#endif

#ifndef CARDS_H 
#define CARDS_H 

#include <iostream> 
#include <vector>

class Cards {


// Member variables of class Cards 

    private: 

        enum cardType {
             Bomb, Reinforcement, Blockade, Airlift, Diplomacy
        };

   public: 

   // Overloaded constructor: A different way of calling the function if needed
    Cards(); 

    // Copy constructor
    Cards(const Cards& c); 

    // Overloaded assignment operator 
    Cards& operator=(const Cards& c);  

    // Stream insertion operator: to perform input and output for user-defined types like an object. 
    friend std::ostream& operator<<(std::ostream& os, const Cards& c); 

    // Getter: Will return the cardType of the Card
    cardType getCardType() const; 

    //Setter: Will set the cardType of the Card 
    void setCardType(cardType); 

    // Destructor 
    ~Cards(); 

    // Special Functions 
    // play() method 
    cardType Play(); 
  

};

class Deck {

// Member variables of class Deck 
private: 
    std::vector<Cards*>* deck; // A deck usually contains a certain number of cards. Hence, the creation of a vector for Card objects. 
    int *numOfCards; 
    int *numOfBombCards; 
    int *numOfReinforcementCards;
    int *numOfBlockadeCards;
    int *numOfAirliftCards;
    int *numOfDiplomacyCards; 

public: 

// Overloaded constructor: A different way of calling the function if needed
    Deck();

// Copy constructor
Deck(const Deck& d);

// Overloaded assignment operator 
Deck& operator=(const Deck& c);

// Stream insertion operator: to perform input and output for user-defined types like an object. 
friend std::ostream& operator<<(std::ostream& os, const Deck& d); 

// Getters 
 int getNumOfCards() const;
 int getNumOfBombCards() const;
 int numOfReinforcementCards() const;
 int numOfBlockadeCards() const;
 int numOfAirliftCards() const;
 int numOfDiplomacyCards() const;  

 // Setters 
 void setNumOfCards(int);
 void setNumOfBombCards(int);
 void setNumOfReinforcementCards(int);
 void setNumOfBlockadeCards(int);
 void setNumOfAirliftCards(int);
 void setNumOfDiplomacyCards(int);

 // Destructor 

 ~Deck(); 

 // Special methods 
 Cards* draw(); 


}; 


class Hand{

// Member variables 

    private: 
    std::vector<Cards*>* hand;
    int numOfCards; 

    public: 

    // Overloaded constructor: A different way of calling the function if needed
    Hand();

     // Copy constructor
    Hand(const Hand& h); 

    // Overloaded assignment operator 
    Hand& operator=(const Hand& h); 

    // Stream insertion operator: to perform input and output for user-defined types like an object. 
    friend std::ostream& operator<<(std::ostream& os, const Hand& h); 

    //Getters 
    int getNumOfCards() const;

    // Setters
    void setNumOfCards(int); 




}; 

#endif // CARDS_H