#include "CommandProcessor.h"
#include "../Engine/GameEngine.h"
#include <map>

Command::Command() : command(""), effect("")
{

}

Command::Command(std::string newCommand, std::string newState) : command(newCommand), effect(newState)
{

}

// the copy constructor will initialize an object using another object of the same class 

Command::Command(const Command& newCommandObject)
{
	this->command = newCommandObject.command;
	this->effect = newCommandObject.effect;
}

//  the assignment operator replaces the contents of existing objects

Command& Command::operator=(const Command& newCommandObject)
{
	if (this == &newCommandObject)
		return *this;

	this->command = newCommandObject.command;
	this->effect = newCommandObject.effect;


	return *this;
}

std::string Command::getCommand() const
{
	return command;
}

void Command::saveCommand(std::string command)
{
	this->command = command; 
}

std::string Command::getEffect() const
{
	return effect; 
}

void Command::saveEffect(std::string effect)
{
	this->effect = effect; 
}

std::ostream& operator<<(std::ostream& os, const Command& newCommandObject)
{
	os << "Command: " << newCommandObject.getCommand() << ", "
		<< "State: " << newCommandObject.getEffect() << ".";

	return os;
}

CommandProcessor::CommandProcessor(){

}
/*
CommandProcessor::CommandProcessor() : gameEngine(new GameEngine()), listOfCommands()
{
	std::map<game_user_input, string> user_input_list;
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
}
*/

CommandProcessor::~CommandProcessor()
{
	delete gameEngine; 
	gameEngine = nullptr; 

	for (auto p : listOfCommands)
	{
		delete p;

	}
	listOfCommands.clear();

}

Command* CommandProcessor::getCommand()
{
	string input = readCommand(); // input will be assigned whatever was taken from the user 
	
	return saveCommand(input); // input will be passed in and saved into the list of commands 
	 //return the current object 
}



Command* CommandProcessor::saveCommand(std::string fromReadCommand)
{
	Command* aCommand = new Command(fromReadCommand, "");
	listOfCommands.push_back(aCommand);
	cout << "The command : " << aCommand << " will now be saved into the list of commands " << endl;
	return aCommand; 
	
}

std::string CommandProcessor::readCommand()
{
	// not to clear as to why it is private 

	std::string inputCommand; 
	cout << "Please type your command with lower-case letters:" << "\n";
	std::getline(std::cin, inputCommand);
	

	cout << "The command : " << inputCommand << " will now be passed to saveCommand(). " << endl; 

	return inputCommand; 

}

GameEngine* CommandProcessor::getGameEngine(){
	return gameEngine;
}

bool CommandProcessor::validate(Command * c){

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

	switch(this->getGameEngine()->getCurrentState()){
	case GAME_STATE_START:
		if(c->getCommand().compare(user_input_list[LOADMAP])){
			return true;
		}
		else{
			return false;
		}
		break;
	default:
		break;

	//c->getCommand()

	}


}



