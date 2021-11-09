#include "CommandProcessor.h"
#include "Engine/GameEngine.h"


//Command class implementation 

Command::Command() :command(""), state("")
{
}

Command::Command(std::string newCommand, std::string newState) : command(newCommand), state(newState)
{
}

Command::Command(const Command& newCommandObject)
{
	this->command = newCommandObject.command;
	this->state = newCommandObject.state;
}

Command& Command::operator=(const Command& newCommandObject)
{
	if (this == &newCommandObject)
		return *this;

	this->command = newCommandObject.command;
	this->state = newCommandObject.state;

	return *this;


}

std::string Command::getCommand()const
{
	return command;
}

std::string Command::getState() const
{
	return state;
}

void Command::saveState(std::string newState)
{
	this->state = newState;
}

std::ostream& operator<<(std::ostream& os, const Command& newCommandObject) {

	os << "Command: " << newCommandObject.getCommand() << ", "
		<< "State: " << newCommandObject.getState() << ".";

	return os;
}

// CommandProcessor class implementation 

CommandProcessor::CommandProcessor()
{
	listOfCommands = std::vector<Command*>(); 

}

CommandProcessor::~CommandProcessor()
{
	listOfCommands.clear();
	listOfCommands.shrink_to_fit();
}


CommandProcessor::CommandProcessor(const CommandProcessor& newCP)
{

	if (this == &newCP)
		return; 

	for (int i = 0; i < newCP.listOfCommands.size(); i++)
	{
		this->listOfCommands.push_back(new Command(*newCP.listOfCommands[i]));
	}
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& newCP)
{
	if (this == &newCP)
		return *this;

	for (int i = 0; i < newCP.listOfCommands.size(); i++)
	{
		this->listOfCommands.push_back(new Command(*newCP.listOfCommands[i]));
	}

	return *this;
}

Command* CommandProcessor::getCommand()
{
	std::string input = readCommand();
	Command* com = saveCommand(input);
	return com;

}

void CommandProcessor::showCommands()
{
	for (int i = 0; i < listOfCommands.size(); i++) {

		cout << listOfCommands.at(i) << std::endl;
	}
}

std::string CommandProcessor::readCommand()
{
	{
		std::string userInput;
		std::getline(std::cin, userInput);
		return userInput;
	}
}

Command* CommandProcessor::saveCommand(std::string fromReadCommand)
{
	{
		Command* aCommand = new Command(fromReadCommand, "");
		listOfCommands.push_back(aCommand);
		return aCommand;
	}
}

void CommandProcessor::validate(std::string aCommand)
{
		//aCommand.compare(LOADMAP)
	if(aCommand.compare(LOADMAP)  

		// first we compare the acommand with the valid allowed command in the game
		//if the commande is valid, we check if the command if valid for the current state
		// to get the current state we get the last commande inf the vector and the property state/ effect of that command
		// if the command is valid in the current state, we create a new command with new effect/state and the last commande 
		// we save the command in the vector

}




