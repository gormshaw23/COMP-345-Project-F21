#include <iostream>
using namespace std;

#include "Orders.cpp"

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