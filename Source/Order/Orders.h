/**
 * Class header for the Order class and its subclasses
 * Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate.
 * The class header also has the OrdersList class.
 * 
 * @author Mark Nasol
 */
#pragma once

#include <string>
#include <ostream>
#include <list>

#include"GameLog/LoggingObserver.h"

/**
 * The function and member declarations of the Order class 
 */
class Order : public Subject, ILoggable
{
public:
    enum OrderType
    {
        deploy,
        advance,
        bomb,
        blockade,
        airlift,
        negotiate
    };

    Order();                                                       //Constructor
    ~Order();                                                      //Destructor
    Order(const Order &od);                                        //Copy constructor
    Order &operator=(const Order &od);                             //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, Order &od); //Stream insertion operator

    Order(OrderType typeValue, std::string descValue);
    bool validate();
    void execute();
    const OrderType getType(); //Gets OrderType enum
    const std::string getTypeName(); //Gets name of the order type
    const std::string getDescription();

    virtual std::string stringToLog();  //use to send the effect of the order as a string 

    
private:
    OrderType oType;        //The order type
    std::string description; //The order description
};

/**
 * The function and member declarations of the Deploy class 
 */
class Deploy : public Order
{
public:
    Deploy();                                                        //Constructor
    ~Deploy();                                                       //Destructor
    Deploy(const Deploy &dep);                                       //Copy constructor
    Deploy &operator=(const Deploy &dep);                            //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, Deploy &dep); //Stream insertion operator
};

/**
 * The function and member declarations of the Advance class 
 */
class Advance : public Order
{
public:
    Advance();                                                        //Constructor
    ~Advance();                                                       //Destructor
    Advance(const Advance &adv);                                      //Copy constructor
    Advance &operator=(const Advance &adv);                           //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, Advance &adv); //Stream insertion operator
};

/**
 * The function and member declarations of the Bomb class 
 */
class Bomb : public Order
{
public:
    Bomb();                                                        //Constructor
    ~Bomb();                                                       //Destructor
    Bomb(const Bomb &bom);                                         //Copy constructor
    Bomb &operator=(const Bomb &bom);                              //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, Bomb &bom); //Stream insertion operator
};

/**
 * The function and member declarations of the Blockade class 
 */
class Blockade : public Order
{
public:
    Blockade();                                                        //Constructor
    ~Blockade();                                                       //Destructor
    Blockade(const Blockade &blo);                                     //Copy constructor
    Blockade &operator=(const Blockade &blo);                          //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, Blockade &blo); //Stream insertion operator
};

/**
 * The function and member declarations of the Airlift class 
 */
class Airlift : public Order
{
public:
    Airlift();                                                        //Constructor
    ~Airlift();                                                       //Destructor
    Airlift(const Airlift &air);                                      //Copy constructor
    Airlift &operator=(const Airlift &air);                           //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, Airlift &air); //Stream insertion operator
};

/**
 * The function and member declarations of the Negotiate class 
 */
class Negotiate : public Order
{
public:
    Negotiate();                                                        //Constructor
    ~Negotiate();                                                       //Destructor
    Negotiate(const Negotiate &ngo);                                    //Copy constructor
    Negotiate &operator=(const Negotiate &ngo);                         //Assignment operator
    friend std::ostream &operator<<(std::ostream &out, Negotiate &ngo); //Stream insertion operator
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