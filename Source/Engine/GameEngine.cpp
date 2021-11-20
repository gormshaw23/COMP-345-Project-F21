#include "GameEngine.h"
#include <map>
#include <string>
#include <algorithm>
using namespace std;
#include <iostream>
#include <Common/CommonTypes.h>
#include "Cards/Cards.h"

/**
 * constructor of GameEngine class
 */
GameEngine::GameEngine()
{
	eState = new GameState(GAME_STATE_UNKNOWN);
#ifdef DEBUG_ENABLE
	cout << "constructor\n";
#endif
}
/**
 * Copy constructor of GameEngine class
 */
GameEngine::GameEngine(const GameEngine& obj)
{
	eState = new GameState;
	*eState = *obj.eState;
}

/**
 * destructor of GameEngine class
 */
GameEngine::~GameEngine()
{
	delete eState;
	eState = NULL;
#ifdef DEBUG_ENABLE
	cout << "destructor\n";
#endif
}

/**
 * setter
 * @param lState local state of GameState
 */
void GameEngine::setCurrentState(GameState lState)
{
	*eState = lState;
}
/**
 * getter
 * @return lState local state of GameState
 */
GameState GameEngine::getCurrentState()
{
	return *eState;
}

/**
 * Assignment operator of GameEngine class
 */

GameEngine& GameEngine::operator=(const GameEngine& obj)
{
	this->eState = new GameState(*obj.eState);
	return *this;
}

/**
 * Stream insertion operator of GameEngine class
 */
ostream& operator<<(ostream& out, const GameState value) {
	const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
	switch (value) {
		PROCESS_VAL(GAME_STATE_UNKNOWN);
		PROCESS_VAL(GAME_STATE_START);
		PROCESS_VAL(GAME_STATE_MAP_LOAD);
		PROCESS_VAL(GAME_STATE_MAP_VALIDATED);
		PROCESS_VAL(GAME_STATE_PLAYERS_ADDED);
		PROCESS_VAL(GAME_STATE_ASSIGN_REINFORCEMENT);
		PROCESS_VAL(GAME_STATE_ISSUE_ORDERS);
		PROCESS_VAL(GAME_STATE_EXECUTE_ORDERS);
		PROCESS_VAL(GAME_STATE_WIN);
	}
#undef PROCESS_VAL

	return out << s;
}
/**
 * free function: get user input
 * @param lState local state
 * @return Name string of the user input
 */
string get_user_input(GameState lState) {
	string Name;
	cout << "The current game state is (" << lState << ")\n";
	cout << "Please type your command with lower-case letters:" << "\n";
	getline(cin, Name);
	return Name;
}

/**
 * function game_run() of GameEngine class
 * To update the current state by a valid command, reject the command if it is invalid
 */
