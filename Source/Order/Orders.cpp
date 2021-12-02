/**
 * Compilation unit for the Order class and its subclasses
 * Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate.
 * The compilation unit also has the OrdersList class.
 * 
 * @author Mark Nasol
 * @author Blayne Bradley (Assignment 2, Part 4)
 */

#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <random>

#include "Orders.h"
#include "Map/map.h"
#include "Player/Player.h"
#include "Engine/GameEngine.h"
#include "Common/localization.h"

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
<<<<<<< HEAD
=======
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
>>>>>>> origin/log
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

void Order::setDescrption(std::string inOrderEffect)
{
    description = inOrderEffect;
}


std::string Order::stringToLog() {
    std::string  toLog = "Temporary info : order executed name : " + this->getTypeName() + " , order description : " + this->getDescription();
    return toLog;
}
/********************************************************************
 * Deploy class function definitions
 ********************************************************************/

Deploy::Deploy() : Order(EOrderType::Deploy, DEPLOY_DESC)
{
}

Deploy::Deploy
(
    Player* inOwner, 
    const int inNumUnits, 
    Territory* inTarget
) : Order(EOrderType::Deploy, DEPLOY_DESC)
{
    this->owner = inOwner;
    this->targetTerritory = inTarget;
    this->armiesToDeploy = inNumUnits;
}

Deploy::~Deploy() 
{
    this->owner = nullptr;
    this->targetTerritory = nullptr;
    this->armiesToDeploy = 0;
}

Deploy::Deploy(const Deploy &dep) : Order(dep)
{
    this->owner = dep.owner;
    this->targetTerritory = dep.targetTerritory;
    this->armiesToDeploy = dep.armiesToDeploy;
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
    if (validate())
    {
        /*
            Since the requirements don't mention checking if the number of armies is correct
            it is instead assumed that if a number of armies is requested higher than the 
            number of available armies, then the available amount is used instead.
        */
        std::size_t armies = std::min(armiesToDeploy, owner->getReinforcementPoolSize());
        owner->setReinforcementPool(owner->getReinforcementPoolSize() - armies);
        
        // Pizza start
        // Get the effect of the order into a string 
        std::string orderEffect = "Order type : " + getTypeName() +", With the effect :  Owner : "+owner->getPlayerName() + ", Target territory : "+ targetTerritory->getName() + ", Number of army added  : " + " added army to Todo find how to add armiesToDeploy to the string "+ "" ;
       
        // change the description with the new string 
        setDescrption(orderEffect);
        // Pizza end

        targetTerritory->setNbArmy(armies);
    }
    // Pizza  
    // call notify at the end of the execute function
     Notify(*this);
}

bool Deploy::validate()
{
    // make sure there are instantiated objects to interact with
    if (targetTerritory == nullptr || owner == nullptr)
    {
        return false;
    }

    // make sure the target territory has a owner
    if (targetTerritory->getPlayer() == nullptr)
    {
        return false;
    }

    // make sure the owner of the target territory is owned by the current player
    if (owner->getPlayerID() != targetTerritory->getPlayer()->getPlayerID())
    {
        return false;
    }

    return true;
}



/********************************************************************
 * Advance class function definitions
 ********************************************************************/

Advance::Advance() : Order(EOrderType::Advance, ADVANCE_DESC)
{
}

Advance::Advance
(
    Player* inOwner, 
    Territory* inSrc, 
    Territory* inDest, 
    std::size_t inArmiesToAdvance
) : Order(EOrderType::Advance, ADVANCE_DESC)
{
    this->owner = inOwner;
    this->src = inSrc;
    this->dest = inDest;
    this->armiesToAdvance = inArmiesToAdvance;
}

Advance::~Advance() 
{
    this->owner = nullptr;
    this->src = nullptr;
    this->dest = nullptr;
    armiesToAdvance = 0;
}

