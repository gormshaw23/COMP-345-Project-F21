#pragma once
#include <iostream>
#include "map.h"
#include <fstream>
#include <string>
#include <vector>

#include "Player/Player.h"

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
//https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/

bool isNumber(const string& str)
{
    for (char const& c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}
//https://www.delftstack.com/fr/howto/cpp/how-to-determine-if-a-string-is-number-cpp/

//Run trought a vactor of bool and check if all of  them are ste to true;
bool checkVectorForTrue(vector<bool>* vectorToCheck) {

    for (auto x : *vectorToCheck) {
        if (x == false) {
            return false;
        }
    }
    return true;
}

// Run trought a vector of string and check if all element of the vector have a value != ""
bool checkVectorForString(vector<string>* vectorToCheck) {
    for (auto x : *vectorToCheck) {
        if (x == "") {
            return false;
        }
    }
    return true;
}

//Defaut constructor may be usefull later
MapLoader::MapLoader() {
    size = 0;
}


//Copy conctructer
MapLoader::MapLoader(const MapLoader& ml) {
    this->listMap = ml.listMap;
    this->size = ml.size;
}

// Parameter constructer
MapLoader::MapLoader(std::vector<Map*> pLisMap, int psize) {
    this->listMap = pLisMap;
    this->size = psize;
}

bool MapLoader::loadMap(std::string fileName) {

    Map* map = new Map();
    std::string file = "map/"+ fileName + ".map";               //Concatenation of the file name 
    map->creatGraph(file);                                      //Creat the graph even if file is invalid
    cout << "Checking " << fileName << ".map" << endl;
    if (map->validate()) {                                      //add valid map to the list and destroy  invalide one       
        cout << "\n==>"<<fileName<<".map is a valid file\n\n";
        this->getListMap()->push_back(map);
        size++;
    }
    else
    {
        cout << "\n==>" << fileName << ".map is an invalid file\n\n";
        delete map;
        return false;
    }

    map = NULL;
    delete map;
    return true;
}

std::vector<Map*>* MapLoader::getListMap() {
    return &listMap;
}

int MapLoader::getSize() {
    return size;
}


//Basic constructor for map 
Map::Map() {

    this->isValidFile = true;                   //Set the vlidity of the file to true.    
}

//Copy constructor for Map
Map::Map(const Map&  m) {
    listContinents = m.listContinents;
    listTerritory = m.listTerritory;
    isValidFile = m.isValidFile;                   //Set the vlidity of the file to true.    
}

 // Valu constructor  for Map 
Map::Map(vector<Territory*> territory, vector<Continent*> continent, bool valid) {
    listTerritory = territory;
    listContinents = continent;
    isValidFile = valid;
}

bool Map::getValidity() {
    return this->isValidFile;
}


/*
// Function to check and creat all The territory and continent bases from the file passed as a parameter.
*/
bool Map::creatGraph(std::string fileName){
    string valueLine;
    ifstream inputFile(fileName);
    inputFile >> valueLine;                     //Value line will have the value of a string that will be sure to identify the section, or contain some info
    while (valueLine != ("[continents]")) {     //Loop until we get reache the continents section
        inputFile >> valueLine;
        if (inputFile.peek() == EOF) {          //Check if the loop reach End of file without findind the [continent] line.
            this->isValidFile = false;
            return false;
           
        }
    }
    
    std::getline(inputFile, valueLine);
    //Loop to initialise all the country 
    while (valueLine != ("[countries]"))                                       //Loop until you reach the country section.
    {
                                                                               //loop to creat a continents from extacting data frome the file 
        Continent* continentsToCreate = new Continent();                       //Creat an empty continent to be filled later.

        vector<string> splitString;
        splitString = removeDupWord(valueLine);                                //split the string in word.
                             
        if (splitString.size() == 3) {                                         //Validate if  the string have exactly 3 element


            if (isNumber(splitString.at(1))) {                                //check if the second element of the string is an int (for casting safety)
                continentsToCreate->setArmyValue(stoi(splitString.at(1)));    //change all the valus for the element of the continent
                continentsToCreate->setName(splitString.at(0));
                continentsToCreate->setColour(splitString.at(2));
                listContinents.push_back(continentsToCreate);
            }
            else {                                                              
                this->isValidFile = false;
            }
        }
        else {
            if (splitString.at(0) != "") {                                  //Handle empty string in the texte file.
                this->isValidFile = false;
            }
        }

        //check for End of file
        if (inputFile.peek() == EOF) {                                      
            this->isValidFile = false;
            return false;
            //exit;
        }
        getline(inputFile, valueLine);


    };


    getline(inputFile, valueLine);

    /*
    // Loop trought the text file and creat territory object with the info form the file.
    */
    while (valueLine != ("[borders]"))                                      //Loop until the border section is reached. 
    {
                                                                                
        Territory* territoryToCreat = new Territory;                        //Creat the new territtory That will be filled up.

        vector<string> splitString;
        splitString = removeDupWord(valueLine);                             //split the string in word.

        if (splitString.size() >= 5) {                                      //Check if the string have the good amount of element.
            if (isNumber(splitString.at(0)) && isNumber(splitString.at(2)) && isNumber(splitString.at(3)) && isNumber(splitString.at(4))) {     //Check if the necesary number are int (for casting safety).
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
            this->addBorderToTerritory(valueLine);      // call the function to  add the border the the good territory. Value got from the text file.
          
        }

        for (auto i : listTerritory) {                  // Add the nececary Territory to every continents
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
                    
    int a = 0;                              //Us to identify target that will recive the folowing territory
   
    for (string i : splitString) {
        if (i == "") {                      //check for an empty string after [border]

        }
        else if(isNumber(i)){
            if (stoi(i) > listTerritory.size() || stoi(i)<0) {              //check if the number in the border transition point to a real Territory
                this->isValidFile = false;
            }
            else if (a == 0) {                                              //check if its the first element of the vector. The first element indicate what territory will get the next border value.  
                
                    targetCountry = listTerritory.at(stoi(i) - 1);          //get a poiter to the contry that will get is border added
            }
            else {

                toAddCountry = listTerritory.at(stoi(i) - 1);               //get a poiter to the coutry to add
                targetCountry->getBorderList().push_back(toAddCountry);    //add the coutry to the vector list of border
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
    if (targetContinent > listContinents.size() || targetContinent < 0) {      //check if the continent of the territory is valid , if not , the map is invalid. 
        this->isValidFile = false;
    }
    else {
        listContinents.at(targetContinent - 1)->getCountryList()->push_back(toAdd);
    }
}

bool Map::checkValidSubgraph() {
    //for every continent
    vector<bool>* listContinentCheck = new vector<bool>;            //creat an empty vecto that will represent every continent.
    int targetContinent = 0;                                            
    for (auto x : listContinents) {                                 //Loop trought all the continent.
        listContinentCheck->push_back(false);                       //Fill the vector the get an entry for every continent.
        vector<string>* grapContinentCheck = new vector<string>;    //Creat an empty vector of string 

        for (Territory* y : *x->getCountryList()) {          
            grapContinentCheck->push_back("");                      //Fill the string vector to represent every terrtory who is a member of the continent.
        };

        x->traverseTerritory(x->getCountryList()->at(0),grapContinentCheck,x->getCountryList()->at(0)->getContinent());     //Call the function to traverse the subgraph.
        cout << "Done checking for continent : " << x->getName() << endl;

        if (checkVectorForString(grapContinentCheck)) {             //Call the function to check if every member of the territory could be reached.
            listContinentCheck->at(targetContinent) = true;         //indicate that this subgraph is valid
        }
        targetContinent++;
        delete grapContinentCheck;
            
    }

    if (checkVectorForTrue(listContinentCheck)) {                   //Check that every subgraph are valid.
        return true;
    }
    else {
        return false;
    }
   
}

bool Map::checkValidGraph() {
    vector<bool>* testGraph = new vector<bool>;                     //Empty bool vector that will represent every territory of the map.
    for (auto x : listTerritory) {                                  //demo loop to creat a vector of the size of the number of territory
        testGraph->push_back(false);
    }
    this->traverseTerritory(listTerritory.at(0), testGraph);        //Call the recusive traversal algorythm to check if the graph is valid 
    if (checkVectorForTrue(testGraph))                              //Check if the graph is valid.
    {
        return true;
    }
    else {
        this->isValidFile = false;
        return false;
    }
    
    delete testGraph;
}

/*
// Will loop trought every territory and check if they are only member of one continent.
*/
bool Map::checkContinents() {
    for (auto x : listTerritory) {                              //Loop trought every Territory.
       
        bool* found = new bool;                                 //bool to keep track if the territory was found in a continent.
        *found = false;
        for (auto w : listContinents) {                         //Loop trought every continent of the map and check if the territory is found in their list.
            if (!w->checkTerritory(x->getID(), found)) {        //If the territory is member of multiple continent, it will retrun false.
                return false;
            }
            
        }
    }
    return true;
}

/*
// check if the territory is found in multiple continent 
*/
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

/*
// Funtion that check it the map is a valid graph , if every continent are valid subgraph, and if every territory are only member of a single continent.
*/
bool Map::validate() {
    if (this->getValidity()) {
        if (this->checkValidGraph()) {
            cout << "This map is a valid graph" << endl;
            if (this->checkValidSubgraph()) {
                cout << "This Map have valid Continent as sub-graph." << endl;
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
                cout << "This Map does not have valid sub-graph." << endl;
                return false;
            }
        }
        else {
            cout << "This map is not a valid graph." << endl;
            return false;
        }
    }
    else {
        cout << "This file is not valid at all,";
        return false;
    }
    
}

/*
//  Function to travese all the adjacent node of a territory. 
*/
bool Map::traverseTerritory(Territory* territory, vector<bool>* visitedTerritory) {
    for (auto T : territory->getBorderList()) {                //Loop trought all the adjacent territory of the current territory
        if (visitedTerritory->at(T->getID() - 1) == false) {    //check if the territory was already visited
            visitedTerritory->at(T->getID() - 1) = true;        //if not , mark it ass visited and call the recursive function on it 
            traverseTerritory(T, visitedTerritory);
        }        
    }
    return true;
}

/*
* Retreives territory ids
* This function will help facilitating the retrieval of
* territory objects by searching a territory by its id.
*/
std::vector<int> Map::getTerritoryIds() {
    vector<int> territoryIds;
    for (Territory* t : this->listTerritory) {
        territoryIds.push_back(t->getID());
    }
    return territoryIds;
}

/*
//Recusive function to only check the subgraph of a continent
*/
bool Continent::traverseTerritory(Territory* territory, vector<string>* visitedTerritory,int continent) {
    int size = visitedTerritory->size();                        //saving the size of the numbr of territory that are suppose to be in the continent.
    bool finishCheck = false;
    for (int i = 0; i < size && finishCheck== false; i++ ) {    //Loop trought the vector containing the name of every terrtory visited.
        if (visitedTerritory->at(i) == "") {                    //if en empty space is found, ad the name the territory getting check in the vector.
            visitedTerritory->at(i) = territory->getName();
            finishCheck = true;
        }
        else if (visitedTerritory->at(i) == territory->getName()) { //if the territory was already inside , do nothing
            finishCheck = true;
        }   
    }
        
    for (auto T: territory->getBorderList()) {                    //Will loop trough all the territory of the adgecency list.
        if (T->getContinent() == continent) {                      // Check if the territory is member of the good continent. 
            bool innerFinishCheck = false;
            for (int i = 0; i < size && innerFinishCheck == false; i++) { //loop trough all the adgecent territory and call the function resursively if needed
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
//continent constructor
Continent::Continent() {
    armyValue = 0;
    string name;
    string colour;
}

//Continent copy constructor 
Continent::Continent(const Continent& c ) {
    armyValue = c.armyValue;
    string name = c.name;
    string colour = c.colour;
    listMemberTerritory = c.listMemberTerritory;
}

// Continent parameters contructor 
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


//Territory default constructor
Territory::Territory() {
     id=0;
     continent=0;
     posx=0;
     posy=0;
     player = new Player();
     nbArmy = 0;
     name="null";
}

//Territory  copy constructor
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

//Territory parameter contructor.
Territory::Territory
(
    int pId, 
    int pContinent, 
    Player* pPlayer, 
    int pNbArmy, 
    int pPosx, 
    int pPosy, 
    std::string pName, 
    std::vector<Territory*> pListTerritory
) 
{
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

void Territory::setArmies(int inArmies)
{
    this->nbArmy = inArmies;
}

void Territory::setOwner(Player* inOwner)
{
    player = inOwner;
}

void Territory::setPosx(int posx) {
    this->posx = posx;
}

void Territory::setPosy(int posy) {
    this->posy = posy;
}

int Territory::getID() const { return id; };
int Territory::getContinent() { return continent; };
int Territory::getPosx() { return posx; };
int Territory::getPosy() { return posy; };
std::string Territory::getName() { return name; };
Player* Territory::getPlayer() { return player; };
int Territory::getNbArmy() { return nbArmy; };
std::vector<Territory*>& Territory::getBorderList() { return this->listBorder; };



//================= Start of the section for the destructor =============================\\

Continent::~Continent() {

    listMemberTerritory.~vector();
}

Territory::~Territory() {
    listBorder.~vector();
    
}

bool Territory::operator==(const Territory& inRHS) const
{
    return this->getID() == inRHS.getID();
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

MapLoader::~MapLoader() {
    for (auto y : listMap) {
        delete y;
    }
    listMap.~vector();
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
    for (auto x : t.getBorderList()) {
        cout <<" "<< x->getName()<<endl;
    }
    return out;
}


