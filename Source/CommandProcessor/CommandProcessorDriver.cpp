

#include "../Engine/GameEngine.h"
#include "CommandProcessor.h" 

#define _DEBUG
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

int main() {

	
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	GameEngine* gE = new GameEngine();
	gE->game_run();

	 
	 
}
