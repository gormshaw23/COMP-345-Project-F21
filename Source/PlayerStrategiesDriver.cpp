#include "Strategy/PlayerStrategies.h"
#include "Player/Player.h"
#include "Engine/GameEngine.h"
#include "Map/map.h"
#include "Cards/Cards.h"
#include "GameLog/LoggingObserver.h"
#include "CommandProcessor/CommandProcessor.h"

#include <iostream>
#include <vector>

using std::vector;

#ifdef PLAYERSTRATEGYDRIVER
int main()
{
	std::list<Subject*>* _listSubject = new std::list<Subject*>;

	static LogObserver* loglog = new  LogObserver(_listSubject);

	GameEngine* ge = new GameEngine(loglog);
	ge->startupPhase();

	delete ge;

	return 0;
}
#endif