/**
 * Compilation unit for the Order class and its subclasses
 * Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate.
 * The compilation unit also has the OrdersList class.
 * 
 * @author Mark Nasol
 */

#include <iostream>
#include <list>
#include <string>
using namespace std;

#include "Orders.h"

/********************************************************************
 * Order class function definitions
 ********************************************************************/

/**
 * Constructor of the Order class
 */
Order::Order()
{
    type = new string("");
    description = new string("");
}

/**
 * Destructor of the Order class
 */
Order::~Order()
{
    delete type;
    delete description;
}

/**
 * Copy constructor of the Order class
 */
Order::Order(const Order &od)
{
    this->type = new string(*(od.type));
    this->description = new string(*(od.description));
}

/**
 * Assignment operator of the Order class
 */
Order &Order::operator=(const Order &od)
{
    this->type = new string(*(od.type));
    this->description = new string(*(od.description));
    return *this;
}

/**
 * Stream insertion operator of the Order class
 */
ostream &operator<<(ostream &out, Order *od)
{
    out << "Order: " << od->getType() << "\nDescription: " << od->getDescription()
        << "\nTerritory: " << od->getTerritory() << "\nNumber of armies: " << od->getNumArmies() << "\n";
    return out;
}

/**
 * 2-parameter constructor of the Order class
 */
Order::Order(string typeValue, string descValue)
{
    type = new string(typeValue);
    description = new string(descValue);
}

/**
 * Validates an order.
 * An order is considered valid if it is a subclass of Order
 * and if it is an existing order type.
 * 
 * @return true if the order is valid, otherwise false
 */
bool Order::validate()
{
    bool isValid = false;
    string types[6] = {"deploy", "advance", "bomb", "blockade", "airlift", "negotiate"};

    //Check if the Order is a subclass of the Order class
    if (!(dynamic_cast<Order *>(this)))
        return false;

    //Check if the type corresponds to a valid order type
    for (int i = 0; i < 6; i++)
    {
        string type = types[i];
        if (this->getType().compare(type) == 0)
            isValid = true;
    }

    return isValid;
}

/**
 * Executes an order if it is a valid order
 */
void Order::execute()
{
    //Check if the order is valid
    if (validate())
    {
        cout << "Order executed\n"
             << endl;
    }
}

/**
 * Getter for the Order's type
 */
string Order::getType()
{
    return *type;
}

/**
 * Getter for the Order's description
 */
string Order::getDescription()
{
    return *description;
}

/**
 * Getter for the Order's territory
 */
int Order::getTerritory()
{
    return *territory;
}

/**
 * Getter for the Order's number of armies
 */
int Order::getNumArmies()
{
    return *numArmies;
}

/**
 * Setter for the Order's type
 */
void Order::setType(string newType)
{
    *type = newType;
}

/**
 * Setter for the Order's description
 */
void Order::setDescription(string newDescription)
{
    *description = newDescription;
}

/**
 * Setter for the Order's territory
 */
void Order::setTerritory(int newTerritory)
{
    *territory = newTerritory;
}

/**
 * Setter for the Order's number of armies
 */
void Order::setNumArmies(int newNumArmies)
{
    *numArmies = newNumArmies;
}

/********************************************************************
 * Deploy class function definitions
 ********************************************************************/

/**
 * Constructor of the Deploy class
 */
Deploy::Deploy() : Order("deploy", "Place some armies on one of the current player’s territories.")
{
    this->setTerritory(1);
    this->setNumArmies(0);
}

/**
 * Destructor of the Deploy class
 */
Deploy::~Deploy() {}

/**
 * Copy constructor of the Deploy class
 */
Deploy::Deploy(const Deploy &dep) : Order(dep)
{
}

/**
 * Assignment operator of the Deploy class
 */
Deploy &Deploy::operator=(const Deploy &dep)
{
    Order::operator=(dep);
    return *this;
}

/**
 * Stream insertion operator of the Deploy class
 */
ostream &operator<<(ostream &out, Deploy *dep)
{
    out << "Order: " << dep->getType() << "\nDescription: " << dep->getDescription()
        << "\nTerritory: " << dep->getTerritory() << "\nNumber of armies: " << dep->getNumArmies() << "\n";
    return out;
}

/**
 * Overidden execute function for the Deploy class
 */
void Deploy::execute()
{
    //Check if the order is valid
    if (validate())
    {
        cout << "Deploying " << this->getNumArmies() << " armies to territory " << this->getTerritory() << endl;
    }
}

/********************************************************************
 * Advance class function definitions
 ********************************************************************/

/**
 * Constructor of the Advance class
 */
Advance::Advance() : Order("advance",
                           "Move some armies from one of the current player’s territories (source) to an adjacent territory (target). If the target territory belongs to the current player, the armies are moved to the target territory. If the target territory belongs to another player, an attack happens between the two territories")
{
    this->setTerritory(1);
    this->setNumArmies(0);
}

/**
 * Destructor of the Advance class
 */
Advance::~Advance() {}

/**
 * Copy constructor of the Advance class
 */
Advance::Advance(const Advance &adv) : Order(adv)
{
}

/**
 * Assignment operator of the Advance class
 */
Advance &Advance::operator=(const Advance &adv)
{
    Order::operator=(adv);
    return *this;
}

/**
 * Stream insertion operator of the Advance class
 */
ostream &operator<<(ostream &out, Advance*adv)
{
    out << "Order: " << adv->getType() << "\nDescription: " << adv->getDescription()
        << "\nTerritory: " << adv->getTerritory() << "\nNumber of armies: " << adv->getNumArmies() << "\n";
    return out;
}

/**
 * Overidden execute function for the Advance class
 */
