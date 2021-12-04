#include "Engine/GameEngine.h"
#include "Player/Player.h"
#include "Map/map.h"
#include "Cards/Cards.h"
#include "Common/CommonTypes.h"
#include "Strategy/PlayerStrategies.h"
#include "../CommandProcessor/CommandProcessor.h"

#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <random>
#include <iomanip>

std::vector<std::string> splitString(std::string str)
{
	std::string word = "";
	std::vector<std::string> splitstring;
	bool betwenbrakets = false;

	for (auto x : str)
	{
		if (x == '<') {
			betwenbrakets = true;
		}
		if (betwenbrakets == false) {
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
		else {
			if (x == '>') {
				betwenbrakets = false;
			}
			else {
				word = word + x;
			}
		}
		
	}
	splitstring.push_back(word);
	return splitstring;
}
/**
 * constructor of GameEngine class
 */
GameEngine::GameEngine()
{
	eState = new GameState(GAME_STATE_UNKNOWN);
	isATournament = false;
#ifdef DEBUG_ENABLE
	std::cout << "constructor\n";
#endif
}
/**
 * Copy constructor of GameEngine class
 */
GameEngine::GameEngine(const GameEngine& obj)
{
	eState = new GameState;
	*eState = *obj.eState;
	commandProces = obj.commandProces;
	isATournament = false;

}

GameEngine::GameEngine(Observer* list) 
{
    eState = new GameState(GAME_STATE_UNKNOWN);
	commandProces = nullptr;
	isATournament = false;
	std::string inputType;
	std::vector<std::string> proccesInput;
	bool goodInput = false;
	while (goodInput == false) {
		std::cout << " How will the command be entered ? ,\n Manuel : -console\n From a file : -file <filename> " << std::endl;
		std::getline(std::cin, inputType);
		proccesInput = splitString(inputType);
		

		if (proccesInput.at(0) == "-console") {
			commandProces = new CommandProcessor(list);
			goodInput = true;
		}
		else if (proccesInput.at(0)== "-file")
		{
			proccesInput.at(1).erase(remove(proccesInput.at(1).begin(), proccesInput.at(1).end(), '<'), proccesInput.at(1).end());
			proccesInput.at(1).erase(remove(proccesInput.at(1).begin(), proccesInput.at(1).end(), '>'), proccesInput.at(1).end());
			std::cout << proccesInput.at(1);
			FileCommandProcessorAdapter * tempCommandProcessor =  new FileCommandProcessorAdapter(list);
			tempCommandProcessor->setFilePath(proccesInput.at(1));
			commandProces = tempCommandProcessor;
			tempCommandProcessor = nullptr;
			delete tempCommandProcessor;
			goodInput = true;
		}
	}
	
#ifdef DEBUG_ENABLE
	std::cout << "constructor\n";
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
	std::cout << "destructor\n";
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
		
	}
#undef PROCESS_VAL

	return out << s;
}

void showPossibleAction(GameState lstate) {
	std::cout << "\n-The current game state is (" << lstate << ")\n";
	switch (lstate)
	{
	case GAME_STATE_START:
		std::cout << "-Possible command Option : loadmap <filename>" << std::endl;
		break;
	case  GAME_STATE_MAP_LOAD:
		std::cout << "-Possible command Option : loadmap <filename> or validatemap" << std::endl;
		break;
	case GAME_STATE_MAP_VALIDATED:
		std::cout << "-Possible command Option : addplayer" << std::endl;
		break;
	case GAME_STATE_PLAYERS_ADDED:
		std::cout << "-Possible command Option : addplayer or gamestart (if two player are present)" << std::endl;
		break;
	default:
		break;
	}
	std::cout << "-Please type your command with lower-case letters:" << "\n";
}

/**
 * free function: get user input
 * @param lState local state
 * @return Name string of the user input
 */
