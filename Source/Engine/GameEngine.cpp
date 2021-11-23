#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <Common/CommonTypes.h>
#include <vector>

#include "Engine/GameEngine.h"
#include "Player/Player.h"
#include "Map/map.h"
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

GameEngine::GameEngine(std::list<Subject*>* list) : CommandProcessor(list)
{
    eState = new GameState(GAME_STATE_UNKNOWN);

#ifdef DEBUG_ENABLE
    cout << "constructor\n";
#endif
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
	GameEngine::Subject::Notify(*this);
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

Player* GameEngine::getNeutralPlayer() const
{
    return neutralPlayer;
}

GameEngine& GameEngine::getInstance()
{
    static GameEngine instance;
    return instance;
}

/**
 * Stream insertion operator of GameEngine class
 */
std::ostream& operator<<(std::ostream& out, const GameState value) {
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
std::string get_user_input(GameState lState) {
	std::string Name = "";
	std::cout << "The current game state is (" << lState << ")\n";
	std::cout << "Please type your command with lower-case letters:" << "\n";
	getline(std::cin, Name);
	return Name;
}

/**
 * function game_run() of GameEngine class
 * To update the current state by a valid command, reject the command if it is invalid
 */
void GameEngine::game_run() {
    //set start state
    GameEngine::setCurrentState(GAME_STATE_START);
    std::string user_input;
    // map a key to the value
    std::map<game_user_input, std::string> user_input_list;
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

    Command* useCommand;
    while (true) {

        user_input = "get_user_input(*eState)";
         useCommand = this->getCommand();

        //compare the user input with game_user_input, if valid, update the state; if invalid, reject the command and remain the current state
        switch (*eState) {

            case  GAME_STATE_START:
                if (this->validate(useCommand,"loadmap")) {
                    useCommand->saveEffect(" from Game state start to load map");
                    setCurrentState(GAME_STATE_MAP_LOAD);
                    std::cout << "Will it finaly work ?"<<std::endl;
                }
                else {
                    useCommand->saveEffect(" Invalid command in game state start");
					std::cout << "Error input(please try: loadmap)\n";
                }
                break;
            case  GAME_STATE_MAP_LOAD:
                if (this->validate(useCommand, "loadmap")) {
                    useCommand->saveEffect(" from load map  to load map");
                    setCurrentState(GAME_STATE_MAP_LOAD);

                }
                else if (this->validate(useCommand, "validatemap")){
                    useCommand->saveEffect(" from load map to validate  map");
                    setCurrentState(GAME_STATE_MAP_VALIDATED);
                    std::cout << " Map validated " << std::endl;
                }
                else {
                    useCommand->saveEffect(" Invalide commandd in map load");
					std::cout << "Error input(please try: "<< user_input_list[VALIDATEMAP] << " or "
                        << user_input_list[LOADMAP] <<"\n" ;
                }
                break;
            case GAME_STATE_MAP_VALIDATED:
                if (this->validate(useCommand, "addplayer")) {
                    useCommand->saveEffect(" from Game state map validate to add player");
                    setCurrentState(GAME_STATE_PLAYERS_ADDED);
                    std::cout << "|| entering player added||" << std::endl;
                }
                else {
                    useCommand->saveEffect(" Invlid in map validated");
					std::cout << "Error input(please try: " << user_input_list[ADDPLAYER] << "\n";
                }
                break;
            case GAME_STATE_PLAYERS_ADDED:
                if (this->validate(useCommand, "assigncountries")) {
                    useCommand->saveEffect(" from Game state player added to assigne countries");
                    setCurrentState(GAME_STATE_ASSIGN_REINFORCEMENT);
                    std::cout << "|| entering assigne countries||" << std::endl;
                    
                }
                else if (this->validate(useCommand, "addplayer")) {
                    useCommand->saveEffect(" from Game state add player to addplayer");
                    setCurrentState(GAME_STATE_PLAYERS_ADDED);
                    std::cout << "|| entering player added||" << std::endl;
                }
                else {
                    useCommand->saveEffect("Invalid in player added ");
					std::cout << "Error input(please try: " << user_input_list[ADDPLAYER] <<" or "
                        << user_input_list[ASSIGNCOUNTRIES] << ")\n";
                }
                break;
            case GAME_STATE_ASSIGN_REINFORCEMENT:

                if (this->validate(useCommand, "issueorder")) {
                    useCommand->saveEffect(" from Game state assigne reinforcement to issue order");
                    setCurrentState(GAME_STATE_ISSUE_ORDERS);
                    std::cout << "|| entering issue Order||" << std::endl;
                }
                else {
                    useCommand->saveEffect(" invalid in assigne reinforcement");
					std::cout << "Error input(please try: " << user_input_list[ISSUEORDER] <<")\n";
                }

                break;
            case GAME_STATE_ISSUE_ORDERS:
                if (this->validate(useCommand, "endissueorders")) {
                    useCommand->saveEffect(" from Game state issue order to execute order order");
                    setCurrentState(GAME_STATE_EXECUTE_ORDERS);
                    std::cout << "|| entering end issue order||" << std::endl;
                }
                
                else if(this->validate(useCommand, "issueorder")) {
                    useCommand->saveEffect(" from Game state issue order to issue order");
                    setCurrentState(GAME_STATE_ISSUE_ORDERS);
                    std::cout << "|| entering issue Order||" << std::endl;
                }
                else {
                    useCommand->saveEffect(" invalid in issue order");
					std::cout << "Error input(please try: " << user_input_list[ISSUEORDER] << " or "
                        << user_input_list[ENDISSUEORDERS] << ")\n";
                }
                break;

            case GAME_STATE_EXECUTE_ORDERS:
                if (this->validate(useCommand, "execorder")) {
                    useCommand->saveEffect(" from Game stat ececutte order to execute order");
                    setCurrentState(GAME_STATE_EXECUTE_ORDERS);
                    std::cout << "|| entering execute Order||" << std::endl;
                    
                }
                else if (this->validate(useCommand, "endexecorders")) {
                    useCommand->saveEffect(" from Game state execute order  to end execute order");
                    setCurrentState(GAME_STATE_ASSIGN_REINFORCEMENT);
                    std::cout << "|| entering end execute Order||" << std::endl;
                }
                else if (this->validate(useCommand, "win")) {
                    useCommand->saveEffect(" from Game state execute order to win");
                    setCurrentState(GAME_STATE_WIN);
                    std::cout << "|| entering win||" << std::endl;
                }
                else {
                    useCommand->saveEffect(" invalid in exec order");
					std::cout << "Error input(please try: " << user_input_list[EXECORDER] << " or "
                        << user_input_list[ENDEXECORDERS] <<" or "
                        << user_input_list[WIN] << ")\n";
                }
                break;
            case GAME_STATE_WIN:
                if (this->validate(useCommand, "play")) {
                    useCommand->saveEffect(" from Game state win to start");
                    setCurrentState(GAME_STATE_START);
                    std::cout << "|| entering play||" << std::endl;

                }
                else if (this->validate(useCommand, "end")) {
                    useCommand->saveEffect(" from Game state win to end");
                    setCurrentState(GAME_STATE_END);
                    std::cout << "|| entering end||" << std::endl;
                }
                else {
                    useCommand->saveEffect(" Invalid in win");
					std::cout << "Error input(please try: " << user_input_list[PLAY] << " or "
                        << user_input_list[END] << ")\n";
                }
                break;
            case GAME_STATE_END:
                std::cout << "|| we reached the end||" << std::endl;
                break;
            default:

                break;
        }
        useCommand = NULL;

        if (*eState == GAME_STATE_END) {

            break;
        }
    }//end of while loop
}//end of game_run()


std::string GameEngine::stringToLog() {
    std::string toLog  = "New game state : ";

    switch (this->getCurrentState())
    {
    case GAME_STATE_UNKNOWN:
        toLog = toLog + " unknown\n";
            break;
    case GAME_STATE_START:
        toLog = toLog + "gamestart\n";
        break;
    case GAME_STATE_MAP_LOAD:
        toLog = toLog + "map load\n";
        break;
    case GAME_STATE_MAP_VALIDATED:
        toLog = toLog + "map validated\n";
        break;
    case GAME_STATE_PLAYERS_ADDED:
        toLog = toLog + "player added\n";
        break;
    case GAME_STATE_ASSIGN_REINFORCEMENT:
        toLog = toLog + "assigne reinforcement\n";
        break;
    case GAME_STATE_ISSUE_ORDERS:
        toLog = toLog + "issue order\n";
        break;
    case GAME_STATE_EXECUTE_ORDERS:
        toLog = toLog + "execute order\n";
        break;
    case GAME_STATE_WIN:
        toLog = toLog + "win\n";
        break;
    default:
        break;
    }
    return  toLog;
}

/*
* The main game loop of the Warzone game
*/
void GameEngine::mainGameLoop(vector<Player*> players, Map* map) {
	while (players.size() != 1) { //Loop if there are 2 or more players left
		int initPlayersSize = players.size();

		//Give a number of armies to each player
		for (Player* p : players)
			reinforcementPhase(p, map);

		//Have each player issue orders
		for (Player* p : players)
			issueOrdersPhase(p, map);

		//Execute all orders from players
		for (Player* p : players)
			executeOrdersPhase(p);

		//Remove player(s) if they have no more territories
		int playerIndex = 0;
		for (int i = 0; i < initPlayersSize; i++) {
			Player* p = players.at(playerIndex);
			if (p->getTerritoriesOwned().size() == 0) {
				players.erase(players.begin() + playerIndex);
				playerIndex--;
			}
			else {
				playerIndex++;
			}
		}
	}//end while
	std::cout << "Game over, " << players.at(0)->getPlayerName() << " wins\n";
}

/*
* Gives a number of armies to the player's reinforcement pool
* based on the number of territories
*
* @param *p pointer to a Player object
* @param *map pointer to a Map object
*/
const void GameEngine::reinforcementPhase(Player* p, Map* map) {

	//Add armies to reinforcement pool based on territories owned
	std::vector<Territory*> playerTerritories = p->getTerritoriesOwned();
	int numTerritoriesOwned = playerTerritories.size();
	int currentRPool = p->getReinforcementPoolSize();

	int newArmies = (numTerritoriesOwned >= 1 && numTerritoriesOwned < 12) ? 3 : numTerritoriesOwned / 3;
	std::cout << "Adding " << newArmies << " armies to reinforcement pool\n";
	currentRPool = (numTerritoriesOwned >= 1 && numTerritoriesOwned < 12) ? currentRPool + 3 : currentRPool + newArmies;

	/*
	* Check if the player owns all territories of a continent,
	* if so, add continent's bonus to reinforcement pool
	*/
	//Sort player territories
	std::vector<Territory*> sortedPlayerTerritories = playerTerritories;
	std::sort(sortedPlayerTerritories.begin(), sortedPlayerTerritories.end(), [](const Territory* lhs, const Territory* rhs) {
		return lhs->getID() < rhs->getID();
		});

	//Iterate through each territory
	int currentContinentID = sortedPlayerTerritories.at(0)->getContinent(); // Get the minimum territory id
	int territoryToContinentCount = 0; //Variable to count territories belonging to one continent
	std::vector<Continent*> mapContinents = map->listContinents;
	for (Territory* t : sortedPlayerTerritories) {

		/*
		Reset territoryToContinentCount if the current territory's continent id
		is different from the previous territory
		*/
		if (t->getContinent() != currentContinentID) {
			currentContinentID = t->getContinent();
			territoryToContinentCount = 1;
		}
		else {
			territoryToContinentCount++; //Increment territoryToContinentCount
		}

		/*
		Verify if the territoryToContinentCount is equal to the size of the
		continent's country list size. If so, add bonus armies to reinforcement pool
		*/
		Continent* c = mapContinents.at(currentContinentID - 1);
		int numCountries = c->getCountryList()->size();
		if (territoryToContinentCount == numCountries) {
			std::cout << "BONUS: Adding " << c->getArmyValu() << " armies to reinforcement pool\n";
			currentRPool = currentRPool + c->getArmyValu();
			territoryToContinentCount = 0;
			currentContinentID++;
		}
	}//end for loop

	p->setReinforcementPool(currentRPool); //Update new reinforcement pool
	std::cout << p->getPlayerName() << "\'s reinforcement pool: " << currentRPool << "\n" << std::endl;
}

/*
* Issue players orders and place them in their order list
*
* @param *p pointer to a Player object
*/
const void GameEngine::issueOrdersPhase(Player* p, Map* map) {
	int currentRPool = 0;
	bool turnEnded = false;
	std::vector<int> territoryIds = map->getTerritoryIds();
	std::vector<Territory*> inTerritories;
	std::vector<int> inArmies;

	/*
	Initialization of variables for the option
	to play a card
	*/
	Hand* h = p->getCurrentHand();
	//Deck* d_temp = new Deck(); //Temporary deck variable
	//d_temp->setDeckSize(20);
	int handSize = h->getHandSize();

	std::cout << "Issuing orders for " << p->getPlayerName() << "\n";
	while (!turnEnded) {
		//Input player order
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
			currentRPool = p->getReinforcementPoolSize();
			while (!isValidInput)
			{
				//Input territory id
				int territory;
				std::cout << "Enter the territory id you would like to deploy to: \n";
				std::cin >> territory;

				//Check if territory exists
				if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end())
				{
					//Add to list of territories for deploy order
					inTerritories.push_back(map->listTerritory.at(territory - 1));

					//Input number of armies to deploy
					int armies;
					std::cout << "Current reinfrocement pool: " << currentRPool << "\nEnter the number of armies you would like to deploy: \n";
					std::cin >> armies;

					//Check if number of armies is greater than 0 and less or equal than the reinforcement pool
					if (armies > 0 && armies <= currentRPool)
					{
						inArmies.push_back(armies);
						currentRPool -= armies;
						p->setReinforcementPool(currentRPool);

						//Input user if they would like to add another territory
						string yesOrNo;
						std::cout << "Would you like to add another territory? (y/n)\n";
						std::cin >> yesOrNo;
						if (yesOrNo.compare("n") == 0) {
							isValidInput = true;
						}
					}
					else
					{
						std::cout << "The given number of armies is invalid\n";
					}//end inner if-else
				}
				else
				{
					std::cout << "The given territory does not exist\n";
				}//end outer if-else
			}//end while loop
			//Create deploy order
			p->issueOrder(EOrderType::Deploy, std::vector<Player*>(), inTerritories, inArmies);

			//Clear vectors
			inTerritories.clear();
			inArmies.clear();
			break;

		case 2: //Advance armies
			if (p->getReinforcementPoolSize() != 0) {
				std::cout << "You must deploy all armies before issuing other orders\n";
			}
			else {
				while (!isValidInput)
				{
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
						else
						{
							cout << "The given number of armies is invalid\n";
						}//end inner if-else
					}
					else
					{
						cout << "The given territory does not exist\n";
					}//end outer if-else
				}//end loop
				//Create advance order
				p->issueOrder(EOrderType::Advance, std::vector<Player*>(), inTerritories, inArmies);

				//Clear vectors
				inTerritories.clear();
				inArmies.clear();
			}//end if
			break;

		case 3: //Play card
			if (p->getReinforcementPoolSize() != 0) {
				std::cout << "You must deploy all armies before issuing other orders\n";
			}
			else {
				bool playedCard = false;

				//Retrieve player's hand		
				Card* c;
				std::cout << p->getPlayerName() << "\'s hand\n";
				h->showHand();

				//Input card
				if (handSize > 0)
				{
					int cardInput;
					std::string prompt = (handSize == 1) ? "Enter 1 " : "Enter a number between 1 and " + handSize;
					std::cout << prompt << " to play a card or 0 to return to the menu";
					std::cin >> cardInput;
					if (cardInput > 0)
					{
						//Iterate through hand
						for (int i = handSize - 1; i >= 0; i--)
						{
							if (playedCard)
								break; //End for loop since a card was played

							c = h->drawCard_Hand();
							if (i == cardInput - 1)
							{
								//Play card from the given input
								EOrderType e = h->play(c);
								if (c->getNewCardType() == Card::Reinforcement)
								{
									p->setReinforcementPool(p->getReinforcementPoolSize() + 5);
									std::cout << "Adding 5 armies to reinforcement pool\n" << std::endl;
									playedCard = true;
								}
								else
								{
									switch (e)
									{
									case EOrderType::Bomb:
										while (!isValidInput)
										{
											//Input territory id
											int territory;
											std::cout << "Enter the territory id you would like to bomb: \n";
											std::cin >> territory;

											if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end())
											{
												//Add to list of territories for bomb order
												inTerritories.push_back(map->listTerritory.at(territory - 1));
												isValidInput = true;
											}
											else
											{
												std::cout << "The given territory does not exist\n";
											}
										} // end while loop
										//Issue bomb order
										p->issueOrder(e, std::vector<Player*>(), inTerritories, inArmies);
										playedCard = true;
										break;

									case EOrderType::Blockade:
										while (!isValidInput)
										{
											//Input territory id
											int territory;
											std::cout << "Enter the territory id you would like to initiate a blockade: \n";
											std::cin >> territory;

											if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end())
											{
												//Add to list of territories for blockade order
												inTerritories.push_back(map->listTerritory.at(territory - 1));
												isValidInput = true;
											}
											else
											{
												std::cout << "The given territory does not exist\n";
											}
										} // end while loop
										//Issue blockade order
										p->issueOrder(e, std::vector<Player*>(), inTerritories, inArmies);
										break;

									case EOrderType::Airlift:
										while (!isValidInput)
										{
											//Input territory id
											int territory;
											std::cout << "Enter the territory id you would like to airlift to: \n";
											std::cin >> territory;

											//Check if territory exists
											if (std::find(territoryIds.begin(), territoryIds.end(), territory) != territoryIds.end())
											{
												//Add to list of territories for Airlift order
												inTerritories.push_back(map->listTerritory.at(territory - 1));

												//Input number of armies to airlift
												int armies;
												std::cout << "Enter the number of armies you would like to airlift: \n";
												std::cin >> armies;
												inArmies.push_back(armies);
												isValidInput = true;
											}
											else
											{
												std::cout << "The given territory does not exist\n";
											}
										}//end loop
										//Issue airlift order
										p->issueOrder(e, std::vector<Player*>(), inTerritories, inArmies);
										break;

									case EOrderType::Negotiate:
										//Issue negotiate order
										std::cout << "Playing negotiate card\n";
										p->issueOrder(e, std::vector<Player*>(), inTerritories, inArmies);
										break;
									}//end switch 
									//Clear vectors
									inTerritories.clear();
									inArmies.clear();
								}//end 4th inner if-else (Play card from the given input)
							}//end 3rd inner if (i == cardInput - 1)
						}//end for loop (Iterate through hand)
					}//end 2nd inner if (cardInput > 0)
				} //end if (Input card)
				else
				{
					std::cout << "You have no cards available";
				}//end inner if-else
			}//end outer if-else
			break;
		case 4: //End turn
			std::cout << "Ending turn\n" << std::endl;
			turnEnded = true;
			break;
		default:
			std::cout << "The given input is invalid\n";
			break;
		}//end switch
	}//end while - continue if turnEnded is false
}

/*
* Execute player orders
*
* @param *p pointer to a Player object
*/
const void GameEngine::executeOrdersPhase(Player* p) {
	std::cout << "Executing " << p->getPlayerName() << "\'s orders\n";
	OrdersList* ol = p->getOrders();

	/*
	Loop player orders for deploy orders and execute them
	*/
	for (int i = 0; i < ol->getOList().size(); i++) {
		Order* o = ol->get(i);
		if (o->getType() == EOrderType::Deploy) {
			o->execute();
		}
	}

	/*
	Loop player orders and execute remaining orders
	*/
	for (Order* o : ol->getOList()) {
		if (o->getType() != EOrderType::Deploy) {
			o->execute();
		}
		ol->remove(0);
	}
}