#pragma once
#include <iostream>
#include "map.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;


vector<string> removeDupWord(string str)
{
    string word = "";
    vector<string> splitstring;
    for (auto x : str)
    {
        if (x == ' ')
        {
            //cout << word << endl;
            splitstring.push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    splitstring.push_back(word);
    return splitstring;
}

bool isNumber(const string& str)
{
    for (char const& c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}
//https://www.delftstack.com/fr/howto/cpp/how-to-determine-if-a-string-is-number-cpp/

bool checkVectorForTrue(vector<bool>* vectorToCheck) {
    for (auto x : *vectorToCheck) {
        if (x == false) {
            return false;
        }
    }
    return true;
}

bool checkVectorForString(vector<string>* vectorToCheck) {
    for (auto x : *vectorToCheck) {
        if (x == "") {
            return false;
        }
    }
    return true;
}


Map::Map() {

    this->isValidFile = true;                   //Set the vlidity of the file to true.    
}

Map::Map(const Map&  m) {
    listContinents = m.listContinents;
    listTerritory = m.listTerritory;
    isValidFile = m.isValidFile;                   //Set the vlidity of the file to true.    
}

 // To check
Map::Map(vector<Territory*> territory, vector<Continent*> continent, bool valid) {
    listTerritory = territory;
    listContinents = continent;
    isValidFile = valid;
}

bool Map::getValidity() {
    return this->isValidFile;
}



bool Map::creatGraph(std::string fileName){
    string valueLine;
    ifstream inputFile(fileName);
    inputFile >> valueLine;                     //Value line will have the value of a string that will be sure to identify the section, or contain some info
    while (valueLine != ("[continents]")) {     //Loop until we get reache the continents section
        inputFile >> valueLine;
        if (inputFile.peek() == EOF) {
            this->isValidFile = false;
            return false;
            //exit;
        }
    }
    /*To Add check fo end of file  to detect invalid file*/




    std::getline(inputFile, valueLine);
    //Loop to initialise all the country 
    while (valueLine != ("[countries]"))                                       //loop to creat a continents from extacting data frome the file 
    {
        Continent* continentsToCreate = new Continent();

        vector<string> splitString;
        splitString = removeDupWord(valueLine);                                //split the string in word.
        splitString.size();
        if (splitString.size() == 3) {


            if (isNumber(splitString.at(1))) {                                //check if tempoValue is a string
                continentsToCreate->setArmyValue(stoi(splitString.at(1)));
                continentsToCreate->setName(splitString.at(0));
                continentsToCreate->setColour(splitString.at(2));
                listContinents.push_back(continentsToCreate);
            }
            else {
                this->isValidFile = false;
            }
        }
        else {
            if (splitString.at(0) != "") {
                this->isValidFile = false;
            }
        }

        if (inputFile.peek() == EOF) {
            this->isValidFile = false;
            return false;
            //exit;
        }
        getline(inputFile, valueLine);


    };


    getline(inputFile, valueLine);

    while (valueLine != ("[borders]"))
    {
        Territory* territoryToCreat = new Territory;

        vector<string> splitString;
        splitString = removeDupWord(valueLine);      //split the string in word.

        if (splitString.size() >= 5) {
            if (isNumber(splitString.at(0)) && isNumber(splitString.at(2)) && isNumber(splitString.at(3)) && isNumber(splitString.at(4))) {
                territoryToCreat->setId(stoi(splitString.at(0)));
                territoryToCreat->setName(splitString.at(1));
                territoryToCreat->setContinent(stoi(splitString.at(2)));
                territoryToCreat->setPosx(stoi(splitString.at(3)));
                territoryToCreat->setPosy(stoi(splitString.at(4)));
                listTerritory.push_back(territoryToCreat);
            }
            else {
                this->isValidFile = false;
            }

        }
        else {

            if (splitString.at(0) != "") {

                this->isValidFile = false;
            }
        }

        if (inputFile.peek() == EOF) {
            this->isValidFile = false;
            return false;
        }
        getline(inputFile, valueLine);     //get the next line in the text file.

    };

    if (this->isValidFile == true) {                    //check if the text file is valid , if it is, finalise the graph.
        while (std::getline(inputFile, valueLine)) {
            this->addBorderToTerritory(valueLine);
          
        }

        for (auto i : listTerritory) {
            addTerritoryToContinent(i);

        }

    }
    else {
        cout << "The value in this file are not valid please folow the document structure" << endl;
    }

    return true;
};

/*
    Recive a string of number representing the border. Add poiter to country object in the "listBorder" of a country object 
*/
void Map::addBorderToTerritory(string line) {
    vector<string> splitString;
    splitString = removeDupWord(line);      //split the string in word.
    Territory* targetCountry = nullptr;
    Territory* toAddCountry = nullptr;
    int nbCountry = 0;
    int a = 0;
    for (string i : splitString) {
        if (i == "") {                      //check for an empty string after [border]

        }
        else if(isNumber(i)){
            if (stoi(i) > listTerritory.size() || stoi(i)<0) { //check if the number in the border transition point to a real Territory
                this->isValidFile = false;
            }
            else if (a == 0) {              //check if its the first element of the vector. The first element indicate what territory will get the next border value.  
                
                    targetCountry = listTerritory.at(stoi(i) - 1);    //get a poiter to the contry that will get is border added
            }
            else {

                toAddCountry = listTerritory.at(stoi(i) - 1);     //get a poiter to the coutry to add
                targetCountry->getBorderList()->push_back(toAddCountry);    //add the coutry to the vector list of border
            }
            a++;
        }
        else {
            this->isValidFile = false;
        }
    }
  
}

void Map::addTerritoryToContinent(Territory* toAdd) {
    
    int targetContinent = toAdd->getContinent();
    if (targetContinent > listContinents.size() || targetContinent < 0) {      //check if the continent of the territory is valid , if not , the map is invalid 
        this->isValidFile = false;
    }
    else {
        listContinents.at(targetContinent - 1)->getCountryList()->push_back(toAdd);
    }
}

bool Map::checkValidSubgraph() {
    //for every continent
    vector<bool>* listContinentCheck = new vector<bool>;
    int targetContinent = 0;
    for (auto x : listContinents) {
        listContinentCheck->push_back(false);
        vector<string>* grapContinentCheck = new vector<string>;

        for (Territory* y : *x->getCountryList()) {
            //cout << "in the loop for continent check : " << y->getName()<<endl;
            grapContinentCheck->push_back("");
        };

        x->traverseTerritory(x->getCountryList()->at(0),grapContinentCheck,x->getCountryList()->at(0)->getContinent());
        cout << "Done checking for continent : " << x->getName() << endl;

        if (checkVectorForString(grapContinentCheck)) {
            listContinentCheck->at(targetContinent) = true;
        }
        targetContinent++;
        delete grapContinentCheck;
            
    }

    if (checkVectorForTrue(listContinentCheck)) {
        return true;
    }
    else {
        return false;
    }
   
}

bool Map::checkValidGraph() {
    vector<bool>* testGraph = new vector<bool>;
    for (auto x : listTerritory) {        //demo loop to creat a vector of the size of the number of territory
        testGraph->push_back(false);
    }
    this->traverseTerritory(listTerritory.at(0), testGraph);
    if (checkVectorForTrue(testGraph))
    {
        return true;
    }
    else {
        this->isValidFile = false;
        return false;
    }
    
    delete testGraph;
}


bool Map::checkContinents() {
    for (auto x : listTerritory) {
       
        bool* found = new bool;
        *found = false;
        for (auto w : listContinents) {
            if (!w->checkTerritory(x->getID(), found)) {
                return false;
            }
            
        }
    }
    return true;
}

bool Continent::checkTerritory(int id , bool* found) {
    for (auto y : listMemberTerritory) {
        if (y->getID() == id) {
            if (*found == false) {
                *found = true;
            }
            else {
                return false;
            }
        }
    }
    return true;
}

bool Map::check() {
    if (this->getValidity()) {
        if (this->checkValidGraph()) {
            cout << "This map is a valid graph" << endl;
            if (this->checkValidSubgraph()) {
                cout << "This Map have valid Continent as sub-graph" << endl;
                if (checkContinents()) {
                    cout << "This Map have valid continent." << endl;
                    return true;
                }
                else {
                    cout << "This map does not have valid continent.";
                    return false;
                }

            }
            else {
                cout << "This Map does not have valid subgraph" << endl;
                return false;
            }

        }
        else {
            cout << "This map is not a valid graph" << endl;
            return false;
        }

    }
    else {
        cout << "This file is not valsid at all";
        return false;
    }
    
    
    
    
}

/*
//  Function to travese all the adjacent node of a territory. 
*/
bool Map::traverseTerritory(Territory* territory, vector<bool>* visitedTerritory) {
    for (auto T : *territory->getBorderList()) {                //Loop trought all the adjacent territory of the current territory
        if (visitedTerritory->at(T->getID() - 1) == false) {    //check if the territory was already visited
            visitedTerritory->at(T->getID() - 1) = true;        //if not , mark it ass visited and call the recursive function on it 
            traverseTerritory(T, visitedTerritory);
        }
        
    }
    return true;
   
}

bool Continent::traverseTerritory(Territory* territory, vector<string>* visitedTerritory,int continent) {
    int size = visitedTerritory->size();
    bool finishCheck = false;
    for (int i = 0; i < size && finishCheck== false; i++ ) {
        if (visitedTerritory->at(i) == "") {
            visitedTerritory->at(i) = territory->getName();
            finishCheck = true;
        }
        else if (visitedTerritory->at(i) == territory->getName()) {
            finishCheck = true;
        }
        
    }
        
    for (auto T: *territory->getBorderList()) {
        if (T->getContinent() == continent) {
            bool innerFinishCheck = false;
            for (int i = 0; i < size && innerFinishCheck == false; i++) {
                if (visitedTerritory->at(i) == "") {
                    traverseTerritory(T, visitedTerritory, continent);
                    innerFinishCheck = true;
                }
                else if (visitedTerritory->at(i) == T->getName()) {
                    innerFinishCheck = true;
                }

            }
           
            
        }
    }
    
    return true;
}

Continent::Continent() {
    armyValue = 0;
    string name;
    string colour;
}

Continent::Continent(const Continent& c ) {
    armyValue = c.armyValue;
    string name = c.name;
    string colour = c.colour;
    listMemberTerritory = c.listMemberTerritory;
}

Continent::Continent(int armyValu, std::string name, std::string colour, std::vector<Territory*> listTerritory) {
    this->armyValue = armyValu;
    this->name = name;
    this->colour = colour;
    this->listMemberTerritory = listTerritory;

}
void Continent::setArmyValue(int value) {
    this->armyValue = value;
}

void Continent::setName(string name) {    
    this->name = name;
}

void Continent::setColour(string value) {
    this->colour = value;

}


string Continent::getName() {
    string toReturn = this->name;
    return toReturn;
}

int Continent::getArmyValu() {
    return this->armyValue;
}

string Continent::getColour() {
    string toReturn = this->colour;
    return toReturn;
}

vector<Territory*>* Continent::getCountryList(){
    return &this->listMemberTerritory;
}



Territory::Territory() {
     id=0;
     continent=0;
     posx=0;
     posy=0;
     player = NULL;
     nbArmy = 0;
    
     name="null";

}

Territory::Territory(const Territory &t) {
    id = t.id;
    name = t.name;
    continent = t.continent;
    player = t.player;
    nbArmy = t.nbArmy;
    posx = t.posx;
    posy = t.posy;
    listBorder = t.listBorder;
}

Territory::Territory(int pId, int pContinent, int pPlayer, int pNbArmy, int pPosx, int pPosy, std::string pName, std::vector<Territory*> pListTerritory) {
    id = pId;
    name = pName;
    continent = pContinent;
    player = pPlayer;
    nbArmy = pNbArmy;
    posx = pPosx;
    posy = pPosy;
    listBorder = pListTerritory;
};

void Territory::setContinent(int continent) {
    this->continent = continent;
     
}

void Territory::setId(int id) {
    this->id = id;
}

void Territory::setName(string name) {
    this->name = name;
}

void Territory::setPosx(int posx) {
    this->posx = posx;
}

void Territory::setPosy(int posy) {
    this->posy = posy;
}


//================= Start of the section for the destructor =============================\\

Continent::~Continent() {

    listMemberTerritory.~vector();
}

Territory::~Territory() {
    listBorder.~vector();
    
}


Map::~Map() {
    for (auto y : listContinents) {
        delete y;
    }
    for (auto x : listTerritory) {
        delete x;
    }
    listContinents.~vector();
    listTerritory.~vector();
}


//================= Start of the section for stream insertion opperator =============================\\

std::ostream& operator << (std::ostream& out, Map& m) {
    std::cout  <<"This map posses the folowing continents :"<< std::endl;
    for (auto x : m.listContinents) {
        cout << *x;
    }
    return out;
}

std::ostream& operator << (std::ostream& out, Continent& c) {
    std::cout <<  "\nThe continent have the following value:\n Name:"<<c.name<<"\n Colour :"<<c.colour<<"\n Army value: "<<c.armyValue<<endl;
    for (auto x : c.listMemberTerritory) {
        cout << *x;
    }
    return out;
}

std::ostream& operator << (std::ostream& out, Territory& t) {
    std::cout << "\nThis territory have the following value:\n Name:" << t.getName()<< "\n Id: " << t.getID() << "\n Player:  " << t.getPlayer() << endl;
    std::cout<<" Nb Army :"<<t.getNbArmy() << "\n Pos X : " << t.getPosx() << "\n Pos Y : " << t.getPosy() << endl;
    std::cout << "The folowing territory are connected to it :\n";
    for (auto x : *t.getBorderList()) {
        cout <<" "<< x->getName()<<endl;
    }
    return out;
}