std::string get_user_input(GameState lState, GameEngine* reference) {
	std::string Name = "";

	showPossibleAction(lState);
	Command* userCommand = reference->commandProces->getCommand(); // use the command processor to enable the loging of information and the file reading
	Name = userCommand->getCommand();
	return Name;
}


std::string GameEngine::stringToLog() {
    std::string toLog  = "New game state : ";

    switch (this->getCurrentState())
    {
    case GAME_STATE_UNKNOWN:
        toLog = toLog + " unknown";
            break;
    case GAME_STATE_START:
        toLog = toLog + "gamestart";
        break;
    case GAME_STATE_MAP_LOAD:
        toLog = toLog + "map load";
        break;
    case GAME_STATE_MAP_VALIDATED:
        toLog = toLog + "map validated";
        break;
    case GAME_STATE_PLAYERS_ADDED:
        toLog = toLog + "player added";
        break;
	case GAME_STATE_FINISHED:
		toLog = toLog + "player added";
		break;
  
    default:
		toLog = "Tournament Mode : \n-M:	";
		for (int x = 0; x < listMapName.size(); x++) {
			toLog = toLog + listMapName.at(x) + ",	";
		}
		toLog = toLog + "\n-P:	";
		for (int y = 0; y < listPlayerFromConsole.size(); y++) {
			toLog = toLog + listPlayerFromConsole.at(y) + ",	";
		}
		toLog = toLog + "\n-G: " + std::to_string(numberGamePrint) + "\n -D: " + std::to_string(drawTurnLimitPrint)+"\n"+ gameresult;
        break;
    }
    return  toLog;
}

static std::string filename;
static std::string playername;
static int playercount = 0;
MapLoader* newmap = new MapLoader();
Map* mapToUse = new Map();
Deck* newDeck = new Deck(30);
Card* newCard = newDeck->drawCard_Deck();
std::vector<int> ReinforcementPools;

/**
 * private function extractName(string) of GameEngine class
 * @param string str user input
 * @return string name
 */



std::string GameEngine::extractName(std::string str) {

	unsigned first = str.find("<");
	unsigned last = str.find(">");
	std::string strNew = str.substr(first + 1, last - first - 1);
	return strNew;
}

/**
 * private function addPlayer(string) of GameEngine class
 * @param string str user input
 * @return void
 */

void GameEngine::addPlayer(std::string user_input) {
	playername = extractName(user_input);
	Player* p = new Player(playername);
	p->setCurrentGameInstance(this);
	p->setCommandProcessor(commandProces);
	playerlist.push_back(p);
	playercount++;
	std::cout << "The player " << playername << " is added." << std::endl;
	std::cout << "There should be 2-6 players in this game. Now we have " << playercount << " players." << std::endl;
	std::cout << std::endl << "Player#    PlayerName" << std::endl;
	for (int i = 0; i < (int)playerlist.size(); i++)
	{
		std::cout << i + 1 << "             " << playerlist.at(i)->getPlayerName() << std::endl;
	}
	std::cout << std::endl;
}


/**
 * private function gamestart() of GameEngine class
 * @param blank
 * @return void
 */

