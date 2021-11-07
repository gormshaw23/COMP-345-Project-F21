// observerPattern.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include "LoggingObserver.h"
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

    LogObserver* loglog = new  LogObserver(_listSubject);

    dumdum->changeName("paule");
    domdum->changeName("Mario");



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
