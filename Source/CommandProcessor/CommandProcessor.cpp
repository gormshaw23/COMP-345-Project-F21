
#include <map>
#include <iostream>
#include <fstream>

#include "CommandProcessor.h"
#include "Engine/GameEngine.h"

using std::string;
using std::cout;
using std::endl;


Command::Command() : command(""), effect("")
{

}
Command::Command(Observer* obser) : command(""), effect("")
{
	std::cout << "In the command constructor\n";
	this->Attach(obser);
}

Command::Command(std::string newCommand, std::string newState, Observer* obser) : command(newCommand), effect(newState)
{
	this->Attach(obser);
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
	Command::Notify(*this);
}

std::string Command::getEffect() const
{
	return effect;
}

void Command::saveEffect(std::string effect)
{
	this->effect = effect;
}

std::string Command::stringToLog() {
	return  "<Command> Command :" + this->command + " with the effect : " + this->effect;
}

std::ostream& operator<<(std::ostream& os, const Command& newCommandObject)
{
	os << "Command: " << newCommandObject.getCommand() << ", "
		<< "State: " << newCommandObject.getEffect() << ".";

	return os;
}

CommandProcessor::CommandProcessor() {
	
}

CommandProcessor::CommandProcessor(Observer* observ) {
	std::cout << "In the command processor";
	this->Attach(observ);
}

CommandProcessor::~CommandProcessor()
{
	

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
	Observer*  obser = getObserver()->front();
	Command* aCommand = new Command(fromReadCommand, "", obser);
	
	std::list<Observer* >::iterator i = this->getObserver()->begin();
	for (; i != this->getObserver()->end(); ++i) {
		
	}
	listOfCommands.push_back(aCommand);
	commandINMemmory = fromReadCommand;
	cout << "The command : " << aCommand->getCommand() << " will now be saved into the list of commands " << endl;
	this->Notify(*this);
	return aCommand;
	

}


std::string CommandProcessor::stringToLog() {

	return "<CommandProcessor> Command saved : " + this->commandINMemmory;
}

std::string CommandProcessor::readCommand()
{
	// not to clear as to why it is private 

	std::string inputCommand;
	std::getline(std::cin, inputCommand);


	cout << "The command : " << inputCommand << " will now be passed to saveCommand(). " << endl;

	return inputCommand;

}



bool CommandProcessor::validate(Command* c, std::string gameState) {


	if (gameState.compare("loadmap")==0) {
		if (c->getCommand().compare("loadmap")==0) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (gameState.compare("validatemap")==0) {
		if (c->getCommand().compare("validatemap")==0) {
			return true;
		}
		else {
			return false;
		}

		
	}
	else if (gameState.compare("addplayer")==0) {
		if (c->getCommand().compare("addplayer")==0) {
			return true;
		}
		else {
			return false;
		}

		
	}
	else if (gameState.compare("assigncountries")==0) {
		if (c->getCommand().compare("assigncountries")==0) {
			return true;
		}
		else {
			return false;
		}

		
	}
	else if (gameState.compare("issueorder")==0) {
		if (c->getCommand().compare("issueorder")==0) {
			return true;
		}
		else {
			return false;
		}

	}
	else if (gameState.compare("endissueorders")==0) {
		if (c->getCommand().compare("endissueorders")==0) {
			return true;
		}
		else {
			return false;
		}
		
		
	}
	else if (gameState.compare("execorder")==0) {
		if (c->getCommand().compare("execorder")==0) {
			return true;
		}
		else {
			return false;
		}

	}
	else if (gameState.compare("endexecorders")==0) {
		if (c->getCommand().compare("endexecorders")==0) {
			return true;
		}
		else {
			return false;
		}

	}
	else if (gameState.compare("win")==0) {
		if (c->getCommand().compare("win")==0) {
			return true;
		}
		else {
			return false;
		}

	}
	else if (gameState.compare("play")==0) {
		if (c->getCommand().compare("play")==0) {
			return true;
		}
		else {
			return false;
		}

	}
	else if (gameState.compare("end")==0) {
		if (c->getCommand().compare("end")==0) {
			return true;
		}
		else {
			return false;
		}

	}
	return false;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(CommandProcessor* c) {
	comProcessor = c;
	listOfCommands;
	commandINMemmory = "";
	filepath = "";
	posInFile = 0;



}

FileCommandProcessorAdapter::FileCommandProcessorAdapter() {
	comProcessor = new CommandProcessor();
	listOfCommands;
	commandINMemmory = "";
	posInFile = 0;


}
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
	delete comProcessor;
	listOfCommands;
	commandINMemmory = "";
	posInFile = 0;


}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(Observer* observ) {
	comProcessor = new CommandProcessor();
	listOfCommands;
	commandINMemmory = "";
	posInFile = 0;

	this->Attach(observ);


}

void FileCommandProcessorAdapter::setFilePath(std::string path) {
	filepath = path;

}

std::string FileCommandProcessorAdapter::readCommand() {
	std::string input = "";


	std::ifstream inputFile(filepath);
	if (inputFile) {
		std::cout << "File existe\n";
		inputFile.seekg(posInFile);
		if (inputFile.peek() == EOF) {
			std::cout << "End of file reached ";
			inputFile.close();
			exit(0);
			return "end off file";

		}
		else {
			std::getline(inputFile, input);
			posInFile = inputFile.tellg();
			std::cout << "Read from file : " << input << std::endl;
			return input;

		}

	}
	else {
	std:cout << "File does not exist , exiting the program";
		exit(0);
	}
	
	

}

Command* FileCommandProcessorAdapter::getCommand() {
	 std::string command =FileCommandProcessorAdapter::readCommand();
	 return saveCommand(command);

}

Command* FileCommandProcessorAdapter::saveCommand(std::string fromReadCommand)
{
	Observer* obser = getObserver()->front();
	Command* aCommand = new Command(fromReadCommand, "", obser);

	std::list<Observer* >::iterator i = this->getObserver()->begin();
	for (; i != this->getObserver()->end(); ++i) {

	}
	listOfCommands.push_back(aCommand);
	commandINMemmory = fromReadCommand;
	cout << "The command : " << aCommand << " will now be saved into the list of commands " << endl;
	Notify(*this);
	return aCommand;


}


/*
Note to myself , link everything by constructor 
and to use the file addapter you have to manualy link the gooc command processor to the gameengine class.
*/