void GameEngine::gamestart() {

	//a) fairly distribute all the territories to the players

	std::random_device rd;
	std::mt19937 g(rd());

	int numberOfTerritory = mapToUse->listTerritory.size();
	std::cout << std::endl << "The numberOfTerritory in the map is: " << numberOfTerritory << std::endl;
	Territory* t = new Territory();
	int round = numberOfTerritory / playercount;
	for (int ii = 0; ii < round; ii++) {
		for (int i = 0; i < playercount; i++) {
			t = mapToUse->listTerritory.at(i + ii * playercount);
			t->setPlayer(playerlist[i]);
			playerlist[i]->getTerritoriesOwned().push_back(t);
		}
	}
	int remainder = numberOfTerritory % playercount;
	for (int i = 0; i < remainder; i++) {
		t = mapToUse->listTerritory.at(round * playercount);
		t->setPlayer(playerlist[i]);
		playerlist[i]->getTerritoriesOwned().push_back(t);
	}

	std::cout << "Fairly distribute all the territories to the players>>>>>>>" << std::endl;
	for (int i = 0; i < playercount; i++)
	{
		std::cout << *(playerlist.at(i)) << std::endl;
	}

	//b) determine randomly the order of play of the players in the game
	std::cout << std::endl << "Determine randomly the order of play of the players in the game>>>>>>" << std::endl;
	std::cout << "Before shuffle, the order of play is: ";
	for (std::vector<Player*>::iterator it = playerlist.begin(); it != playerlist.end(); ++it) {
		std::cout << " " << (*it)->getPlayerName();
	}

	std::shuffle(playerlist.begin(), playerlist.end(), g);

	std::cout << std::endl << "After shuffle, the order of play is: ";
	for (std::vector<Player*>::iterator it = playerlist.begin(); it != playerlist.end(); ++it)
	{
		std::cout << " " << (*it)->getPlayerName();
	}
	std::cout << std::endl;

	//c) give 50 initial armies to the players, which are placed in their respective reinforcement pool
	std::cout << std::endl << "Give 50 initial armies to the players>>>>>>" << std::endl;
	for (int i = 0; i < playercount; i++)
	{
		ReinforcementPools.push_back(50);
	}
	std::cout << "player name        quantity of armies" << std::endl;
	for (int i = 0; i < playercount; i++)
	{
		std::cout << playerlist.at(i)->getPlayerName();
		std::cout << std::setw(25 - playerlist.at(i)->getPlayerName().size());
		std::cout << ReinforcementPools.at(i) << std::endl;
	}
	std::cout << std::endl;
	//d) let each player draw 2 initial cards from the deck using the deck�s draw() method

	for (int i = 0; i < playercount; i++) {
		playerlist.at(i)->getCurrentHand()->insertCard_Hand(newCard);
		newCard = newDeck->drawCard_Deck();
		playerlist.at(i)->getCurrentHand()->insertCard_Hand(newCard);
	}
	std::cout << "Let each player draw 2 initial cards from the deck>>>>>>>" << std::endl;
	std::cout << "players' initial cards are: " << std::endl;
	std::cout << "player name        cards" << std::endl;
	for (int i = 0; i < playercount; i++)
	{
		std::cout << playerlist.at(i)->getPlayerName() << std::endl;
		const Hand* plHand = playerlist[i]->getCurrentHand();
		for (const auto& card : plHand->getHand())
		{
			std::cout << "\t" << *card << std::endl;
		}
	}
	std::cout << std::endl;
}

