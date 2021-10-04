/**
 * The driver file for the Order class and its subclasses
 * Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate.
 * The driver file also has the OrdersList class.
 * 
 * @author Mark Nasol
 */

#include <iostream>
#include <vector>
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

     // if(dOrder->validate())
     //      cout << "The given order is valid\n" << endl;
     // else
     //      cout << "The given order is invalid\n" << endl;

     //     Order *oOrder = new Deploy;

     //TODO Place orders in the OrderList object

     // vector<Order*> vOrders;
     // vOrders.push_back(dOrder);
     // vOrders.push_back(aOrder);
     // vOrders.push_back(bOrder);
     // vOrders.push_back(blOrder);
     // vOrders.push_back(aiOrder);
     // vOrders.push_back(nOrder);

     // for(int i = 0; i < vOrders.size(); i++){
     //      vOrders[i]->execute();
     // }

     //Delete orders from memory
     delete dOrder;
     delete aOrder;
     delete bOrder;
     delete blOrder;
     delete aiOrder;
     delete nOrder;

     return 0;
}