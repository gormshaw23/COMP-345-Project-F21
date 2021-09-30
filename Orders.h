#ifndef ORDERS_H
#define ORDERS_H

#include <string>
using namespace std;

class Orders{
    public:
        Orders();
        Orders(string typeValue, string descValue, string effectValue);
        bool validate();
        void execute();
        string getType();
        string getDescription();
        string getEffect();
        void setType(string newType);
        void setDescription(string newDescription);
        void setEffect(string newEffect);
    private:
        string type;
        string description;
        string effect;
};


class OrdersList{

};

class Deploy: public Orders{

};

Deploy::Deploy() : Orders("deploy", "place some armies on one of the current player’s territories.", "")
{}

class Advance: public Orders{

};

Advance::Advance() : Orders("advance",
 "move some armies from one of the current player’s territories (source) to an adjacent territory (target). If the target territory belongs to the current player, the armies are moved to the target territory. If the target territory belongs to another player, an attack happens between the two territories",
  "")
{}

class Bomb: public Orders{

};

Bomb::Bomb() : Orders("bomb", "destroy half of the armies located on an opponent’s territory that is adjacent to one of the current player’s territories", "")
{}

class Blockade: public Orders{

};

Blockade::Blockade() : Orders("blockade", "triple the number of armies on one of the current player’s territories and make it a neutral territory", "")
{}

class Airlift: public Orders{

};

Airlift::Airlift() : Orders("airlift", "advance some armies from one of the current player’s territories to any another territory", "")
{}

class Negotiate: public Orders{

};

Negotiate::Negotiate() : Orders("negotiate", "prevent attacks between the current player and another player until the end of the turn", "")
{}

#endif