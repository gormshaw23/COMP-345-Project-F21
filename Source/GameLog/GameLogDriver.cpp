// observerPattern.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include "LoggingObserver.h"
#include "Engine/GameEngine.h"
#include "Order/Orders.h"
#include <iostream>
#include <list>

#include <fstream>




int main()
{
    std::list<Subject*>* _listSubject = new std::list<Subject*>;

    cout << "Welcome to Warzone!\n";
    cout << "Please refer the table below for the command.\n\n"
        "    state                              command option(s)\n"
        "GAME_STATE_START                   loadmap\n"
        "GAME_STATE_MAP_LOAD                loadmap / validatemap\n"
        "GAME_STATE_MAP_VALIDATED           addplayer\n"
        "GAME_STATE_PLAYERS_ADDED           addplayer / assigncountries\n"
        "GAME_STATE_ASSIGN_REINFORCEMENT    issueorder\n"
        "GAME_STATE_ISSUE_ORDERS            issueorder / endissueorders\n"
        "GAME_STATE_EXECUTE_ORDERS          endexecorders / execorder / win\n"
        "GAME_STATE_WIN                     play / end\n" << endl;

    //Creat the game engine
    GameEngine gameobj(_listSubject);
    //little fix
    GameEngine* obj = &gameobj;
    _listSubject->push_back(obj);

    

    //creation of the order
    Deploy* dOrder = new Deploy;
    Advance* aOrder = new Advance;
    _listSubject->push_back(aOrder);
    _listSubject->push_back(dOrder);


    //creation on an orderd list
    OrdersList ol;
    OrdersList* pol = &ol;
    _listSubject->push_back(pol);


    //creation of the log Observer
    static LogObserver* loglog = new  LogObserver(_listSubject);


   
    ol.add(dOrder);
    ol.add(aOrder);
   

    //execution of the order 
    cout << "\nExecuting orders" << endl;
    for (Order* o : ol.getOList())
    {
        if (o->validate())
            o->execute();
        ol.remove(0); //Removes executed Order
    }

    //destrucion of the order list to show that detroying a subject do not crach the observer
    ol.~OrdersList();


    gameobj.game_run();
    cout << "Good Game!\nGood Bye!" << endl;

    return 0;

}
