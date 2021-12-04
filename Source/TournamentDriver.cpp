/*
 * The driver class for the tournament mode of the Warzone game
 *
 * Feel free to add members who have contributed to the 2nd part of assignment 3
 * 
 * @author Mark Nasol
 * @author
 * @author
 */

#include "Engine/GameEngine.h"
#include "Strategy/PlayerStrategies.h"
#include <Map/map.h>


int main() {
	/*
	-----------------------------------------------------------------
		Test code for checking if the game has exceeded the maximum 
		number of turns and the execution of tournament mode
	-----------------------------------------------------------------
	*/
	//Testing the main game loop with a number of maximum turns
	int maxTurns = 5;

	//Load map
	MapLoader* mapL = new MapLoader();
	mapL->loadMap("canada");
	Map* map = mapL->getListMap()->at(0);

	//Create game engine
	GameEngine* ge = new GameEngine();
	ge->setIsATournament(true);

	//Create players
	Player* p1 = new Player("player1");
	Player* p2 = new Player("player2");

	p1->setPlayerStrategy(new AggressivePlayerStrategy());
	p2->setPlayerStrategy(new BenevolentPlayerStrategy());
	p1->setReinforcementPool(50);
	p2->setReinforcementPool(50);

	//Set territories
	std::vector<Territory*> p1Territories;
	std::vector<Territory*> p2Territories;

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

	std::vector<Player*> players = {p1, p2};
	std::string result = ge->mainGameLoop(players, map, 50);
	std::cout << "Player strategy winner : " << result << std::endl;

	return 0;
}