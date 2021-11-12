#include "GameEngine.h"
#include "map.h"
#include "Cards.h"
#include "Orders.h"
#include "Player.h"
#include <map>
#include <string>
using namespace std;
#include <iostream>
#include <random>
#include <iterator>

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
    if (&obj != this) {
        delete eState;
        this->eState = new GameState(*obj.eState);
    }
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
        PROCESS_VAL(GAME_STATE_PLAY);
       
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


static string filename;
static string playername;
static int playercount = 0;
MapLoader* newmap = new MapLoader();
Map* mapToUse = new Map();
vector<Player*> playerlist;
Deck* newDeck = new Deck(30);
Card* newCard = newDeck->drawCard_Deck();
vector<int> ReinforcementPools;

/**
 * private function extractName(string) of GameEngine class
 * @param string str user input
 * @return string name
 */
string GameEngine::extractName(string str) {

    unsigned first = str.find("<");
    unsigned last = str.find(">");
    string strNew = str.substr(first + 1, last - first - 1);
    return strNew;
}

/**
 * private function addPlayer(string) of GameEngine class
 * @param string str user input
 * @return void
 */

void GameEngine::addPlayer(string user_input) {

    playername = extractName(user_input);
    Player* p = new Player(playername);
    playerlist.push_back(p);
    playercount++;
    cout << "The player " << playername << " is added." << endl;
    cout << "There should be 2-6 players in this game. Now we have " << playercount << " players." << endl;
    cout << endl << "Player#    PlayerName" << endl;
    for (int i = 0; i < (int)playerlist.size(); i++) {
        cout << i + 1 << "             " << playerlist.at(i)->getPlayerName() << endl;
    }
    cout << endl;
    
}

/**
 * private function gamestart() of GameEngine class
 * @param blank
 * @return void
 */

void GameEngine::gamestart() {

    //a) fairly distribute all the territories to the players

    int numberOfTerritory = mapToUse->listTerritory.size();
    cout << endl << "The numberOfTerritory in the map is: " << numberOfTerritory << endl;
    Territory* t = new Territory();
    int round = numberOfTerritory / playercount;
    for (int ii = 0; ii < round; ii++) {
        for (int i = 0; i < playercount; i++) {
            t = mapToUse->listTerritory.at(i + ii * playercount);
            playerlist[i]->getTerritoriesOwned().push_back(t);
        }
    }
    int remainder = numberOfTerritory % playercount;
    for (int i = 0; i < remainder; i++) {
        t = mapToUse->listTerritory.at(round * playercount);
        playerlist[i]->getTerritoriesOwned().push_back(t);

    }

    cout << "Fairly distribute all the territories to the players>>>>>>>" << endl;
    for (int i = 0; i < playercount; i++) {
        cout << *(playerlist.at(i)) << endl;
    }
   
    //b) determine randomly the order of play of the players in the game
    cout << endl << "Determine randomly the order of play of the players in the game>>>>>>" << endl;
    cout << "Before shuffle, the order of play is: ";
    for (vector<Player*>::iterator it = playerlist.begin(); it != playerlist.end(); ++it) {
        cout << " " << (*it)->getPlayerName();
    }

    random_shuffle(playerlist.begin(), playerlist.end());

    cout << endl << "After shuffle, the order of play is: ";
    for (vector<Player*>::iterator it = playerlist.begin(); it != playerlist.end(); ++it) {
        cout << " " << (*it)->getPlayerName();
    }
    cout << endl;

    //c) give 50 initial armies to the players, which are placed in their respective reinforcement pool
    cout << endl << "Give 50 initial armies to the players>>>>>>" << endl;
    for (int i = 0; i < playercount; i++) {
        ReinforcementPools.push_back(50);
    }
    cout << "player name        quantity of armies" << endl;
    for (int i = 0; i < playercount; i++) {
        cout << playerlist.at(i)->getPlayerName();
        cout << "                       ";
        cout << ReinforcementPools.at(i) << endl;
    }
    cout << endl;
    //d) let each player draw 2 initial cards from the deck using the deck’s draw() method

    for (int i = 0; i < playercount; i++) {

        playerlist.at(i)->getCurrentHand()->insertCard_Hand(newCard);
        newCard = newDeck->drawCard_Deck();
        playerlist.at(i)->getCurrentHand()->insertCard_Hand(newCard);

    }
    cout << "Let each player draw 2 initial cards from the deck>>>>>>>" << endl;
    cout << "players' initial cards are: " << endl;
    cout << "player name        cards" << endl;
    for (int i = 0; i < playercount; i++) {

        cout << playerlist.at(i)->getPlayerName() << "              ";
        playerlist.at(i)->getCurrentHand()->showHand();
        cout << endl;
    }

}