void Advance::execute()
{
    //Check if the order is valid
    if (validate())
    {
        cout << "Moving " << this->getNumArmies() << " armies to territory " << this->getTerritory() << endl;
    }
}

/********************************************************************
 * Bomb class function definitions
 ********************************************************************/

/**
 * Constructor of the Bomb class
 */
Bomb::Bomb() : Order("bomb", "Destroy half of the armies located on an opponent’s territory that is adjacent to one of the current player’s territories.")
{
    this->setTerritory(1);
}

/**
 * Destructor of the Bomb class
 */
Bomb::~Bomb() {}

/**
 * Copy constructor of the Bomb class
 */
Bomb::Bomb(const Bomb &bom) : Order(bom)
{
}

/**
 * Assignment operator of the Bomb class
 */
Bomb &Bomb::operator=(const Bomb &bom)
{
    Order::operator=(bom);
    return *this;
}

/**
 * Stream insertion operator of the Bomb class
 */
ostream &operator<<(ostream &out, Bomb*bom)
{
    out << "Order: " << bom->getType() << "\nDescription: " << bom->getDescription()
        << "\nTerritory: " << bom->getTerritory() << "\nNumber of armies: " << bom->getNumArmies() << "\n";
    return out;
}

/**
 * Overidden execute function for the Bomb class
 */
void Bomb::execute()
{
    //Check if the order is valid
    if (validate())
    {
        cout << "Bombing territory " << this->getTerritory() << endl;
    }
}

/********************************************************************
 * Blockade class function definitions
 ********************************************************************/

/**
 * Constructor of the Order class
 */
Blockade::Blockade() : Order("blockade", "triple the number of armies on one of the current player’s territories and make it a neutral territory")
{
    this->setTerritory(1);
    this->setNumArmies(0);
}

/**
 * Destructor of the Order class
 */
Blockade::~Blockade() {}

/**
 * Copy constructor of the Blockade class
 */
Blockade::Blockade(const Blockade &blo) : Order(blo)
{
}

/**
 * Assignment operator of the Blockade class
 */
Blockade &Blockade::operator=(const Blockade &blo)
{
    Order::operator=(blo);
    return *this;
}

/**
 * Stream insertion operator of the Blockade class
 */
ostream &operator<<(ostream &out, Blockade*blo)
{
    out << "Order: " << blo->getType() << "\nDescription: " << blo->getDescription()
        << "\nTerritory: " << blo->getTerritory() << "\nNumber of armies: " << blo->getNumArmies() << "\n";
    return out;
}

/**
 * Overidden execute function for the Blockade class
 */
void Blockade::execute()
{
    //Check if the order is valid
    if (validate())
    {
        cout << "Tripling the number of armies on territory " << this->getTerritory() << endl;
    }
}

/********************************************************************
 * Airlift class function definitions
 ********************************************************************/

/**
 * Constructor of the Airlift class
 */
Airlift::Airlift() : Order("airlift", "Advance some armies from one of the current player’s territories to any another territory")
{
    this->setTerritory(1);
    this->setNumArmies(0);
}

/**
 * Destructor of the Airlift class
 */
Airlift::~Airlift() {}

/**
 * Copy constructor of the Airlift class
 */
Airlift::Airlift(const Airlift &air) : Order(air)
{
}

/**
 * Assignment operator of the Airlift class
 */
Airlift &Airlift::operator=(const Airlift &air)
{
    Order::operator=(air);
    return *this;
}

/**
 * Stream insertion operator of the Airlift class
 */
ostream &operator<<(ostream &out, Airlift*air)
{
    out << "Order: " << air->getType() << "\nDescription: " << air->getDescription()
        << "\nTerritory: " << air->getTerritory() << "\nNumber of armies: " << air->getNumArmies() << "\n";
    return out;
}

/**
 * Overidden execute function for the Airlift class
 */
void Airlift::execute()
{
    //Check if the order is valid
    if (validate())
    {
        cout << "Airlifting " << this->getNumArmies << " armies to territory " << this->getTerritory() << endl;
    }
}

/********************************************************************
 * Negotiate class function definitions
 ********************************************************************/

/**
 * Constructor of the Negotiate class
 */
Negotiate::Negotiate() : Order("negotiate", "Prevent attacks between the current player and another player until the end of the turn")
{
}

/**
 * Destructor of the Negotiate class
 */
Negotiate::~Negotiate() {}

/**
 * Copy constructor of the Negotiate class
 */
Negotiate::Negotiate(const Negotiate &ngo) : Order(ngo)
{
}

/**
 * Assignment operator of the Negotiate class
 */
Negotiate &Negotiate::operator=(const Negotiate &ngo)
{
    Order::operator=(ngo);
    return *this;
}

/**
 * Stream insertion operator of the Negotiate class
 */
ostream &operator<<(ostream &out, Negotiate*ngo)
{
    out << "Order: " << ngo->getType() << "\nDescription: " << ngo->getDescription()
        << "\nTerritory: " << ngo->getTerritory() << "\nNumber of armies: " << ngo->getNumArmies() << "\n";
    return out;
}

/**
 * Overidden execute function for the Negotiate class
 */
void Negotiate::execute()
{
    //Check if the order is valid
    if (validate())
    {
        cout << "An attack has been prevented from a negotiation between 2 players." << endl;
    }

/********************************************************************
 * OrderList class function definitions
 ********************************************************************/

/**
 * Constructor of the Order class
 */
OrderList::OrderList()
{
}

/**
 * Destructor of the Order class
 */
OrderList::~OrderList()
{
}

OrderList::OrderList(const OrderList &ol)
{
}

// OrderList &OrderList::operator=(const OrderList &ol)
// {

// }

// ostream &operator<<(ostream &out, const OrderList &ol)
// {

// }