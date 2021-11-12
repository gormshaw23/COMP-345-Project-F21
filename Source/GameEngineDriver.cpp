#include "GameEngine.h"
#include "Cards.h"
#include "CommonTypes.h"
#include "map.h"
#include "Orders.h"
#include "Player.h"
using namespace std;
#include <iostream>


int main() {

    cout << "Welcome to Warzone!\n";
    cout << "This is the StartUp Phase!\nPlease refer the table below for the command.\n\n"
        "    state                              command option(s)\n"
        "GAME_STATE_START                   loadmap <filename>\n"
        "GAME_STATE_MAP_LOAD                loadmap <filename>/ validatemap\n"
        "GAME_STATE_MAP_VALIDATED           addplayer <playername>\n"
        "GAME_STATE_PLAYERS_ADDED           addplayer <playername>/ gamestart\n"
        "GAME_STATE_GAMESTART               \n"<< endl;
    GameEngine gameobj;
    gameobj.startupPhase();
    cout << endl << "Now it's time to PLAY Warzone!\nEnjoy!!" << endl;
    return 0;
}