#ifndef GameEngine_h
#define GameEngine_h
#include <iostream>
using namespace std;

//game state
enum GameState
{
    GAME_STATE_UNKNOWN = 0,
    GAME_STATE_START = 1,
    GAME_STATE_MAP_LOAD = 2,
    GAME_STATE_MAP_VALIDATED = 3,
    GAME_STATE_PLAYERS_ADDED = 4,
    GAME_STATE_PLAY = 5,
    GAME_STATE_MAX = 0XFFFF,//set a max value to prevent overflow
};
//user input
enum game_user_input {
    LOADMAP,
    VALIDATEMAP,
    ADDPLAYER,
    GAMESTART,
    
};

//GameEngine class
class GameEngine {
public:
    GameEngine(); //constructor
    ~GameEngine(); //destructor
    GameEngine(const GameEngine& gm); //copy constructor
    GameState getCurrentState(); //getter
    GameEngine& operator=(const GameEngine& obj);//Assignment operator
    friend ostream& operator<<(ostream& out, const GameState value);//stream insertion operator
    void startupPhase();
private:
    GameState* eState;
    void setCurrentState(GameState eState);//setter
    string extractName(string);//extract name from loadmap and addplayer command
    void addPlayer(string);//add player
    void gamestart();//gamestart main function
};
#endif