void GameEngine::game_run() {
	//set start state
	GameEngine::setCurrentState(GAME_STATE_START);
	string user_input;
	// map a key to the value
	map<game_user_input, string> user_input_list;
	user_input_list[LOADMAP] = "loadmap";
	user_input_list[VALIDATEMAP] = "validatemap";
	user_input_list[ADDPLAYER] = "addplayer";
	user_input_list[ASSIGNCOUNTRIES] = "assigncountries";
	user_input_list[ISSUEORDER] = "issueorder";
	user_input_list[ENDEXECORDERS] = "endexecorders";
	user_input_list[EXECORDER] = "execorder";
	user_input_list[ENDISSUEORDERS] = "endissueorders";
	user_input_list[WIN] = "win";
	user_input_list[PLAY] = "play";
	user_input_list[END] = "end";
	//using a loop to read the input until the end of the state
	while (true) {

		user_input = get_user_input(*eState);
		//compare the user input with game_user_input, if valid, update the state; if invalid, reject the command and remain the current state
		switch (*eState) {

		case  GAME_STATE_START:
			if (!user_input.compare(user_input_list[LOADMAP])) {
				setCurrentState(GAME_STATE_MAP_LOAD);
			}
			else {

				cout << "Error input(please try: loadmap)\n";
			}
			break;
		case  GAME_STATE_MAP_LOAD:
			if (!user_input.compare(user_input_list[LOADMAP])) {
				setCurrentState(GAME_STATE_MAP_LOAD);

			}
			else if (!user_input.compare(user_input_list[VALIDATEMAP])) {
				setCurrentState(GAME_STATE_MAP_VALIDATED);
			}
			else {

				cout << "Error input(please try: " << user_input_list[VALIDATEMAP] << " or "
					<< user_input_list[LOADMAP] << "\n";
			}
			break;
		case GAME_STATE_MAP_VALIDATED:
			if (!user_input.compare(user_input_list[ADDPLAYER])) {
				setCurrentState(GAME_STATE_PLAYERS_ADDED);

			}
			else {
				cout << "Error input(please try: " << user_input_list[ADDPLAYER] << "\n";
			}
			break;

		case GAME_STATE_PLAYERS_ADDED:
			if (!user_input.compare(user_input_list[ADDPLAYER])) {
				setCurrentState(GAME_STATE_PLAYERS_ADDED);

			}
			else if (!user_input.compare(user_input_list[ASSIGNCOUNTRIES])) {
				setCurrentState(GAME_STATE_ASSIGN_REINFORCEMENT);
			}
			else {
				cout << "Error input(please try: " << user_input_list[ADDPLAYER] << " or "
					<< user_input_list[ASSIGNCOUNTRIES] << ")\n";
			}
			break;
		case GAME_STATE_ASSIGN_REINFORCEMENT:

			if (!user_input.compare(user_input_list[ISSUEORDER])) {
				setCurrentState(GAME_STATE_ISSUE_ORDERS);

			}
			else {
				cout << "Error input(please try: " << user_input_list[ISSUEORDER] << ")\n";
			}

			break;
		case GAME_STATE_ISSUE_ORDERS:
			if (!user_input.compare(user_input_list[ISSUEORDER])) {
				setCurrentState(GAME_STATE_ISSUE_ORDERS);

			}
			else if (!user_input.compare(user_input_list[ENDISSUEORDERS])) {
				setCurrentState(GAME_STATE_EXECUTE_ORDERS);
			}
			else {
				cout << "Error input(please try: " << user_input_list[ISSUEORDER] << " or "
					<< user_input_list[ENDISSUEORDERS] << ")\n";
			}
			break;

		case GAME_STATE_EXECUTE_ORDERS:
			if (!user_input.compare(user_input_list[EXECORDER])) {
				setCurrentState(GAME_STATE_EXECUTE_ORDERS);

			}
			else if (!user_input.compare(user_input_list[ENDEXECORDERS])) {
				setCurrentState(GAME_STATE_ASSIGN_REINFORCEMENT);
			}
			else if (!user_input.compare(user_input_list[WIN])) {
				setCurrentState(GAME_STATE_WIN);
			}
			else {
				cout << "Error input(please try: " << user_input_list[EXECORDER] << " or "
					<< user_input_list[ENDEXECORDERS] << " or "
					<< user_input_list[WIN] << ")\n";
			}
			break;
		case GAME_STATE_WIN:
			if (!user_input.compare(user_input_list[PLAY])) {
				setCurrentState(GAME_STATE_START);

			}
			else if (!user_input.compare(user_input_list[END])) {
				setCurrentState(GAME_STATE_END);
			}
			else {
				cout << "Error input(please try: " << user_input_list[PLAY] << " or "
					<< user_input_list[END] << ")\n";
			}
			break;
		case GAME_STATE_END:

			break;
		default:

			break;
		}
		if (*eState == GAME_STATE_END) {

			break;
		}
	}//end of while loop
}//end of game_run()

/*
* The main game loop of the Warzone game
*/
void GameEngine::mainGameLoop(vector<Player*> players, Map* map) {
	while (players.size() != 1) { //Loop if there are 2 or more players left
		//Give a number of armies to each player
		for (Player* p : players)
			reinforcementPhase(p, map);

		//Have each player issue orders
		for (Player* p : players)
			issueOrdersPhase(p, map);

		//Execute all orders from players
		for (Player* p : players)
			executeOrdersPhase(p);
	}
	cout << "Game over, " << players.at(0)->getPlayerName() << " wins\n";
}

list<Player*> GameEngine::getPlayers_temp() {
	return players_temp;
}

