#ifndef GameEngine_h
#define GameEngine_h
#include <iostream>
#include <list>

#include "Player/Player.h"
#include "Map/map.h"

using namespace std;

//game state
enum GameState
{
    GAME_STATE_UNKNOWN=0,
    GAME_STATE_START=1,
    GAME_STATE_MAP_LOAD=2,
    GAME_STATE_MAP_VALIDATED=3,
    GAME_STATE_PLAYERS_ADDED=4,
    GAME_STATE_ASSIGN_REINFORCEMENT=5,
    GAME_STATE_ISSUE_ORDERS=6,
    GAME_STATE_EXECUTE_ORDERS=7,
    GAME_STATE_WIN=8,
    GAME_STATE_END=9,
    GAME_STATE_MAX=0XFFFF,//set a max value to prevent overflow
};
//user input
enum game_user_input {
    LOADMAP,
    VALIDATEMAP,
    ADDPLAYER,
    ASSIGNCOUNTRIES,
    ISSUEORDER,
    ENDEXECORDERS,
    EXECORDER,
    ENDISSUEORDERS,
    WIN,
    PLAY,
    END,
};

//class Player;

//GameEngine class
class GameEngine{
    public:
        GameEngine(); //constructor
        ~GameEngine(); //destructor
        GameEngine(const GameEngine &gm); //copy constructor
        GameState getCurrentState(); //getter
        void game_run();
        GameEngine &operator=(const GameEngine &obj);//Assignment operator
        friend ostream &operator<<(ostream &out, const GameState value);//stream insertion operator

        void mainGameLoop(list<Player*> players, Map* map); //Game loop function
        list<Player*> getPlayers_temp(); //Temporary getter for list of players

    private:
        GameState* eState;
        void setCurrentState(GameState eState);//setter
        void reinforcementPhase(Player* p, Map* m);
        void issueOrdersPhase(Player* p, Map* map);
        void executeOrdersPhase(Player* p);

        list<Player*> players_temp; //Temporary variable for list of players
};
#endif

