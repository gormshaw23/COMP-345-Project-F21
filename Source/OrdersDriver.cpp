/**
 * The driver file for the OrdersList and Order classes
 * and its subclasses
 * Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate.
 * 
 * @author Mark Nasol
 */

#include <iostream>

using std::cout;
using std::endl;

#include "Order/Orders.h"

#ifdef ORDERSDRIVER
int main()
{
     //Create every kind of order and adding them to the OrderList
     Deploy *dOrder = new Deploy;
     Advance *aOrder = new Advance;
     Bomb *bOrder = new Bomb;
     Blockade *blOrder = new Blockade;
     Airlift *aiOrder = new Airlift;
     Negotiate *nOrder = new Negotiate;

     cout << "\nAdding orders to OrderList" << endl;
     OrdersList ol;
     ol.add(dOrder);
     ol.add(aOrder);
     ol.add(bOrder);
     ol.add(blOrder);
     ol.add(aiOrder);
     ol.add(nOrder);
     cout << ol;

     //Testing move function
     cout << "\nMoving first order with the last order" << endl;
     ol.move(0, 5);
     cout << ol;

     cout << "\nMoving 4th order with 2nd order" << endl;
     ol.move(3, 1);
     cout << ol;

     //Testing remove function
     cout << "\nRemoving last order" << endl;
     ol.remove(5);
     cout << ol;

     //Testing validate() and execute() functions
     //The orders are then removed
     cout << "\nExecuting orders" << endl;
     for (Order *o : ol.getOList())
     {
          if (o->validate())
               o->execute();
          ol.remove(0); //Removes executed Order
     }
     cout << ol;

     return 0;
}
#endif