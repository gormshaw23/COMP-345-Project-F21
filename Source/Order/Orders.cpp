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
Order::Order(EOrderType typeValue, std::string descValue)
{
    oType = typeValue;
    description = descValue;
}

/**
 * Getter for the Order's type
 * 
 * @return the Order's type
 */
const EOrderType Order::getType()
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
        case EOrderType::Deploy:
            typeString = "Deploy";
            break;
        case EOrderType::Advance:
            typeString = "Advance";
            break;
        case EOrderType::Bomb:
            typeString = "Bomb";
            break;
        case EOrderType::Blockade:
            typeString = "Blockade";
            break;
        case EOrderType::Airlift:
            typeString = "Airlift";
            break;
        case EOrderType::Negotiate:
            typeString = "Negotiate";
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

/********************************************************************
 * Deploy class function definitions
 ********************************************************************/

Deploy::Deploy() : Order(EOrderType::Deploy, "Place some armies on one of the current player’s territories.")
{
}

Deploy::~Deploy() {}

Deploy::Deploy(const Deploy &dep) : Order(dep)
{
}

Deploy &Deploy::operator=(const Deploy &dep)
{
    Order::operator=(dep);
    return *this;
}

std::ostream &operator<<(std::ostream &out, Deploy &dep)
{
    out << "Order: " << dep.getTypeName() << "\nDescription: " << dep.getDescription();
    return out;
}

void Deploy::execute()
{

}

bool Deploy::validate()
{
    return false;
}



/********************************************************************
 * Advance class function definitions
 ********************************************************************/

Advance::Advance() : Order(EOrderType::Advance,
                           "Move some armies from one of the current player’s territories (source) to an adjacent territory (target). If the target territory belongs to the current player, the armies are moved to the target territory. If the target territory belongs to another player, an attack happens between the two territories")
{
}

Advance::~Advance() {}

Advance::Advance(const Advance &adv) : Order(adv)
{
}

Advance &Advance::operator=(const Advance &adv)
{
    Order::operator=(adv);
    return *this;
}

std::ostream &operator<<(std::ostream &out, Advance &adv)
{
    out << "Order: " << adv.getTypeName() << "\nDescription: " << adv.getDescription();
    return out;
}

void Advance::execute()
{

}

bool Advance::validate()
{
    return false;
}

/********************************************************************
 * Bomb class function definitions
 ********************************************************************/

Bomb::Bomb() : Order(EOrderType::Bomb, "Destroy half of the armies located on an opponent’s territory that is adjacent to one of the current player’s territories.")
{
}

Bomb::~Bomb() {}

Bomb::Bomb(const Bomb &bom) : Order(bom)
{
}

Bomb &Bomb::operator=(const Bomb &bom)
{
    Order::operator=(bom);
    return *this;
}

std::ostream &operator<<(std::ostream &out, Bomb &bom)
{
    out << "Order: " << bom.getTypeName() << "\nDescription: " << bom.getDescription();
    return out;
}

void Bomb::execute()
{

}

bool Bomb::validate()
{
    return false;
}

/********************************************************************
 * Blockade class function definitions
 ********************************************************************/

Blockade::Blockade() : Order(EOrderType::Blockade, "triple the number of armies on one of the current player’s territories and make it a neutral territory")
{
}

Blockade::~Blockade() {}

Blockade::Blockade(const Blockade &blo) : Order(blo)
{
}

Blockade &Blockade::operator=(const Blockade &blo)
{
    Order::operator=(blo);
    return *this;
}

std::ostream &operator<<(std::ostream &out, Blockade &blo)
{
    out << "Order: " << blo.getTypeName() << "\nDescription: " << blo.getDescription();
    return out;
}

void Blockade::execute()
{

}

bool Blockade::validate()
{
    return false;
}

/********************************************************************
 * Airlift class function definitions
 ********************************************************************/

Airlift::Airlift() : Order(EOrderType::Airlift, "Advance some armies from one of the current player’s territories to any another territory")
{
}

Airlift::~Airlift() {}

Airlift::Airlift(const Airlift &air) : Order(air)
{
}

Airlift &Airlift::operator=(const Airlift &air)
{
    Order::operator=(air);
    return *this;
}

std::ostream &operator<<(std::ostream &out, Airlift &air)
{
    out << "Order: " << air.getTypeName() << "\nDescription: " << air.getDescription();
    return out;
}

void Airlift::execute()
{

}

bool Airlift::validate()
{
    return false;
}

/********************************************************************
 * Negotiate class function definitions
 ********************************************************************/

Negotiate::Negotiate() : Order(EOrderType::Negotiate, "Prevent attacks between the current player and another player until the end of the turn")
{
}

Negotiate::~Negotiate() {}

Negotiate::Negotiate(const Negotiate &ngo) : Order(ngo)
{
}

Negotiate &Negotiate::operator=(const Negotiate &ngo)
{
    Order::operator=(ngo);
    return *this;
}

std::ostream &operator<<(std::ostream &out, Negotiate &ngo)
{
    out << "Order: " << ngo.getTypeName() << "\nDescription: " << ngo.getDescription();
    return out;
}

void Negotiate::execute()
{

}

bool Negotiate::validate()
{
    return false;
}

/********************************************************************
 * OrdersList class function definitions
 ********************************************************************/

OrdersList::OrdersList()
{
}

OrdersList::~OrdersList()
{
    this->oList.clear();
}

OrdersList::OrdersList(const OrdersList &ol)
{
    this->oList = ol.oList;
}

OrdersList &OrdersList::operator=(const OrdersList &ol)
{
    this->oList = ol.oList;
    return *this;
}

std::ostream &operator<<(std::ostream &out, OrdersList &ol)
{
    out << "\nContents of OrdersList" << std::endl;
    out << "List size: " << ol.oList.size() << std::endl;
    for (Order *o : ol.getOList())
    {
        out << "\nOrder: " << o->getTypeName() << "\n";
    }

    return out;
}

/*
 * Adds an element to the OrdersList
 * 
 * @param o an Order object
 */
void OrdersList::add(Order *o)
{
    oList.push_back(o); //Add Order to the back of the list
}

/*
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

/*
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

/*
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

/*
 * Getter for the list of Orders
 * 
 * @return a list of Orders
 */
const std::list<Order *> OrdersList::getOList()
{
    return oList;
}