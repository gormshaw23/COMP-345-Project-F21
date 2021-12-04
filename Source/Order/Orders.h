/**
 * Class header for the Order class and its subclasses
 * Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate.
 * The class header also has the OrdersList class.
 * 
 * @author Mark Nasol
 */
#pragma once

// chance for attacker to kill 1 defending unit
#define ATTACKER_CHANCE 60
// chance for defender to kill 1 attacking unit
#define DEFENDER_CHANCE 70

#include <string>
#include <ostream>
#include <list>

#include"GameLog/LoggingObserver.h"

class Territory;
class Player;
class GameEngine;

enum class EOrderType
{
    Deploy,
    Advance,
    Bomb,
    Blockade,
    Airlift,
    Negotiate
};

/**
 * The function and member declarations of the Order class 
 */
class Order : virtual public Subject, virtual public ILoggable
{
public:
    Order();                                                       //Constructor
    ~Order();                                                      //Destructor
    Order(const Order &od);                                        //Copy constructor
    Order &operator=(const Order &od);                             //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, Order &od); //Stream insertion operator

    Order(EOrderType typeValue, std::string descValue);
    virtual bool validate() =0;
    virtual void execute() =0;
    const EOrderType getType(); //Gets OrderType enum
    const std::string getTypeName(); //Gets name of the order type
    const std::string getDescription();
    void setDescrption(std::string inOrderEffect);
    virtual std::string stringToLog();  //use to send the effect of the order as a string 

    
private:
    EOrderType oType;        //The order type
    std::string description; //The order description
};

/**
 * The function and member declarations of the Deploy class 
 */

// Pizza , add (virtual public Subject , virtual public ILoggable,) in all the other type of Order 
class Deploy :  virtual public Subject , virtual public ILoggable, public Order
{
public:
    Deploy();                                                        //Constructor
    Deploy(Player* inOwner, const int inNumUnits, Territory* inTarget);
    ~Deploy();                                                       //Destructor
    Deploy(const Deploy &dep);                                       //Copy constructor
    Deploy &operator=(const Deploy &dep);                            //Assignment operator


    virtual void execute() override;
    virtual bool validate() override;

    friend std::ostream &operator<<(std::ostream &out, Deploy &dep); //Stream insertion operator
private:
    // num units to take from reinforcement pool
    std::size_t armiesToDeploy = 0;
    // territory to place the units, *should* be owned
    Territory* targetTerritory = nullptr;
    // the player who owns this order
    Player* owner = nullptr;
};

/**
 * The function and member declarations of the Advance class 
 */
class Advance : public Order
{
public:
    Advance();                                                        //Constructor
    Advance(Player* owner, Territory* src, Territory* dest, std::size_t armiesToAdvance);
    ~Advance();                                                       //Destructor
    Advance(const Advance &adv);                                      //Copy constructor
    Advance &operator=(const Advance &adv);                           //Assignment operator

    virtual void execute() override;
    virtual bool validate() override;

    friend std::ostream &operator<<(std::ostream &out, Advance &adv); //Stream insertion operator
private:
    Player* owner = nullptr;
    Territory* src = nullptr; 
    Territory* dest = nullptr; 
    std::size_t armiesToAdvance = 0;
};

/**
 * The function and member declarations of the Bomb class 
 */
class Bomb : public Order
{
public:
    Bomb();                                                        //Constructor
    Bomb(Player* inOwner, Territory* inTarget);
    ~Bomb();                                                       //Destructor
    Bomb(const Bomb &bom);                                         //Copy constructor
    Bomb &operator=(const Bomb &bom);                              //Assignment operator

    virtual void execute() override;
    virtual bool validate() override;

    friend std::ostream &operator<<(std::ostream &out, Bomb &bom); //Stream insertion operator
private:
    Territory* target = nullptr;
    Player* owner = nullptr;
};

/**
 * The function and member declarations of the Blockade class 
 */
class Blockade : public Order
{
public:
    /* ctors */
    Blockade();
    Blockade(GameEngine* inCurrentInstance, Player* inOwner, Territory* inTarget);
    ~Blockade();
    Blockade(const Blockade &blo);
    /* operators */
    Blockade &operator=(const Blockade &blo);

    virtual void execute() override;
    virtual bool validate() override;

    friend std::ostream &operator<<(std::ostream &out, Blockade &blo); //Stream insertion operator
private:
    Player* owner = nullptr;
    Territory* target = nullptr;
    GameEngine* currentInstance;
};

/**
 * The function and member declarations of the Airlift class 
 */
class Airlift : public Order
{
public:
    Airlift();                                                        //Constructor
    Airlift(Player* inOwner, Territory* src, Territory* dest, std::size_t inArmiesToAirlift);
    ~Airlift();                                                       //Destructor
    Airlift(const Airlift &air);                                      //Copy constructor
    Airlift &operator=(const Airlift &air);                           //Assignment 

    virtual void execute() override;
    virtual bool validate() override;

    friend std::ostream &operator<<(std::ostream &out, Airlift &air); //Stream insertion operator
private:
    Player* owner = nullptr;
    Territory* src = nullptr;
    Territory* dest = nullptr;
    std::size_t armiesToAirlift = 0;
};

/**
 * The function and member declarations of the Negotiate class 
 */
class Negotiate : public Order
{
public:
    Negotiate();                                                        //Constructor
    Negotiate(Player* inOwner, Player* inTarget);
    ~Negotiate();                                                       //Destructor
    Negotiate(const Negotiate &ngo);                                    //Copy constructor
    Negotiate &operator=(const Negotiate &ngo);                         //Assignment 

    virtual void execute() override;
    virtual bool validate() override;

    friend std::ostream &operator<<(std::ostream &out, Negotiate &ngo); //Stream insertion operator
private:
    Player* owner = nullptr;
    Player* target = nullptr;
};

/**
 * The function and member declarations of the OrdersList class 
 */
class OrdersList : public Subject , ILoggable
{
public:
    OrdersList();                                                       //Constructor
    ~OrdersList();                                                      //Destructor
    OrdersList(const OrdersList &ol);                                    //Copy constructor
    OrdersList &operator=(const OrdersList &ol);                         //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, OrdersList &ol); //Stream insertion operator

    void add(Order *o);
    Order *get(const int index);
    void move(const int oldIndex, const int newIndex);
    void remove(const int index);
    const std::list<Order *> getOList();
    virtual std::string stringToLog();

private:
    std::list<Order *> oList; //The list of Orders
};