/*
 The startup phase the number of player and the number of territory 
*/
void GameEngine::startupPhase() {
	//set start state
	GameEngine::setCurrentState(GAME_STATE_START);
	
	std::cout << "********************************************************************" << std::endl;
	std::cout << "			Welcome to Warzone!" << std::endl;
	std::cout << "********************************************************************\n" << std::endl;
	std::cout << "Please enter \"loadmap <filename>\" in the mentioned format to start playing. " << std::endl;

	std::string user_input;
	// map a key to the value
	std::map<game_user_input, std::string> user_input_list;
	user_input_list[LOADMAP] = "loadmap";
	user_input_list[VALIDATEMAP] = "validatemap";
	user_input_list[ADDPLAYER] = "addplayer";
	user_input_list[GAMESTART] = "gamestart";
	user_input_list[TOURNAMENT] = "tournament";

	//using a loop to read the input until the end of the state
	while (true) {
		//We need to use command processor insted
		Command* userCommand = commandProces->getCommand();
		user_input = userCommand->getCommand();

		//Compare the user input with game_user_input, if valid, update the state; if invalid, reject the command and remain the current state
		switch (*eState) {
		case  GAME_STATE_START:
			if (/*!user_input.substr(0, 7).compare(user_input_list[LOADMAP])*/commandProces->validate(userCommand, user_input_list[LOADMAP])) {
				//do mapload
				filename = extractName(user_input);
				setCurrentState(GAME_STATE_MAP_LOAD);
				userCommand->saveEffect("Passing from  <GAME_STATE_START> to <GAME_STATE_MAP_LOAD>");
				std::cout << "Map loaded!\nPlease try: " << "\"" << user_input_list[VALIDATEMAP] << "\" to validate the current map, or "
					<< "\"" << user_input_list[LOADMAP] << " <filename>\" to load another map." << "\n";
			}
			else if(commandProces->validate(userCommand, user_input_list[TOURNAMENT])) {
				//To do :  add the process to creat the tournament  here
				std::cout << "pass the validation" << std::endl;
				std::vector<std::string> inTournamentCommend = splitString(user_input);
				std::string listMapCommand = "null"; // The list of map file name to load for the tournament;
				std::string listPLayerStrategyCommand = "null" ; // The list of player strategies
				int nbGameCommand = 0; // The number of game 
				int maxNbTurnCommand = 0; //inTournamentCommend.at(8);
				bool valideTournamentCommend = false;
				
				//int a =  std::find(inTournamentCommend.begin(),inTournamentCommend.end() , "-p");


				int tournyCommendSize = inTournamentCommend.size();
				for (int i = 0; i < tournyCommendSize; i++) {
					//find the appropriate pre data word ( EX: -M) and add the  value of the next  word in the associate variable;
					if (inTournamentCommend.at(i) == "-M") {
						i++;
						listMapCommand = extractName(inTournamentCommend.at(i));
					}
					else if (inTournamentCommend.at(i) == "-P") {
						i++;
						listPLayerStrategyCommand = extractName(inTournamentCommend.at(i));
					}
					else if (inTournamentCommend.at(i) == "-G") {
						i++;
						nbGameCommand = std::stoi(extractName(inTournamentCommend.at(i)));
					}
					else if (inTournamentCommend.at(i) == "-D") {
						i++;
						maxNbTurnCommand = std::stoi(extractName(inTournamentCommend.at(i)));
					}
				}
				// Testing if the command is valid / if all the info are correcly entered 

				if (listMapCommand != "null") {
					if (listPLayerStrategyCommand != "null") {
						std::vector<std::string> strategyTypeCheck = splitString(listPLayerStrategyCommand);
						bool typeCheckBool = false;
						for (int j = 0; j < strategyTypeCheck.size(); j++) {
							if (strategyTypeCheck.at(j) == "Human" || strategyTypeCheck.at(j) == "Aggressive" || strategyTypeCheck.at(j) == "Benevolent" || strategyTypeCheck.at(j) == "Neutral" || strategyTypeCheck.at(j) == "Cheater") {
								typeCheckBool = true;
							}
							
							if (typeCheckBool == false) {
								return;
							}
						}
						
						if (nbGameCommand > 0 && nbGameCommand <= 5) {
							if (maxNbTurnCommand >= 10) {
								valideTournamentCommend = true;
							}
						}
					}
				}
				 
				//use this part and all the  extracted parameter to start the tournament 
				if (valideTournamentCommend) {
					userCommand->saveEffect("Creating a tournament with the parameter : -M <" + listMapCommand + ">  -P <" + listPLayerStrategyCommand + ">  -G <" + std::to_string(nbGameCommand) + ">  -D <" + std::to_string(maxNbTurnCommand) + ">.");
					TournamentMode(listMapCommand, listPLayerStrategyCommand, nbGameCommand, maxNbTurnCommand);

					//void GameEngine::TournamentMode(int M, int P, int G, int D) {
				}
				
			}
			else {
				std::cout << "Error input(please try: \"loadmap <filename>)\"\n";
			}
			break;
		case  GAME_STATE_MAP_LOAD:
			if (commandProces->validate(userCommand, user_input_list[LOADMAP])) {
				//do mapload	
				filename = extractName(user_input);
				setCurrentState(GAME_STATE_MAP_LOAD);
				userCommand->saveEffect("Passing from  <GAME_STATE_START> to <GAME_STATE_MAP_LOAD>");
				std::cout << "Map loaded!\nPlease try: " << "\"" << user_input_list[VALIDATEMAP] << "\" to validate the current map, or "
					<< "\"" << user_input_list[LOADMAP] << " <filename>\" to load another map." << "\n";
			}
			else if (commandProces->validate(userCommand, user_input_list[VALIDATEMAP]))
			{
				std::cout << "filename: " << filename << std::endl;
				bool mapload = newmap->MapLoader::loadMap(filename);

				if (mapload == true) {
					mapToUse = newmap->getListMap()->at(0);
					setCurrentState(GAME_STATE_MAP_VALIDATED);
					userCommand->saveEffect("Passing from  <GAME_STATE_MAP_LOAD> to <GAME_STATE_MAP_VALIDATED> : Map validated :"+ filename);
					std::cout << "Map validated!\nPlease enter: \"addplayer\" command to begin adding players." << "\n";
					
				}
				else {
					setCurrentState(GAME_STATE_START);
					userCommand->saveEffect("Passing from  <GAME_STATE_MAP_LOAD> to <GAME_STATE_START>");
					std::cout << "The map is invalid, let's try that again!" << std::endl;
					std::cout << "Please enter \"loadmap <filename>\" in the mentioned format to start playing. " << std::endl;
				}
			}
			else {
				std::cout << "Error input(please try: " << user_input_list[VALIDATEMAP] << " or "
					<< user_input_list[LOADMAP] << "\n";
			}
			break;
		case GAME_STATE_MAP_VALIDATED:
			if (commandProces->validate(userCommand, user_input_list[ADDPLAYER])) {
				addPlayer(user_input.substr(user_input_list[ADDPLAYER].size() + 1, user_input.size()));
				setCurrentState(GAME_STATE_PLAYERS_ADDED);
				userCommand->saveEffect("Passing from  <GAME_STATE_MAP_VALIDATED> to <GAME_STATE_PLAYERS_ADDED> , new player added");
				std::cout << "New player added!\nPlease enter: \"addplayer\" command to continue adding players." << "\n";
			}
			else {
				std::cout << "Error input(please try: \"addplayer <playername>\" to add a player."<< "\n";
			}
			break;
		case GAME_STATE_PLAYERS_ADDED:
			if (playercount < 2) {
				if (commandProces->validate(userCommand, user_input_list[ADDPLAYER]))
				{
					addPlayer(user_input.substr(user_input_list[ADDPLAYER].size() + 1, user_input.size()));
					setCurrentState(GAME_STATE_PLAYERS_ADDED);
					userCommand->saveEffect("Passing from  <GAME_STATE_PLAYERS_ADDED> to <GAME_STATE_PLAYERS_ADDED> , new player added");
					std::cout << "Player added!\nPlease try: " << "\"" << user_input_list[ADDPLAYER] << "\" to add another player, or "
						<< "\"" << user_input_list[GAMESTART] << "\" to begin playing.Duh" << "\n";
				}
				else
				{
					std::cout << "The players are less than 2, there should be 2-6 players in this game." << std::endl;
					std::cout << "Please enter \"addplayer <playername>.\"" << std::endl;
				}
			}
			else if (playercount >= 6) {
				if (commandProces->validate(userCommand, user_input_list[GAMESTART])) {
					GameEngine::gamestart();
					setCurrentState(GAME_STATE_PLAY);
					userCommand->saveEffect("Passing from  <GAME_STATE_PLAYERS_ADDED> to <GAME_STATE_PLAY> , The game will start");
					std::cout << "Player limit reached! There is a limit of 6." << std::endl;
					std::cout << "All set! Ready to play!" << std::endl;
					//TODO: Add variable for maxNumberOfTurns
					int tempMaxTurn = 10;//Samuel temp fix Potato 
					mainGameLoop(playerlist, mapToUse, tempMaxTurn);
				}
				else {
					std::cout << "Error input! The players have reached to upper limit of 6. Please enter gamestart." << std::endl;
				}
			}
			else if (commandProces->validate(userCommand, user_input_list[ADDPLAYER])) {
				addPlayer(user_input);
				setCurrentState(GAME_STATE_PLAYERS_ADDED);
				userCommand->saveEffect("Passing from  <GAME_STATE_PLAYERS_ADDED> to <GAME_STATE_PLAYERS_ADDED> , new player added ");
				std::cout << "Player added!\nPlease try: " << "\"" << user_input_list[ADDPLAYER] << "\" to add another player, or "
					<< "\"" << user_input_list[GAMESTART] << "\" to begin playing." << "\n";

			}
			else if (commandProces->validate(userCommand, user_input_list[GAMESTART])) {
				GameEngine::gamestart();
				setCurrentState(GAME_STATE_PLAY);
				userCommand->saveEffect("Passing from  <GAME_STATE_PLAYERS_ADDED> to <GAME_STATE_PLAY> , The game will start");
				std::cout << "All set! Ready to play!" << std::endl;
				//TODO: Add variable for maxNumberOfTurns
				int tempMaxTurn = 10;//Samuel temp fix Potato
				mainGameLoop(playerlist, mapToUse, tempMaxTurn);
			}
			else {
				std::cout << "Error input(please try: " << user_input_list[ADDPLAYER] << " or "
					<< user_input_list[GAMESTART] << ")\n";
			}
			break;
		case GAME_STATE_PLAY:			
			break;
		default:
			break;
		}
		if (*eState == GAME_STATE_PLAY) {
			break;
		}
	}//end of while loop
}


