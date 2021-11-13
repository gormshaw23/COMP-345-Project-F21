#include "GameEngine.h"
#include "CommandProcessor/CommandProcessor.h"
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
   //Notify(*this);
   GameEngine::Subject::Notify(*this);
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
                    cout << "Error input(please try: loadmap)\n";
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
                    cout << "Error input(please try: "<< user_input_list[VALIDATEMAP] << " or "
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
                    cout << "Error input(please try: " << user_input_list[ADDPLAYER] << "\n";
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
                    cout << "Error input(please try: " << user_input_list[ADDPLAYER] <<" or "
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
                    cout << "Error input(please try: " << user_input_list[ISSUEORDER] <<")\n";
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
                    cout << "Error input(please try: " << user_input_list[ISSUEORDER] << " or "
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
                    cout << "Error input(please try: " << user_input_list[EXECORDER] << " or "
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
                    cout << "Error input(please try: " << user_input_list[PLAY] << " or "
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