/*
* Gives a number of armies to the player's reinforcement pool
* based on the number of territories
*
* @param *p pointer to a Player object
* @param *map pointer to a Map object
*/
void GameEngine::reinforcementPhase(Player* p, Map* map) {
	//Add armies to reinforcement pool based on territories owned
	vector<Territory*> playerTerritories = p->getTerritoriesOwned();
	int numTerritoriesOwned = playerTerritories.size();
	int currentRPool = p->getRPool_temp();

	int newArmies = (numTerritoriesOwned >= 1 && numTerritoriesOwned < 12) ? 3 : numTerritoriesOwned / 3;
	std::cout << "Adding " << newArmies << " armies to reinforcement pool\n";
	currentRPool = (numTerritoriesOwned >= 1 && numTerritoriesOwned < 12) ? currentRPool + 3 : currentRPool + newArmies;

	/*
	* Check if the player owns all territories of a continent,
	* if so, add continent's bonus to reinforcement pool
	*/
	//Sort player territories
	vector<Territory*> sortedPlayerTerritories = playerTerritories;
	sort(sortedPlayerTerritories.begin(), sortedPlayerTerritories.end());

	//Iterate through each territory
	int currentContinentID = 1;
	int territoryToContinentCount = 0; //Variable to count territories belonging to one continent
	for (Territory* t : sortedPlayerTerritories) {

		/*
		Reset territoryToContinentCount if the current territory's continent id
		is different from the previous territory
		*/
		if (t->getContinent() != currentContinentID) {
			currentContinentID = t->getContinent();
			territoryToContinentCount = 0;
		}

		territoryToContinentCount++; //Increment territoryToContinentCount

		/*
		Verify if the territoryToContinentCount is equal to the size of the
		continent's country list size. If so, add bonus armies to reinforcement pool
		*/
		vector<Continent*> mapContinents = map->listContinents;
		Continent* c = mapContinents.at(currentContinentID - 1);
		int numCountries = c->getCountryList()->size();
		if (territoryToContinentCount == numCountries) {
			std::cout << "BONUS: Adding " << c->getArmyValu() << " armies to reinforcement pool\n";
			currentRPool = currentRPool + c->getArmyValu();
		}

	}//end for loop

	p->setRPool_temp(currentRPool); //Update new reinforcement pool
	std::cout << p->getPlayerName() << "\'s reinforcement pool: " << currentRPool << "\n";
}

