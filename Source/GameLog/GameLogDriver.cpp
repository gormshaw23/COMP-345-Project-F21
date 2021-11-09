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

    dummy* dumdum = new dummy();
    _listSubject->push_back(dumdum);

    dummy* damdum = new dummy();
    _listSubject->push_back(damdum);

    dummy* domdum = new dummy();
    _listSubject->push_back(domdum);

    dumdum->changeName("paule");
    domdum->changeName("Mario");
   



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
    GameEngine gameobj;
    GameEngine* obj = &gameobj;
    _listSubject->push_back(obj);

    Deploy* dOrder = new Deploy;
    Advance* aOrder = new Advance;
    _listSubject->push_back(aOrder);
    _listSubject->push_back(dOrder);


    OrdersList ol;
    OrdersList* pol = &ol;
    _listSubject->push_back(pol);


    LogObserver* loglog = new  LogObserver(_listSubject);

    ol.add(dOrder);
    ol.add(aOrder);

    cout << "\nExecuting orders" << endl;
    for (Order* o : ol.getOList())
    {
        if (o->validate())
            o->execute();
        ol.remove(0); //Removes executed Order
    }




    gameobj.game_run();
    cout << "Good Game!\nGood Bye!" << endl;
    
    
   

    return 0;

    // test to reset file at for new game 
    /*
    std::ofstream outfile;
    outfile.open("log.txt");
    outfile << "done";
    outfile.close();

    */
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
