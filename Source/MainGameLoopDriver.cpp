/**
 * The driver file for the Main game loop
 *
 * @author Mark Nasol
 */
#pragma once
#include <vector>
using std::vector;
#include "Player/Player.h"
#include "Engine/GameEngine.h"
#include "Map/map.h"
#include "Cards/Cards.h"
#include "GameLog/LoggingObserver.h"
#include "CommandProcessor/CommandProcessor.h"

#ifdef GAMELOOPDRIVER
int main() {
  /*
	//Load map
	MapLoader* mapL = new MapLoader();
	mapL->loadMap("canada");
	Map* map = mapL->getListMap()->at(0);

	//Create game engine
	GameEngine* ge = new GameEngine();

	//Create players
	Player* p1 = new Player("player1");
	Player* p2 = new Player("player2");
	p1->setReinforcementPool(50);
	p2->setReinforcementPool(50);
	vector<Player*> players = { p1, p2 };

	//Set territories
	vector<Territory*> p1Territories;
	vector<Territory*> p2Territories;

	//Give cards to players
	Card* cBomb = new Card(Card::Bomb);
	Card* cAirlift = new Card(Card::Airlift);
	Card* cBlockade = new Card(Card::Blockade);
	Card* cDiplomacy = new Card(Card::Diplomacy);
	Card* cReinforcement = new Card(Card::Reinforcement);

	Hand* h1 = new Hand();
	Hand* h2 = new Hand();

	h1->insertCard_Hand(cBomb);
	h1->insertCard_Hand(cDiplomacy);
	h1->insertCard_Hand(cReinforcement);
	h1->insertCard_Hand(cAirlift);
	h1->insertCard_Hand(cBlockade);

	p1->setCurrentHand(h1);
	p2->setCurrentHand(h2);

	for (int i = 0; i < map->listTerritory.size(); i++) {
		Territory* t = map->listTerritory.at(i);
		if (i < map->listTerritory.size() - 1) {
			t->setPlayer(p1);
			p1Territories.push_back(t);
		}
		else {
			t->setPlayer(p2);
			p2Territories.push_back(t);
		}
	}

	p1->setTerritoriesOwned(p1Territories);
	p2->setTerritoriesOwned(p2Territories);
*/
	//Go through main game loop

	std::list<Subject*>* _listSubject = new std::list<Subject*>;
	
	static LogObserver* loglog = new  LogObserver(_listSubject);
	
	GameEngine* ge = new GameEngine(loglog);
	ge->startupPhase();

	//ge->mainGameLoop(players, map);


	//delete p1;
	//delete p2;
	delete ge;
	//delete map;
	//delete h1;
	//delete h2;
	//delete mapL;

	return 0;
}
#endif