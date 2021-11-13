#pragma once
#include <iostream>
#include <vector>

class Player;

class Territory {
private:
	int id;
	int continent;
	int posx;
	int posy;
	Player* player = nullptr;
	int nbArmy;
	std::string name;
	
	std::vector<Territory*> listBorder;
public:
	void setId(int inID);
	void setContinent(int inContinentID);
	void setPosx(int inPosX);
	void setPosy(int inPosY);
	void setName(std::string inName);
	void setArmies(int inArmies);
	void setOwner(Player* inOwner);

	int getID() const;
	int getContinent();
	int getPosx();
	int getPosy();
	std::string getName();
	Player* getPlayer();
	int getNbArmy();
	std::vector<Territory*>& getBorderList();

	friend std::ostream& operator << (std::ostream& out, Territory& t);
	//friend std::istream& operator >> (std::istream& in, Territory& t);

	Territory();
	Territory(const Territory &);
	Territory
	(
		int inTerritoryId, 
		int inContinentID, 
		Player* inOwner,
		int inArmies, 
		int inPosX,
		int inPosY, 
		std::string inProvName, 
		std::vector<Territory*> inBorder
	);
	~Territory();

	bool operator==(const Territory& inRHS) const;
};


class Continent {
private:

	int armyValue;
	std::string name;
	std::string colour;
	std::vector<Territory*> listMemberTerritory;

public:
	bool checkTerritory(int, bool*);

	void setArmyValue(int);
	void setName(std::string);
	void setColour(std::string);
	bool traverseTerritory(Territory*, std::vector<std::string>*, int);	//int is for the continent ID

	friend std::ostream& operator << (std::ostream& out, Continent& c);
	friend std::istream& operator >> (std::istream& in, Continent& c);
	
	int getArmyValu();
	std::string getColour();
	std::string getName();
	std::vector<Territory*>* getCountryList();

	Continent();
	Continent(const Continent &);
	Continent(int, std::string, std::string, std::vector<Territory*>);
	~Continent();

	//Destructor : to implements
};



class Map {
public:
	std::vector<Continent*> listContinents;
	std::vector<Territory*> listTerritory;

	bool creatGraph(std::string);
	void addBorderToTerritory(std::string);
	void addTerritoryToContinent(Territory*);
	
	bool validate();
	bool checkValidGraph();				//check if the the map is a graph
	bool checkValidSubgraph();			//check if the continents are valid subgraph
	bool checkContinents();				//check if every contri only belong to a sigle continents.
	bool traverseTerritory(Territory*,std::vector<bool>*);

	bool getValidity();

	std::vector<int> getTerritoryIds(); //Retrieve territory ids
	
	friend std::ostream& operator << (std::ostream& out, Map& m);
	friend std::istream& operator >> (std::istream& in, Map& m);
	
	//contructor section
	Map();
	Map(std::vector<Territory*>,std::vector<Continent*>,bool);
	Map(const Map&);
	~Map();

private:
	bool isValidFile ;
	
	//Continent* arrayContinent[];
};


class MapLoader {
public:
	bool loadMap(std::string);
	int getSize();
	std::vector<Map*>* getListMap();

	MapLoader();
	MapLoader(std::vector<Map*>, int);
	MapLoader(const MapLoader&);
	~MapLoader();
private:
	std::vector<Map*> listMap;
	int size;
};




