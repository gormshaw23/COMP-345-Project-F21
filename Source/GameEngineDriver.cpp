#include "Engine/GameEngine.h"
using namespace std;
#include <iostream>


int main() {

  cout << "Welcome to Warzone!\n";
  cout << "Please refer the table below for the command.\n\n"
           "    state                              command option(s)\n"
           "GAME_STATE_START                   loadmap\n"
           "GAME_STATE_MAP_LOAD                loadmap / validatemap\n"
           "GAME_STATE_MAP_VALIDATED           addplayer\n"
           "GAME_STATE_PLAYERS_ADDED           addplayer / assigncountries\n"
           "GAME_STATE_ASSIGN_REINFORCEMENT    issueorder\n"
           "GAME_STATE_ISSUE_ORDERS            issueorder / endissueorders\n"
           "GAME_STATE_EXECUTE_ORDERS          endexecorders / execorder / win\n"
           "GAME_STATE_WIN                     play / end\n" << endl;
  GameEngine gameobj;
  gameobj.game_run();
  cout << "Good Game!\nGood Bye!" << endl;
  return 0;
}
