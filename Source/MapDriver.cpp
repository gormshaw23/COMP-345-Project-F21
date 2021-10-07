#include <iostream>
#include <fstream>
#include <string>
#include <vector> //for testing purpus 

#include "map.h"
using namespace std;



int main()
{
     Map* map = new Map();
    // map->creatGraph("simpsons_world.map");
     map->creatGraph("map/myImortal.map");
     
     map->check();

     /*
     //Demo constructor Territory
     std::cout << "Befor the copy constructor for Territory";
     Territory& ref = *map->listTerritory.at(0);
     Territory* paramT = new Territory(ref.getID(), ref.getContinent(), ref.getPlayer(), ref.getNbArmy(), ref.getPosx(), ref.getPosy(), ref.getName(), *ref.getBorderList());
     Territory* copyT = new Territory(*paramT);
     std::cout <<"Copy constructor : " << *copyT << endl;
     std::cout << "Reference constructor : " << *paramT << endl;

     delete paramT;
     delete copyT;
     */

     /*
     //Demo constructor Continent
     std::cout << "Befor the constructor for Continent";
     Continent& ref = *map->listContinents.at(0);
     Continent* paramC = new Continent(ref.getArmyValu(),ref.getName(),ref.getColour(),*ref.getCountryList());
     Continent* copyC = new Continent(*paramC);

     std::cout << "Copy constructor : " << *copyC << endl;
     std::cout << "Reference constructor : " << *paramC << endl;

     delete copyC;
     delete paramC;
     */

     /*
     //Demo Map
     Map* paramM = new Map(map->listTerritory, map->listContinents, map->getValidity());
     Map * copyM = new Map(*map);

      //cout << *paramM;
     //cout << *copyM;
     delete paramM;
     delete copyM;
     */


    // copyMap->listTerritory.at(41)->setName("Hi Mark");
    

 
}