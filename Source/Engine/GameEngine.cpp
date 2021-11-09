#include "GameEngine.h"
#include <map>
#include <string>
using namespace std;
#include <iostream>

/**
 * constructor of GameEngine class
 */
GameEngine::GameEngine()
{
    eState = new GameState(GAME_STATE_UNKNOWN);
#ifdef DEBUG_ENABLE
    cout<<"constructor\n";
#endif
}
/**
 * Copy constructor of GameEngine class
 */
GameEngine::GameEngine(const GameEngine &obj)
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
    cout<<"destructor\n";
#endif
}

/**
 * setter
 * @param lState local state of GameState
 */
void GameEngine::setCurrentState(GameState lState)
{
   *eState = lState;
   this->Notify(*this);
}
/**
 * getter
 * @return lState local state of GameState
 */
GameState GameEngine:: getCurrentState()
{
    return *eState;
}

/**
 * Assignment operator of GameEngine class
 */

GameEngine &GameEngine::operator=(const GameEngine &obj)
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

                    cout << "Error input(please try: "<< user_input_list[VALIDATEMAP] << " or "
                        << user_input_list[LOADMAP] <<"\n" ;
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
                    cout << "Error input(please try: " << user_input_list[ADDPLAYER] <<" or "
                        << user_input_list[ASSIGNCOUNTRIES] << ")\n";
                }
                break;
            case GAME_STATE_ASSIGN_REINFORCEMENT:

                if (!user_input.compare(user_input_list[ISSUEORDER])) {
                    setCurrentState(GAME_STATE_ISSUE_ORDERS);

                }
                else {
                    cout << "Error input(please try: " << user_input_list[ISSUEORDER] <<")\n";
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
                        << user_input_list[ENDEXECORDERS] <<" or "
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


std::string GameEngine::stringToLog() {
    std::string toLog;

    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch (this->getCurrentState()) {
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

    return  s;
    //toLog  << this->getCurrentState();
    //return toLog;
}
dammy::dammy() {

    this->name = "paule";
}

void dammy::changeName(std::string text) {
    this->name = text;
    this->Notify(*this);
}

std::string dammy::stringToLog() {
    return this->name;
}