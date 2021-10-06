#ifndef gameEngine_h
#define gameEngine_h

enum GameState
{
    GAME_STATE_UNKNOWN=0,
    GAME_STATE_START=1,
    GAME_STATE_MAP_LOAD=2,
    GAME_STATE_MAP_VALIDATED=3,
    GAME_STATE_PLAYERS_ADDED,
    GAME_STATE_ASSIGN_REINFORCEMENT,
    GAME_STATE_ISSUE_ORDERS,
    GAME_STATE_EXECUTE_ORDERS,
    GAME_STATE_WIN,
    GAME_STATE_END,
    GAME_STATE_MAX=0XFFFF,
};




class gameEngine{
    public:

        gameEngine();
        GameState getCurrentState();
        void game_run();
    private:
        GameState* eState;
        void setCurrentState(GameState eState);

};
#endif
