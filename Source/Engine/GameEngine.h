#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <string>

#include "Player/Player.h"
#include "../GameLog/LoggingObserver.h"
#include "../CommandProcessor/CommandProcessor.h"

class Player;
class Map;

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

class GameEngine : virtual public Subject, virtual  public ILoggable /*,  public CommandProcessor*/ {
public:
    GameEngine(); //constructor

    GameEngine(Observer*); //constructor
    ~GameEngine(); //destructor
    CommandProcessor* commandProces;
    using Subject::Notify;
    GameEngine(const GameEngine& gm); //copy constructor
    GameState getCurrentState(); //getter

    void game_run();
    virtual std::string stringToLog() override;
    GameEngine& operator=(const GameEngine& obj);//Assignment operator

    Player* getNeutralPlayer() const;
    std::vector<Player*> getPlayerList() const;
    std::vector<Player*>& getPlayerList();

    void mainGameLoop(std::vector<Player*> players, Map* map); //Game loop function

    void startupPhase();

    const std::vector<Territory*> GetEnemyTerritoriesOfCurrentPlayer(Player* p);
private:
    GameState* eState;
    void setCurrentState(GameState eState);
    const void reinforcementPhase(Player* p, Map* m);
    /* issueOrdersPhase methods */
    // issueOrdersPhase
    const void issueOrdersPhase(Player* p, Map* map);
    const void executeOrdersPhase(Player* p);
    void gamestart();
    std::string extractName(std::string);//extract name from loadmap and addplayer command
    void addPlayer(std::string);//add player


    std::vector<Player*> playerlist;
    Player* neutralPlayer = nullptr;
};




