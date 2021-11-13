#include <iostream>
#include <fstream>
#include <string>
#include <vector> //for testing purpus 

#include "Map/map.h"
using namespace std;

#ifdef MAPDRIVER
int main()
{
   
    MapLoader* mapL = new MapLoader();
    mapL->loadMap("canada");
    mapL->loadMap("caribbean");
    mapL->loadMap("chutes");
    mapL->loadMap("simpsons_world");
    mapL->loadMap("startrek");
    mapL->loadMap("myImortal");

    
    cout << "MapLoader contain " << mapL->getSize() << " valid map. When you will press anny key, the third Map will be printed." << endl;
    
    std::string comand;
    cin >> comand;
    cout << *mapL->getListMap()->at(2);
    
     /*
     //Demo constructor Territory
     std::cout << "Befor the copy constructor for Territory";
     Territory& ref = *mapL->getListMap()->at(2)->listTerritory.at(0);
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
     Continent& ref = *mapL->getListMap()->at(2)->listContinents.at(0);
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
}
#endif