/*
* Issue players orders and place them in their order list
*
* @param *p pointer to a Player object
*/
void GameEngine::issueOrdersPhase(Player* p, Map* map) {
	bool turnEnded = false;
	std::vector<int> territoryIds = map->getTerritoryIds();
	std::vector<Territory*> inTerritories;
	std::vector<int> inArmies;

	/*
	Initialization of variables for the option
	to play a card
	*/
	Hand* h = p->getCurrentHand();
	Deck* d_temp = new Deck(); //Temporary deck variable
	int handSize = h->getHandSize();

	std::cout << "Issuing orders for " << p->getPlayerName() << "\n";
	while (!turnEnded) {
		//Input player order
		int currentRPool = p->getRPool_temp();
		int input;
		bool isValidInput = false;
		std::cout << "Enter a number for the given options:\n"
			<< "1 - Deploy Armies\n"
			<< "2 - Advance Armies\n"
			<< "3 - Play Card\n"
			<< "4 - End turn\n";
		std::cin >> input;

		//Check if the input is valid
		switch (input) {
		case 1: //Deploy armies
			while (!isValidInput) {
				//Input territory id
				int territory;
				std::cout << "Enter the territory id you would like to deploy to: \n";
				std::cin >> territory;

				//Check if territory exists
				if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end()) {
					//Add to list of territories for deploy order
					inTerritories.push_back(map->listTerritory.at(territory - 1));

					//Input number of armies to deploy
					int armies;
					std::cout << "Current reinfrocement pool: " << currentRPool << "\nEnter the number of armies you would like to deploy: \n";
					std::cin >> armies;

					//Check if number of armies is greater than 0 and less or equal than the reinforcement pool
					if (armies > 0 && armies <= currentRPool) {
						inArmies.push_back(armies);
						currentRPool -= armies;
						p->setRPool_temp(currentRPool);

						//Input user if they would like to add another territory
						string yesOrNo;
						std::cout << "Would you like to add another territory? (y/n)\n";
						std::cin >> yesOrNo;
						if (yesOrNo.compare("n") == 0) {
							isValidInput = true;
						}
					}
					else {
						cout << "The given number of armies is invalid\n";
					}
				}
				else {
					cout << "The given territory does not exist\n";
				}
			}//end while loop
			//Create deploy order
			p->issueOrder(EOrderTypes::Deploy, inTerritories, inArmies);

			//Clear vectors
			inTerritories.clear();
			inArmies.clear();
			break;

		case 2: //Advance armies
			while (!isValidInput) {
				//Input territory id
				int territory;
				std::cout << "Enter the territory id you would like to advance to: \n";
				std::cin >> territory;

				//Check if territory exists
				if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end()) {
					//Add to list of territories for advance order
					Territory* t = map->listTerritory.at(territory - 1);
					inTerritories.push_back(t);

					//Input number of armies to advance
					int armies;
					std::cout << "Enter the number of armies you would like to advance: \n";
					std::cin >> armies;

					//Check if number of armies is greater than 0
					if (armies > 0) {
						inArmies.push_back(armies);

						//Input user if they would like to add another territory
						string yesOrNo;
						std::cout << "Would you like to add another territory? (y/n)\n";
						std::cin >> yesOrNo;
						if (yesOrNo.compare("n") == 0) {
							isValidInput = true;
						}
					}
					else {
						cout << "The given number of armies is invalid\n";
					}
				}
				else {
					cout << "The given territory does not exist\n";
				}
			}//end loop
			//Create advance order
			p->issueOrder(EOrderTypes::Advance, inTerritories, inArmies);

			//Clear vectors
			inTerritories.clear();
			inArmies.clear();
			break;

		case 3: //Play card
			//Retrieve player's hand		
			Card* c;	
			std::cout << p->getPlayerName() << "\'s hand\n";
			h->showHand();

			//Input card
			if (handSize > 0) {
				int cardInput;
				std::string prompt = (handSize == 1) ? "Enter 1 " : "Enter a number between 1 and " + handSize;
				std::cout << prompt << " to play a card or 0 to return to the menu";
				std::cin >> cardInput;
				if (cardInput > 0) {
					//Iterate through hand
					for (int i = 0; i < handSize; i++) {
						c = h->drawCard_Hand();
						if (i == cardInput - 1) {
							//Play card from the given input
							EOrderTypes e = h->play(c);
							d_temp->insertCard_Deck(c);
							if (c->getNewCardType() == Card::Reinforcement) {
								p->setRPool_temp(p->getRPool_temp() + 5);
							}
							else {
								switch (e) {
								case EOrderTypes::Bomb:
									while (!isValidInput) {
										//Input territory id
										int territory;
										std::cout << "Enter the territory id you would like to advance to: \n";
										std::cin >> territory;

										if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end()) {
											//Add to list of territories for bomb order
											inTerritories.push_back(map->listTerritory.at(territory - 1));
											isValidInput = true;
										}
										else {
											cout << "The given territory does not exist\n";
										}
									} // end while loop
									//Issue bomb order
									p->issueOrder(e, inTerritories, inArmies);

									//Clear vectors
									inTerritories.clear();
									inArmies.clear();
									break;

								case EOrderTypes::Blockade:
									while (!isValidInput) {
										//Input territory id
										int territory;
										std::cout << "Enter the territory id you would like to initiate a blockade: \n";
										std::cin >> territory;

										if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end()) {
											//Add to list of territories for blockade order
											inTerritories.push_back(map->listTerritory.at(territory - 1));
											isValidInput = true;
										}
										else {
											cout << "The given territory does not exist\n";
										}
									} // end while loop
									//Issue blockade order
									p->issueOrder(e, inTerritories, inArmies);

									//Clear vectors
									inTerritories.clear();
									inArmies.clear();
									break;

								case EOrderTypes::Airlift:
									while (!isValidInput) {
										//Input territory id
										int territory;
										std::cout << "Enter the territory id you would like to advance to: \n";
										std::cin >> territory;

										//Check if territory exists
										if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end()) {
											//Add to list of territories for Airlift order
											inTerritories.push_back(map->listTerritory.at(territory - 1));

											//Input number of armies to advance
											int armies;
											std::cout << "Current reinfrocement pool: " << currentRPool << "Enter the number of armies you would like to advance: \n";
											std::cin >> armies;
											inArmies.push_back(armies);
											isValidInput = true;
										}
										else {
											cout << "The given territory does not exist\n";
										}
									}//end loop
									//Issue airlift order
									p->issueOrder(e, inTerritories, inArmies);

									//Clear vectors
									inTerritories.clear();
									inArmies.clear();
									break;

								case EOrderTypes::Negotiate:
									//Issue negotiate order
									p->issueOrder(e, inTerritories, inArmies);
									break;
								}//end switch 
							}
							break;
						}
					}
				}
			} //end if
			else {
				std::cout << "You have no cards available";
			}
			break;
		case 4: //End turn
			std::cout << "Ending turn\n";
			turnEnded = true;
			break;
		default:
			cout << "The given input is invalid\n";
			break;
		}//end switch
	}//end while
}

/*
* Execute player orders
*
* @param *p pointer to a Player object
*/
void GameEngine::executeOrdersPhase(Player* p) {
	std::cout << "Executing " << p->getPlayerName() << "\'s orders\n";
	OrdersList* ol = p->getOrders();

	/*
	Loop player orders for deploy orders and execute them
	*/
	for (int i = 0; i < ol->getOList().size(); i++) {
		Order* o = ol->get(i);
		if (o->getTypeName().compare("deploy") == 0) {
			o->execute();
		}
	}

	/*
	Loop player orders and execute remaining orders
	*/
	for (Order* o : ol->getOList()) {
		if (o->getTypeName().compare("deploy") != 0) {
			o->execute();
		}
		ol->remove(0);
	}
}

