#include <iostream>
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

int main()
{
    //Create every kind of order
    Deploy *dOrder = new Deploy;
    Advance *aOrder = new Advance;
    Bomb *bOrder = new Bomb;
    Blockade *blOrder = new Blockade;
    Airlift *aiOrder = new Airlift;
    Negotiate *nOrder = new Negotiate;

    //Place orders in the OrderList object

    //Demonstrate all orders
    cout << "Order: " << dOrder->getType() << "\nDescription: " << dOrder->getDescription() << "\nEffect: " << dOrder->getEffect() << "\n"
         << endl;
    cout << "Order: " << aOrder->getType() << "\nDescription: " << aOrder->getDescription() << "\nEffect: " << aOrder->getEffect() << "\n"
         << endl;
    cout << "Order: " << bOrder->getType() << "\nDescription: " << bOrder->getDescription() << "\nEffect: " << bOrder->getEffect() << "\n"
         << endl;
    cout << "Order: " << blOrder->getType() << "\nDescription: " << blOrder->getDescription() << "\nEffect: " << blOrder->getEffect() << "\n"
         << endl;
    cout << "Order: " << aiOrder->getType() << "\nDescription: " << aiOrder->getDescription() << "\nEffect: " << aiOrder->getEffect() << "\n"
         << endl;
    cout << "Order: " << nOrder->getType() << "\nDescription: " << nOrder->getDescription() << "\nEffect: " << nOrder->getEffect() << "\n"
         << endl;

    //Delete orders from memory
    delete dOrder;
    delete aOrder;
    delete bOrder;
    delete blOrder;
    delete aiOrder;
    delete nOrder;

    return 0;
}