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
	std::string map = "canada";
	std::string pStrategies = "Aggressive Benevolent Neutral Cheater";
	int games = 4;
	int maxTurns = 30;

	GameEngine* ge = new GameEngine();
	ge->TournamentMode(map, pStrategies, games, maxTurns);

	delete ge;
	return 0;
}