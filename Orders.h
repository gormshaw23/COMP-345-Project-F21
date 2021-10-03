#ifndef ORDERS_H
#define ORDERS_H

#include <string>
using namespace std;

class Orders
{
public:
    Orders();
    ~Orders();

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
    string * type;
    string * description;
    string * effect;
};

class OrdersList
{
public:
    OrdersList();
    ~OrdersList();
    void move();
    void remove();
};

class Deploy : public Orders
{
public:
    Deploy();
    ~Deploy();
};

class Advance : public Orders
{
public:
    Advance();
    ~Advance();
};

class Bomb : public Orders
{
public:
    Bomb();
    ~Bomb();
};

class Blockade : public Orders
{
public:
    Blockade();
    ~Blockade();
};

class Airlift : public Orders
{
public:
    Airlift();
    ~Airlift();
};

class Negotiate : public Orders
{
public:
    Negotiate();
    ~Negotiate();
};

#endif