Advance::Advance(const Advance &adv) : Order(adv)
{
    this->owner = adv.owner;
    this->src = adv.src;
    this->dest = adv.dest;
    this->armiesToAdvance = adv.armiesToAdvance;
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
    if (validate())
    {
        // saving our initial allotment of units to be moved for math later on
        const std::size_t initialAdvancingArmies = std::min(static_cast<std::size_t>(src->getNbArmy()), armiesToAdvance);
        // what units we'll be interacting with
        std::size_t remainingAdvancingArmies = initialAdvancingArmies;
        if (src->getPlayer()->getPlayerID() == dest->getPlayer()->getPlayerID())
        {
            // move the troops, no combat
            // we move either the specified number or if larger than what is available
            // we move all available troops.
            dest->setNbArmy(remainingAdvancingArmies + dest->getNbArmy());
        }
        else
        {
            // WAAAAAGH!!!! The enemy territory has defenders! o7
            if (dest->getNbArmy() > 0)
            {
                // initial our random number generator, this is a better and more secure
                // than the typical default rng method
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist100(1, 100);
                while (remainingAdvancingArmies > 0 && dest->getNbArmy() > 0)
                {
                    // assumed that basically every attacking unit gets a chance to attack
                    // and the unit that is defending gets a chance to fire back; with dmg
                    // calculated at the sametime.
                    for (int i = 0; i < remainingAdvancingArmies; ++i)
                    {
                        int attackResult = dist100(rng);
                        // 60% chance to eliminate a defending unit
                        if (attackResult <= ATTACKER_CHANCE)
                        {
                            dest->setNbArmy(dest->getNbArmy() - 1);
                        }

                        int defendResult = dist100(rng);

                        // 70% chance a defending unit can eliminate an 
                        // attacking unit
                        if (defendResult <= DEFENDER_CHANCE)
                        {
                            // available troops diminished
                            remainingAdvancingArmies--;
                        }
                    }
                }

                if (remainingAdvancingArmies > 0)
                {
                    // defender loses, move to occupy with
                    // remaining forces.
                    dest->setPlayer(src->getPlayer());
                    dest->setNbArmy(remainingAdvancingArmies);
                    owner->setCapturedTerritoryFlag(true);
                }

                // happens either way
                src->setNbArmy(src->getNbArmy() - initialAdvancingArmies);
            }
            else
            {
                // move to occupy
                dest->setPlayer(src->getPlayer());
                dest->setNbArmy(remainingAdvancingArmies);
                src->setNbArmy(src->getNbArmy() - initialAdvancingArmies);
            }
        }
    }
}

bool Advance::validate()
{
    // make sure there are instantiated objects to interact with
    if (owner == nullptr || src == nullptr || dest == nullptr)
    {
        return false;
    }

    // make sure the target territories have owners
    if (src->getPlayer() == nullptr || dest->getPlayer() == nullptr)
    {
        return false;
    }

    // make sure the owner of the src territory is owned by the current player
    if (owner->getPlayerID() != src->getPlayer()->getPlayerID())
    {
        return false;
    }

    // check if there is a truce via negotiate
    std::vector<Player*> negotiatees = owner->getNotAttackablePlayers();
    if (std::find(negotiatees.begin(), negotiatees.end(), dest->getPlayer()) != negotiatees.end())
    {
        return false;
    }

    // check to make sure that the destination is adjacent to the source
    std::vector<Territory*> adjacents = src->getBorderList();
    if (adjacents.empty() || std::find(adjacents.begin(), adjacents.end(), dest) == adjacents.end())
    {
        return false;
    }

    return true;
}

/********************************************************************
 * Bomb class function definitions
 ********************************************************************/

Bomb::Bomb() : Order(EOrderType::Bomb, BOMB_DESC)
{

}

Bomb::Bomb(Player* inOwner, Territory* inTarget)
{
    this->owner = inOwner;
    this->target = inTarget;
}

Bomb::~Bomb() 
{
    this->owner = nullptr;
    this->target = nullptr;
}

Bomb::Bomb(const Bomb &bom) : Order(bom)
{
    this->owner = bom.owner;
    this->target = bom.target;
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
    if (validate())
    {
        target->setNbArmy(target->getNbArmy() / 2);
    }
}

bool Bomb::validate()
{
    // check to make sure interactable objects exist
    if (owner == nullptr || target == nullptr)
    {
        return false;
    }

    if (target->getPlayer() == nullptr)
    {
        return false;
    }

    // check if target is owned by player
    if (target->getPlayer()->getPlayerID() == owner->getPlayerID())
    {
        return false;
    }

    bool isAdjacent = false;
    std::vector<Territory*> playerTerritories = owner->getTerritoriesOwned();
    for (auto& territory : playerTerritories)
    {
        // check to make sure that the target is adjacent to one of players territories
        // this is a very terribly inefficient algorithm
        // gaze on my works and despair
        std::vector<Territory*> adjacents = territory->getBorderList();
        if (std::find(adjacents.begin(), adjacents.end(), target) != adjacents.end())
        {
            isAdjacent = true;
            break;
        }
    }

    if (!isAdjacent)
    {
        return false;
    }

    // check if there is a truce via negotiate
    std::vector<Player*> negotiatees = owner->getNotAttackablePlayers();
    if (std::find(negotiatees.begin(), negotiatees.end(), target->getPlayer()) != negotiatees.end())
    {
        return false;
    }

    return true;
}

