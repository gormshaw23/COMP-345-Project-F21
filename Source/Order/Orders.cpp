/**
 * Compilation unit for the Order class and its subclasses
 * Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate.
 * The compilation unit also has the OrdersList class.
 * 
 * @author Mark Nasol
 */

#include <iostream>
#include <iterator>
#include <list>
#include <string>

#include "Orders.h"

/********************************************************************
 * Order class function definitions
 ********************************************************************/

/**
 * Constructor of the Order class
 */
Order::Order()
{
    description = "";
}

/**
 * Destructor of the Order class
 */
Order::~Order()
{
}

/**
 * Copy constructor of the Order class
 */
Order::Order(const Order &od)
{
    this->oType = od.oType;
    this->description = od.description;
}

/**
 * Assignment operator of the Order class
 */
Order &Order::operator=(const Order &od)
{
    this->oType = od.oType;
    this->description = od.description;
    return *this;
}

/**
 * Stream insertion operator of the Order class
 */
std::ostream &operator<<(std::ostream &out, Order &od)
{
    out << "Order: " << od.getTypeName() << "\nDescription: " << od.getDescription();
    return out;
}

/**
 * 2-parameter constructor of the Order class
 * 
 * @param typeValue the Order type
 * @param descValue the Order description
 */
Order::Order(OrderType typeValue, std::string descValue)
{
    oType = typeValue;
    description = descValue;
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
    std::string orderType[6] = {"deploy", "advance", "bomb", "blockade", "airlift", "negotiate"};

    //Check if the Order is a subclass of the Order class
    if (!(dynamic_cast<Order *>(this)))
        return false;

    //Check if the type corresponds to a valid order type
    for (int i = 0; i < 6; i++)
    {
        std::string type = orderType[i];
        if (this->getTypeName().compare(type) == 0)
            isValid = true;
    }

    std::string validStr = (isValid) ? "valid" : "not valid";

    std::cout << "\nThe order of type " << this->getTypeName() << " is " << validStr << std::endl;
    return isValid;
}

/**
 * Executes an order if it is a valid order
 */
void Order::execute()
{
    std::cout << "Executing " << this->getTypeName() << " order" << std::endl;
    this->Notify(*this);
}

/**
 * Getter for the Order's type
 * 
 * @return the Order's type
 */
const Order::OrderType Order::getType()
{
    return oType;
}

/**
 * Getter for the Order's type name
 * 
 * @return the Order's type name
 */
const std::string Order::getTypeName()
{
    std::string typeString = "";
    switch(this->getType()){
        case Order::OrderType::deploy:
            typeString = "deploy";
            break;
        case Order::OrderType::advance:
            typeString = "advance";
            break;
        case Order::OrderType::bomb:
            typeString = "bomb";
            break;
        case Order::OrderType::blockade:
            typeString = "blockade";
            break;
        case Order::OrderType::airlift:
            typeString = "airlift";
            break;
        case Order::OrderType::negotiate:
            typeString = "negotiate";
            break;
        default:
            break;
    }
    return typeString;
}

/**
 * Getter for the Order's description
 * 
 * @return the Order's description
 */
const std::string Order::getDescription()
{
    return description;
}



std::string Order::stringToLog() {
    std::string  toLog = "Temporary info : order executed name : " + this->getTypeName() + " , order description : " + this->getDescription();
    return toLog;
}
/********************************************************************
 * Deploy class function definitions
 ********************************************************************/

/**
 * Constructor of the Deploy class
 */
Deploy::Deploy() : Order(OrderType::deploy, "Place some armies on one of the current player’s territories.")
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
std::ostream &operator<<(std::ostream &out, Deploy &dep)
{
    out << "Order: " << dep.getTypeName() << "\nDescription: " << dep.getDescription();
    return out;
}

/********************************************************************
 * Advance class function definitions
 ********************************************************************/

/**
 * Constructor of the Advance class
 */
