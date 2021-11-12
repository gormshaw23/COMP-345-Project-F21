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

	//Set territories


	//1. Reinforcement phase cases

	//2. Issuing only deploy orders

	//3. Issue advance orders to defend or to attack

	//4. Play cards to issue orders

	//5. Remove player who has no more controlled territories

	//6. End game when there is one player left


	delete p1;
	delete p2;
	delete ge;
	delete map;
	delete mapL;

	return 0;
}