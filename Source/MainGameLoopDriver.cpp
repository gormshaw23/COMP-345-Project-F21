/**
 * The driver file for the Main game loop
 *
 * @author Mark Nasol
 */

#include "Player/Player.h"
#include "Engine/GameEngine.h"


int main() {
	//Load map
	MapLoader* mapL = new MapLoader();
	mapL->loadMap("canada");
	Map* map = mapL->getListMap()->at(0);

	//Create game engine
	GameEngine* ge = new GameEngine();

	//Create players
	Player *p1 = new Player("player1");
	Player* p2 = new Player("player2");
	p1->setRPool_temp(50);
	p2->setRPool_temp(50);
	list<Player*> players = { p1, p2 };

	//Set territories
	vector<Territory*> p1Territories;
	vector<Territory*> p2Territories;

	//Player 1 gets all territories but one
	//Player 2 gets ont territory only
	for (int i = 0; i < map->listTerritory.size(); i++) {
		Territory* t = map->listTerritory.at(i);
		if (i != map->listTerritory.size() - 1) {
			t->setPlayer(p1);
			p1Territories.push_back(t);
		}	
		else {
			t->setPlayer(p2);
			p2Territories.push_back(t);
		}
			
	}

	//Go through main game loop
	ge->mainGameLoop(players, map);

	delete p1;
	delete p2;
	delete ge;
	delete map;
	delete mapL;

	return 0;
}