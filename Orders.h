/**
 * Class header for the Order class and its subclasses
 * Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate.
 * The class header also has the OrdersList class.
 * 
 * @author Mark Nasol
 */
#pragma once

#include <string>
using namespace std;


/**
 * The function and member declarations of the Order class 
 */
class Order
{
public:
    Order(); //Constructor
    ~Order(); //Destructor
    Order(const Order &od); //Copy constructor
    Order &operator =(const Order &od); //Assignment operator
    friend ostream &operator <<(ostream &out, Order *od); //Stream insertion operator

    Order(string typeValue, string descValue);
    bool validate();
    virtual void execute();
    string getType();
    string getDescription();
    int getTerritory();
    int getNumArmies();
    void setType(string newType);
    void setDescription(string newDescription);
    void setTerritory(int newTerritory);
    void setNumArmies(int newNumArmies);

private:
    string *type; //The order type
    string *description; //The order description
    int *territory; //The territory id
    int *numArmies; //The number of armies to be deployed
};

/**
 * The function and member declarations of the Deploy class 
 */
class Deploy : public Order
{
public:
    Deploy(); //Constructor
    ~Deploy(); //Destructor
    Deploy(const Deploy &dep); //Copy constructor
    Deploy &operator =(const Deploy &dep); //Assignment operator
    friend ostream &operator <<(ostream &out, Deploy *dep); //Stream insertion operator

    void execute() override;
};

/**
 * The function and member declarations of the Advance class 
 */
class Advance : public Order
{
public:
    Advance(); //Constructor
    ~Advance(); //Destructor
    Advance(const Advance &adv); //Copy constructor
    Advance &operator =(const Advance &adv); //Assignment operator
    friend ostream &operator <<(ostream &out, const Advance &adv); //Stream insertion operator

    void execute() override;
};

/**
 * The function and member declarations of the Bomb class 
 */
class Bomb : public Order
{
public:
    Bomb(); //Constructor
    ~Bomb(); //Destructor
    Bomb(const Bomb &bom); //Copy constructor
    Bomb &operator =(const Bomb &bom); //Assignment operator
    friend ostream &operator <<(ostream &out, const Bomb &bom); //Stream insertion operator

    void execute() override;
};

/**
 * The function and member declarations of the Blockade class 
 */
class Blockade : public Order
{
public:
    Blockade(); //Constructor
    ~Blockade(); //Destructor
    Blockade(const Blockade &blo); //Copy constructor
    Blockade &operator =(const Blockade &blo); //Assignment operator
    friend ostream &operator <<(ostream &out, const Blockade &blo); //Stream insertion operator

    void execute() override;
};

/**
 * The function and member declarations of the Airlift class 
 */
class Airlift : public Order
{
public:
    Airlift(); //Constructor
    ~Airlift(); //Destructor
    Airlift(const Airlift &air); //Copy constructor
    Airlift &operator =(const Airlift &air); //Assignment operator
    friend ostream &operator <<(ostream &out, const Airlift &air); //Stream insertion operator

    void execute() override;
};

/**
 * The function and member declarations of the Negotiate class 
 */
class Negotiate : public Order
{
public:
    Negotiate(); //Constructor
    ~Negotiate(); //Destructor
    Negotiate(const Negotiate &ngo); //Copy constructor
    Negotiate &operator =(const Negotiate &ngo); //Assignment operator
    friend ostream &operator <<(ostream &out, const Negotiate &ngo); //Stream insertion operator

    void execute() override;
};

/**
 * The function and member declarations of the OrderList class 
 */
class OrderList
{
public:
    OrderList(); //Constructor
    ~OrderList(); //Destructor
    OrderList(const OrderList &ol); //Copy constructor
    OrderList &operator =(const OrderList &ol); //Assignment operator
    friend ostream &operator <<(ostream &out, const OrderList &ol); //Stream insertion operator

    void move(); //Need parameters
    void remove(); //Need parameters
};