#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <string>

#include "../GameLog/LoggingObserver.h"

class Territory;
class Player;
class Map;
class CommandProcessor;

//game state
enum GameState
{
    GAME_STATE_UNKNOWN = 0,
    GAME_STATE_START = 1,
    GAME_STATE_MAP_LOAD = 2,
    GAME_STATE_MAP_VALIDATED = 3,
    GAME_STATE_PLAYERS_ADDED = 4,
    GAME_STATE_PLAY = 5,
    GAME_STATE_FINISHED = 6,
    GAME_STATE_MAX = 0XFFFF,//set a max value to prevent overflow

};
//user input
enum game_user_input {
    LOADMAP,
    VALIDATEMAP,
    ADDPLAYER,
    GAMESTART,
    TOURNAMENT,
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

    /* variable to print the tournament result*/
    std::vector<std::string> listPlayerFromConsole;
    std::vector<std::string> listMapName;
    int numberGamePrint;
    int drawTurnLimitPrint;
    std::string gameresult;


    Player* getNeutralPlayer() const;
    const std::vector<Player*>& getPlayerList() const;
    std::vector<Player*>& getPlayerList();

    std::string mainGameLoop(std::vector<Player*> players, Map* map, int maxNumberOfTurns); //Game loop function

    void startupPhase();

    void TournamentMode(std::string M, std::string P, int G, int D); //map and player set up for tournament mode
    void TournamentMode(int M, int P, int G, int D); //map and player set up for tournament mode

    bool getIsATournament();
    void setIsATournament(bool tournamentValue);

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
    std::string extractPlayerStrategy(std::string str);
    void addPlayer(std::string);//add player

    std::vector<Player*> playerlist;
    Player* neutralPlayer = nullptr;

    bool isATournament; //Determines if a game is part of a tournament
};