Advance::Advance() : Order(OrderType::advance,
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
std::ostream &operator<<(std::ostream &out, Advance &adv)
{
    out << "Order: " << adv.getTypeName() << "\nDescription: " << adv.getDescription();
    return out;
}

/********************************************************************
 * Bomb class function definitions
 ********************************************************************/

/**
 * Constructor of the Bomb class
 */
Bomb::Bomb() : Order(OrderType::bomb, "Destroy half of the armies located on an opponent’s territory that is adjacent to one of the current player’s territories.")
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
std::ostream &operator<<(std::ostream &out, Bomb &bom)
{
    out << "Order: " << bom.getTypeName() << "\nDescription: " << bom.getDescription();
    return out;
}

/********************************************************************
 * Blockade class function definitions
 ********************************************************************/

/**
 * Constructor of the Order class
 */
Blockade::Blockade() : Order(OrderType::blockade, "triple the number of armies on one of the current player’s territories and make it a neutral territory")
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
std::ostream &operator<<(std::ostream &out, Blockade &blo)
{
    out << "Order: " << blo.getTypeName() << "\nDescription: " << blo.getDescription();
    return out;
}

/********************************************************************
 * Airlift class function definitions
 ********************************************************************/

/**
 * Constructor of the Airlift class
 */
Airlift::Airlift() : Order(OrderType::airlift, "Advance some armies from one of the current player’s territories to any another territory")
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
std::ostream &operator<<(std::ostream &out, Airlift &air)
{
    out << "Order: " << air.getTypeName() << "\nDescription: " << air.getDescription();
    return out;
}

/********************************************************************
 * Negotiate class function definitions
 ********************************************************************/

/**
 * Constructor of the Negotiate class
 */
Negotiate::Negotiate() : Order(OrderType::negotiate, "Prevent attacks between the current player and another player until the end of the turn")
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
std::ostream &operator<<(std::ostream &out, Negotiate &ngo)
{
    out << "Order: " << ngo.getTypeName() << "\nDescription: " << ngo.getDescription();
    return out;
}

/********************************************************************
 * OrdersList class function definitions
 ********************************************************************/

/**
 * Constructor of the OrdersList class
 */
OrdersList::OrdersList()
{
}

/**
 * Destructor of the OrdersList class
 */
OrdersList::~OrdersList()
{
}

/**
 * Copy constructor of the OrdersList class
 */
OrdersList::OrdersList(const OrdersList &ol)
{
    this->oList = ol.oList;
}

/**
 * Assignment operator of the OrdersList class
 */
OrdersList &OrdersList::operator=(const OrdersList &ol)
{
    this->oList = ol.oList;
    return *this;
}

/**
 * Stream insertion operator of the OrdersList class
 */
std::ostream &operator<<(std::ostream &out, OrdersList &ol)
{
    // list<Order*> displayOList = ;
    out << "\nContents of OrdersList" << std::endl;
    out << "List size: " << ol.oList.size() << std::endl;
    for (Order *o : ol.getOList())
    {
        out << "\nOrder: " << o->getTypeName() << "\n";
    }

    return out;
}

/**
 * Adds an element to the OrdersList
 * 
 * @param o an Order object
 */
void OrdersList::add(Order *o)
{
    oList.push_back(o); //Add Order to the back of the list
    this->Notify(*this);
}

/**
 * Gets an Order from the OrdersList
 * 
 * @param index
 */
Order *OrdersList::get(int index)
{
    auto o = oList.begin();
    std::advance(o, index); //Iterate the list to the given index
    return *o;
}

/**
 * Moves an Order from one position to another
 * 
 * @param oldIndex the original position
 * @param newIndex the new position
 */
void OrdersList::move(int oldIndex, int newIndex)
{
    Order *order1 = get(oldIndex); //Order to be swapped
    Order *order2 = get(newIndex);
    std::list<Order *> newList; //Create new list to add the swapped Orders
    int size = oList.size();

    for (int i = 0; i < size; i++)
    {
        if (i == newIndex)
            newList.push_back(order1);
        else if (i == oldIndex)
            newList.push_back(order2);
        else
        {
            Order *o = get(i);
            newList.push_back(o);
        }
    }
    oList.swap(newList);
}

/**
 * Removes an Order from the OrdersList
 * 
 * @param index
 */
void OrdersList::remove(int index)
{
    Order *o = get(index);
    oList.remove(o);
    delete o; //Remove order from memory
}

/**
 * Getter for the list of Orders
 * 
 * @return a list of Orders
 */
const std::list<Order *> OrdersList::getOList()
{
    return oList;
}

std::string OrdersList::stringToLog() {

    std::string toLog = "New order added to the OrdersList : " + this->getOList().back()->getTypeName();
    return toLog;

 }