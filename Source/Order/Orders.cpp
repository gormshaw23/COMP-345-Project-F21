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
    out << "Order: " << od->getType() << "\nDescription: " << od->getDescription();
    return out;
}

/**
 * 2-parameter constructor of the Order class
 * 
 * @param typeValue the Order type
 * @param descValue the Order description
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

    string validStr = (isValid) ? "valid" : "not valid";

    cout << "\nThe order of type " << this->getType() << " is " << validStr << endl;
    return isValid;
}

/**
 * Executes an order if it is a valid order
 */
void Order::execute()
{
    cout << "Executing " << this->getType() << " order" << endl;
}

/**
 * Getter for the Order's type
 * 
 * @return the Order's type
 */
const string Order::getType()
{
    return *type;
}

/**
 * Getter for the Order's description
 * 
 * @return the Order's description
 */
const string Order::getDescription()
{
    return *description;
}

/********************************************************************
 * Deploy class function definitions
 ********************************************************************/

/**
 * Constructor of the Deploy class
 */
Deploy::Deploy() : Order("deploy", "Place some armies on one of the current player’s territories.")
{
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
    out << "Order: " << dep->getType() << "\nDescription: " << dep->getDescription();
    return out;
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
ostream &operator<<(ostream &out, Advance *adv)
{
    out << "Order: " << adv->getType() << "\nDescription: " << adv->getDescription();
    return out;
}

/********************************************************************
 * Bomb class function definitions
 ********************************************************************/

/**
 * Constructor of the Bomb class
 */
Bomb::Bomb() : Order("bomb", "Destroy half of the armies located on an opponent’s territory that is adjacent to one of the current player’s territories.")
{
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
ostream &operator<<(ostream &out, Bomb *bom)
{
    out << "Order: " << bom->getType() << "\nDescription: " << bom->getDescription();
    return out;
}

/********************************************************************
 * Blockade class function definitions
 ********************************************************************/

/**
 * Constructor of the Order class
 */
Blockade::Blockade() : Order("blockade", "triple the number of armies on one of the current player’s territories and make it a neutral territory")
{
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
ostream &operator<<(ostream &out, Blockade *blo)
{
    out << "Order: " << blo->getType() << "\nDescription: " << blo->getDescription();
    return out;
}

/********************************************************************
 * Airlift class function definitions
 ********************************************************************/

/**
 * Constructor of the Airlift class
 */
Airlift::Airlift() : Order("airlift", "Advance some armies from one of the current player’s territories to any another territory")
{
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
ostream &operator<<(ostream &out, Airlift *air)
{
    out << "Order: " << air->getType() << "\nDescription: " << air->getDescription();
    return out;
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
ostream &operator<<(ostream &out, Negotiate *ngo)
{
    out << "Order: " << ngo->getType() << "\nDescription: " << ngo->getDescription();
    return out;
}

/********************************************************************
 * OrderList class function definitions
 ********************************************************************/

/**
 * Constructor of the OrderList class
 */
OrderList::OrderList()
{
    oList = new list<Order *>;
}

/**
 * Destructor of the OrderList class
 */
OrderList::~OrderList()
{
}

/**
 * Copy constructor of the OrderList class
 */
OrderList::OrderList(const OrderList &ol)
{
    this->oList = new list<Order *>(*(ol.oList));
}

/**
 * Assignment operator of the OrderList class
 */
OrderList &OrderList::operator=(const OrderList &ol)
{
    this->oList = new list<Order *>(*(ol.oList));
    return *this;
}

/**
 * Stream insertion operator of the OrderList class
 */
ostream &operator<<(ostream &out, OrderList *ol)
{
    // list<Order*> displayOList = ;
    out << "\nContents of OrderList" << endl;
    out << "List size: " << ol->oList->size() << endl;
    for (Order *o : ol->getOList())
    {
        out << "\nOrder: " << o->getType() << "\n";
    }

    return out;
}

/**
 * Adds an element to the OrderList
 * 
 * @param o an Order object
 */
void OrderList::add(Order *o)
{
    oList->push_back(o); //Add Order to the back of the list
}

/**
 * Gets an Order from the OrderList
 * 
 * @param index
 */
Order *OrderList::get(int index)
{
    auto o = oList->begin();
    advance(o, index); //Iterate the list to the given index
    return *o;
}

/**
 * Moves an Order from one position to another
 * 
 * @param oldIndex the original position
 * @param newIndex the new position
 */
void OrderList::move(int oldIndex, int newIndex)
{
    Order *order1 = get(oldIndex); //Order to be swapped
    Order *order2 = get(newIndex);
    list<Order *> *newList = new list<Order *>; //Create new list to add the swapped Orders
    int size = oList->size();

    for (int i = 0; i < size; i++)
    {
        if (i == newIndex)
            newList->push_back(order1);
        else if (i == oldIndex)
            newList->push_back(order2);
        else
        {
            Order *o = get(i);
            newList->push_back(o);
        }
    }
    oList->swap(*newList);
    delete newList;
}

/**
 * Removes an Order from the OrderList
 * 
 * @param index
 */
void OrderList::remove(int index)
{
    Order *o = get(index);
    oList->remove(o);
    delete o; //Remove order from memory
}

/**
 * Getter for the list of Orders
 * 
 * @return a list of Orders
 */
const list<Order *> OrderList::getOList()
{
    return *oList;
}