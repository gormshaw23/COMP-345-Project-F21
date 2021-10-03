#include <iostream>
#include <list>
#include <string>
using namespace std;

#include "Orders.h"

Orders::Orders()
{
    type = new string("");
    description = new string("");
    effect = new string("");
}

Orders::Orders(string typeValue, string descValue, string effectValue)
{
    type = new string(typeValue);
    description = new string(descValue);
    effect = new string(effectValue);
}

Orders::~Orders(){
    delete type;
    delete description;
    delete effect;
}

Deploy::Deploy() : Orders("deploy", "place some armies on one of the current player’s territories.", "")
{
}

Deploy::~Deploy(){}

Advance::Advance() : Orders("advance",
                            "move some armies from one of the current player’s territories (source) to an adjacent territory (target). If the target territory belongs to the current player, the armies are moved to the target territory. If the target territory belongs to another player, an attack happens between the two territories",
                            "")
{
}

Advance::~Advance(){}

Bomb::Bomb() : Orders("bomb", "destroy half of the armies located on an opponent’s territory that is adjacent to one of the current player’s territories", "")
{
}

Bomb::~Bomb(){}

Blockade::Blockade() : Orders("blockade", "triple the number of armies on one of the current player’s territories and make it a neutral territory", "")
{
}

Blockade::~Blockade(){}

Airlift::Airlift() : Orders("airlift", "advance some armies from one of the current player’s territories to any another territory", "")
{
}

Airlift::~Airlift(){}

Negotiate::Negotiate() : Orders("negotiate", "prevent attacks between the current player and another player until the end of the turn", "")
{
}

Negotiate::~Negotiate(){}

bool Orders::validate()
{
    bool isValid = false;
    string types[6] = {"deploy", "advance", "bomb", "blockade", "airlift", "negotiate"};
    
    //Check if the Order is a subclass of the Order class

    //Check if the type corresponds to a valid order type


    return isValid;
}

void Orders::execute()
{
    //Check if the order is valid
    if(validate()){
        //TODO Execute order and print effect
    }
}

string Orders::getType()
{
    return *type;
}

string Orders::getDescription()
{
    return *description;
}

string Orders::getEffect()
{
    return *effect;
}

void Orders::setType(string newType)
{
    *type = newType;
}

void Orders::setDescription(string newDescription)
{
    *description = newDescription;
}

void Orders::setEffect(string newEffect)
{
    *effect = newEffect;
}