/**
* function startupPhase() of GameEngine class, the phase to set up before playing
* @param blank
* @return void
*/

void GameEngine::startupPhase() {
    //set start state
    GameEngine::setCurrentState(GAME_STATE_START);

    cout << "Please enter loadmap <filename> in the mentioned format: " << endl;
    string user_input;
    // map a key to the value
    map<game_user_input, string> user_input_list;
    user_input_list[LOADMAP] = "loadmap";
    user_input_list[VALIDATEMAP] = "validatemap";
    user_input_list[ADDPLAYER] = "addplayer";
    user_input_list[GAMESTART] = "gamestart";
    
    //using a loop to read the input until the end of the state
    while (true) {

        user_input = get_user_input(*eState);

        //compare the user input with game_user_input, if valid, update the state; if invalid, reject the command and remain the current state
        switch (*eState) {

        case  GAME_STATE_START:
            if (!user_input.substr(0, 7).compare(user_input_list[LOADMAP])) {
                //do mapload
                filename = extractName(user_input);               
                setCurrentState(GAME_STATE_MAP_LOAD);
            }
            else {

                cout << "Error input(please try: loadmap <filename>)\n";
            }
            break;
        case  GAME_STATE_MAP_LOAD:
            if (!user_input.substr(0, 7).compare(user_input_list[LOADMAP])) {
                //do mapload
                filename = extractName(user_input);
                setCurrentState(GAME_STATE_MAP_LOAD);                
            }
            else if (!user_input.compare(user_input_list[VALIDATEMAP])) {
                
                
                cout << "filename: " << filename << endl;
                bool mapload = newmap->MapLoader::loadMap(filename);

                if (mapload == true) {
                    mapToUse = newmap->getListMap()->at(0);
                    setCurrentState(GAME_STATE_MAP_VALIDATED);
                }
                else {
                    setCurrentState(GAME_STATE_START);
                }
   
            }
            else {

                cout << "Error input(please try: " << user_input_list[VALIDATEMAP] << " or "
                    << user_input_list[LOADMAP] << "\n";
            }
            break;
        case GAME_STATE_MAP_VALIDATED:

            if (!user_input.substr(0,9).compare(user_input_list[ADDPLAYER])) {
                
                addPlayer(user_input);
                setCurrentState(GAME_STATE_PLAYERS_ADDED);
            }
            else {
                cout << "Error input(please try: " << user_input_list[ADDPLAYER] << "\n";
            }
            break;

        case GAME_STATE_PLAYERS_ADDED:
            if (playercount < 2) {
                if (!user_input.substr(0, 9).compare(user_input_list[ADDPLAYER])) {
                        
                    addPlayer(user_input);
                    setCurrentState(GAME_STATE_PLAYERS_ADDED);
                }
                else {
                    cout << "The players are less than 2, there should be 2-6 players in this game." << endl;
                    cout << "Please enter addplayer <playername>." << endl;
                }
            }
           
            else if (playercount >= 6) {
                if (!user_input.compare(user_input_list[GAMESTART])) {
                    
                    GameEngine::gamestart();
                    setCurrentState(GAME_STATE_PLAY);
                }
                else {
                    cout << "Error input! The players have reached to upper limit of 6. Please enter gamestart." << endl;
                }
                               
            }
            else if (!user_input.substr(0, 9).compare(user_input_list[ADDPLAYER])) {
                
                addPlayer(user_input);
                setCurrentState(GAME_STATE_PLAYERS_ADDED);

            }
            else if (!user_input.compare(user_input_list[GAMESTART])) {
                                
                GameEngine::gamestart();
                setCurrentState(GAME_STATE_PLAY);
            }
            else {
                cout << "Error input(please try: " << user_input_list[ADDPLAYER] << " or "
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

    delete newmap;
    delete newDeck;
    delete newCard;
    playerlist.clear();
    playerlist.shrink_to_fit();
}//end of startupPhase()

