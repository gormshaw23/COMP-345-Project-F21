#include "gameEngine.h"
#define DEBUG_ENABLE 1
#include<map>
#include <string>
using namespace std;
#include <iostream>

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

GameState gState;

gameEngine::gameEngine()
{
    eState =&gState;
    *eState = GAME_STATE_UNKNOWN;
#ifdef DEBUG_ENABLE
    cout<<"constructor\n";
#endif
}

void gameEngine::setCurrentState(GameState lState)
{
   *eState = lState;
}

GameState gameEngine:: getCurrentState()
{
    return*eState;
}


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

string get_user_input(GameState lState) {
    string Name;
    cout << "current game state is (" << lState << ")\n";
    cout << "please enter your selection" << "\n";
    getline(cin, Name);
    return Name;
}


void gameEngine::game_run() {

    gameEngine::setCurrentState(GAME_STATE_START);
    string user_input;

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

    while (true) {

        user_input = get_user_input(*eState);
    //step1 user input
        switch (*eState) {

            case  GAME_STATE_START:
                if (!user_input.compare(user_input_list[LOADMAP])) {
                    setCurrentState(GAME_STATE_MAP_LOAD);
                }
                else {

                    cout << "error input(try: loadmap)\n";
                }
                break;
            case  GAME_STATE_MAP_LOAD:
                if (!user_input.compare(user_input_list[LOADMAP])) {
                    setCurrentState(GAME_STATE_MAP_LOAD);
                    //TODO LOAD MAP
                }
                else if (!user_input.compare(user_input_list[VALIDATEMAP])) {
                    setCurrentState(GAME_STATE_MAP_VALIDATED);
                }
                else {

                    cout << "error input(try: <<"<< user_input_list[VALIDATEMAP] << " or "
                        << user_input_list[LOADMAP] <<"\n" ;
                }
                break;
            case GAME_STATE_MAP_VALIDATED:
                if (!user_input.compare(user_input_list[ADDPLAYER])) {
                    setCurrentState(GAME_STATE_PLAYERS_ADDED);
                    //TODO LOAD MAP
                }
                else {
                    cout << "error input(try: <<" << user_input_list[ADDPLAYER] << "\n";
                }
                break;

            case GAME_STATE_PLAYERS_ADDED:
                if (!user_input.compare(user_input_list[ADDPLAYER])) {
                    setCurrentState(GAME_STATE_PLAYERS_ADDED);
                    //TODO LOAD MAP
                }
                else if (!user_input.compare(user_input_list[ASSIGNCOUNTRIES])) {
                    setCurrentState(GAME_STATE_ASSIGN_REINFORCEMENT);
                }
                else {
                    cout << "error input(try: " << user_input_list[ADDPLAYER] <<" or "
                        << user_input_list[ASSIGNCOUNTRIES] << ")\n";
                }
                break;
            case GAME_STATE_ASSIGN_REINFORCEMENT:

                if (!user_input.compare(user_input_list[ISSUEORDER])) {
                    setCurrentState(GAME_STATE_ISSUE_ORDERS);
                    //TODO LOAD MAP
                }
                else {
                    cout << "error input(try: " << user_input_list[ISSUEORDER] <<")\n";
                }

                break;
            case GAME_STATE_ISSUE_ORDERS:
                if (!user_input.compare(user_input_list[ISSUEORDER])) {
                    setCurrentState(GAME_STATE_ISSUE_ORDERS);
                    //TODO LOAD MAP
                }
                else if (!user_input.compare(user_input_list[ENDISSUEORDERS])) {
                    setCurrentState(GAME_STATE_EXECUTE_ORDERS);
                }
                else {
                    cout << "error input(try: " << user_input_list[ISSUEORDER] << " or "
                        << user_input_list[ENDISSUEORDERS] << ")\n";
                }
                break;

            case GAME_STATE_EXECUTE_ORDERS:
                if (!user_input.compare(user_input_list[EXECORDER])) {
                    setCurrentState(GAME_STATE_EXECUTE_ORDERS);
                    //TODO LOAD MAP
                }
                else if (!user_input.compare(user_input_list[ENDEXECORDERS])) {
                    setCurrentState(GAME_STATE_ASSIGN_REINFORCEMENT);
                }
                else if (!user_input.compare(user_input_list[WIN])) {
                    setCurrentState(GAME_STATE_WIN);
                }
                else {
                    cout << "error input(try: " << user_input_list[EXECORDER] << " or "
                        << user_input_list[ENDEXECORDERS] <<" or "
                        << user_input_list[WIN] << ")\n";
                }
                break;
            case GAME_STATE_WIN:
                if (!user_input.compare(user_input_list[PLAY])) {
                    setCurrentState(GAME_STATE_START);
                    //TODO LOAD MAP
                }
                else if (!user_input.compare(user_input_list[END])) {
                    setCurrentState(GAME_STATE_END);
                }
                else {
                    cout << "error input(try: " << user_input_list[PLAY] << " or "
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













       // break;



    }





}