/*
* The main game loop of the Warzone game
* 
* @param players A vector of players
* @param map The current game map
* @param maxNumberOfTurns The maximum number of turns
*/
void GameEngine::mainGameLoop(std::vector<Player*> players, Map* map, int maxNumberOfTurns)
{
	int turn = 1; //Turn counter
	while (players.size() != 1 && turn < maxNumberOfTurns)
	{ //Loop if there are 2 or more players left
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
		//Increment turn counter
		turn++;
	}//end while
	std::string endGameMessage = (players.size() == 1) ? "Game over, " + players.at(0)->getPlayerName() + " wins\n"
		: "The game has exceeded the amount of turns, therefore the game is a draw.\n";
	std::cout << endGameMessage;
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

const std::vector<Territory*> GameEngine::GetEnemyTerritoriesOfCurrentPlayer(Player* p)
{
	std::vector<Territory*> allEnemyTerritories;
	std::vector<Player*> allPlayers = getPlayerList();
	for (auto& enemy : allPlayers)
	{
		allEnemyTerritories.insert(
			allEnemyTerritories.end(),
			enemy->getTerritoriesOwned().begin(),
			enemy->getTerritoriesOwned().end()
		);
	}

	return allEnemyTerritories;
}

/*
* Issue players orders and place them in their order list
*
* @param *p pointer to a Player object
*/
const void GameEngine::issueOrdersPhase(Player* p, Map* map) {
	p->issueOrder();
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

std::vector<Player*> GameEngine::getPlayerList() const
{
	return this->playerlist;
}

std::vector<Player*>& GameEngine::getPlayerList()
{
	return this->playerlist;
}


/**
* function of TournamentMode in GameEngine class
* @param: int M listofmapfiles, int P listofplayerstrategies, int G numberofgames, int D maxnumberofturns
* @return: void
*/
void GameEngine::TournamentMode(std::string M, std::string P, int G, int D) {
	this->numberGamePrint = G;
	this->drawTurnLimitPrint = D;
	// create map list
	MapLoader* newmap = new MapLoader();
	Map* map = new Map();
	std::vector<Map*> maps;
	listMapName = splitString(M);
	int nbGameFromMap = listMapName.size();


	
	for (int x = 0; x < listMapName.size(); x++) {
		if (newmap->loadMap(listMapName.at(x))) {
			map = newmap->getListMap()->at(x);
			maps.push_back(map);
		}
		else {
			std::cout << "The map in invalid the default one will be use insted.\n ";
			newmap->MapLoader::loadMap("canada");
			exit(1);
			map = newmap->getListMap()->at(0);
			maps.push_back(map);
		}
	}
	/*
	//map1
	newmap->MapLoader::loadMap("canada");
	map = newmap->getListMap()->at(0);
	maps.push_back(map);
	//map2
	newmap->MapLoader::loadMap("caribbean");
	map = newmap->getListMap()->at(0);
	maps.push_back(map);
	//map3
	newmap->MapLoader::loadMap("chutes");
	map = newmap->getListMap()->at(0);
	maps.push_back(map);
	//map4
	newmap->MapLoader::loadMap("myImortal");
	map = newmap->getListMap()->at(0);
	maps.push_back(map);
	//map5
	newmap->MapLoader::loadMap("simpsons_world");
	map = newmap->getListMap()->at(0);
	maps.push_back(map);
	*/
	listPlayerFromConsole = splitString(P);
	Player* p = nullptr;
	std::vector<Player*> playerlist;
	std::string playerName;
	for (int y = 0; y < listPlayerFromConsole.size(); y++) {
		playerName = "player " + std::to_string(y);
		if (listPlayerFromConsole.at(y) == "Human") {
			
			p = new Player(playerName);
		}
		if (listPlayerFromConsole.at(y) == "Aggressive") {
			p = new Player(new AggressivePlayerStrategy(), playerName);

		}
		if (listPlayerFromConsole.at(y) == "Benevolent") {
			p = new Player(new BenevolentPlayerStrategy(), playerName);

		}
		if (listPlayerFromConsole.at(y) == "Neutral") {
			p = new Player(new NeutralPlayerStrategy(), playerName);

		}
		if (listPlayerFromConsole.at(y) == "Cheater") {
			p = new Player(new CheaterPlayerStrategy(), playerName);

		}
		playerlist.push_back(p);

	}
	/*
	// create player list
	Player* p1 = new Player (new AggressivePlayerStrategy());
	
	playerlist.push_back(p1);
	Player* p2 = new Player(new BenevolentPlayerStrategy());
	//p1->setPlayerStrategy();
	playerlist.push_back(p2);

	Player* p3 = new Player(new NeutralPlayerStrategy());	
	playerlist.push_back(p3);
	
	Player* p4 = new Player(new CheaterPlayerStrategy());
	playerlist.push_back(p4);
	*/

	// create current player list
	std::vector<Player*> currentPlayerlist;
	
	for (int i = 0; i < 2/*P*/; i++) {
		currentPlayerlist.push_back(playerlist.at(i));
	}


	Map* currentMap = nullptr;
	// outer loop for listofmapfiles
	std::string gameresult = "Result :\n";
	std::string winer = "";
	for (int i = 0; i < nbGameFromMap; i++) {
		currentMap = maps.at(i);
		gameresult = "Map " + std::to_string(i) + " : ";
		// inner loop for numberofgames
		
		for (int j = 0; j < G; j++) {
			for (int i = 0; i < 2/*P*/; i++) {
				currentPlayerlist.push_back(playerlist.at(i));
			}
			
			/*winer =*/mainGameLoop(currentPlayerlist, currentMap, D);

			gameresult = gameresult + "      " + winer;
			currentPlayerlist.clear();
		}//end of inner loop
		gameresult = gameresult + "\n";
	}//end of outer loop
	setCurrentState(GAME_STATE_FINISHED);
}//end of TournamentMode
//<<<<<<< HEAD