/********************************************************************
 * Blockade class function definitions
 ********************************************************************/

Blockade::Blockade() : Order(EOrderType::Blockade, BLOCKADE_DESC)
{
}

Blockade::Blockade(Player* inOwner, Territory* inTarget)
{
    this->owner = inOwner;
    this->target = inTarget;
}

Blockade::~Blockade() 
{
    this->owner = nullptr;
    this->target = nullptr;
}

Blockade::Blockade(const Blockade &blo) : Order(blo)
{
    this->owner = blo.owner;
    this->target = blo.target;
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
    if (validate())
    {
        // add the target territory to the neutral player and double the number of troops
        Player* neutralPlayer = GameEngine::getInstance().getNeutralPlayer();
        if (neutralPlayer != nullptr)
        {
            neutralPlayer->getTerritoriesOwned().push_back(target);
            owner->getTerritoriesOwned().erase
            (
                std::remove
                (
                    owner->getTerritoriesOwned().begin(),
                    owner->getTerritoriesOwned().end(),
                    target
                ),
                owner->getTerritoriesOwned().end()
            );

            target->setNbArmy(target->getNbArmy() * 2);
        }
    }
}

bool Blockade::validate()
{
    // check to make sure interactable objects exist
    if (owner == nullptr || target == nullptr)
    {
        return false;
    }

    // check if target is owned by player
    if (target->getPlayer()->getPlayerID() != owner->getPlayerID())
    {
        return false;
    }

    return true;
}

/********************************************************************
 * Airlift class function definitions
 ********************************************************************/

Airlift::Airlift() : Order(EOrderType::Airlift, AIRLIFT_DESC)
{
}

Airlift::Airlift(Player* inOwner, Territory* inSrc, Territory* inDest, std::size_t inArmiesToAirlift)
{
    this->owner = inOwner;
    this->src = inSrc;
    this->dest = inDest;
    this->armiesToAirlift = inArmiesToAirlift;
}

Airlift::~Airlift() 
{
    this->owner = nullptr;
    this->src = nullptr;
    this->dest = nullptr;
    this->armiesToAirlift = 0;
}

Airlift::Airlift(const Airlift &air) : Order(air)
{
    this->owner = air.owner;
    this->src = air.src;
    this->dest = air.dest;
    this->armiesToAirlift = air.armiesToAirlift;
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
    if (validate())
    {
        const std::size_t initialArmiesToAirlift = std::min(static_cast<std::size_t>(src->getNbArmy()), armiesToAirlift);
        src->setNbArmy(src->getNbArmy() - initialArmiesToAirlift);
        dest->setNbArmy(dest->getNbArmy() + initialArmiesToAirlift);
    }
}

bool Airlift::validate()
{
    // check to make sure interactable objects exist
    if (owner == nullptr || src == nullptr || dest == nullptr)
    {
        return false;
    }

    // check if target is owned by player
    if (src->getPlayer()->getPlayerID() != owner->getPlayerID() || 
        dest->getPlayer()->getPlayerID() != owner->getPlayerID())
    {
        return false;
    }

    return false;
}

/********************************************************************
 * Negotiate class function definitions
 ********************************************************************/

Negotiate::Negotiate() : Order(EOrderType::Negotiate, NEGOTIATE_DESC)
{
}

Negotiate::Negotiate(Player* inOwner, Player* inTarget)
{
    this->owner = inOwner;
    this->target = inTarget;
}

Negotiate::~Negotiate() 
{
    this->owner = nullptr;
    this->target = nullptr;
}

Negotiate::Negotiate(const Negotiate &ngo) : Order(ngo)
{
    this->owner = ngo.owner;
    this->target = ngo.target;
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
    if (validate())
    {
        std::vector<Player*> targetPlayerNegotiatees = target->getNotAttackablePlayers();
        if (std::find(targetPlayerNegotiatees.begin(), targetPlayerNegotiatees.end(), owner) == targetPlayerNegotiatees.end())
        {
            target->getNotAttackablePlayers().push_back(owner);
        }
    }
}

bool Negotiate::validate()
{
    if (owner == nullptr || target == nullptr)
    {
        return false;
    }

    if (owner->getPlayerID() == target->getPlayerID())
    {
        return false;
    }

    return true;
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
    this->Notify(*this);
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

std::string OrdersList::stringToLog() {

    std::string toLog = "New order added to the OrdersList : " + this->getOList().back()->getTypeName();
    return toLog;

 }