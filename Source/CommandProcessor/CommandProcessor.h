#pragma once 
//#include "../Engine/GameEngine.h"
#include "../GameLog/LoggingObserver.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <istream>

#include <map>


class Command : public Subject, ILoggable {

public:
	//Command constructors 

	Command();
	Command(std::string newCommand, std::string newState);
	Command(const Command& newCommandObject);
	Command& operator =(const Command& newCommandObject);
	friend std::ostream& operator<<(std::ostream& os, const Command& newCommandObject);

	// getters and setters of command attributes, saveCommand() and saveState() act as setters  

	std::string getCommand() const;
	void saveCommand(std::string command);
	std::string getEffect() const;
	void saveEffect(std::string state);
	virtual std::string stringToLog();



private:
	//command attributes 
	std::string command;
	std::string effect;
};

class CommandProcessor :  virtual public Subject,  virtual public ILoggable{
public:
	CommandProcessor();
	~CommandProcessor();
	virtual std::string stringToLog() override; 
	//GameEngine* getGameEngine();
	virtual Command* getCommand();
	virtual bool validate(Command* c, std::string);
	



private:
	//GameEngine* gameEngine;

	std::vector<Command*> listOfCommands;
	std::string commandINMemmory;
	std::string readCommand();

	Command* saveCommand(std::string fromReadCommand);


};



class FileLineReader {

public:
	/*
		FileLineReader();

		FileLineReader(string path);

		FileLineReader(const FileLineReader& flr);

		~FileLineReader();

		std::string readLineFromFile();

		std::fstream input;

	private:
		std::string filePath;
		*/
};

class FileCommandProcessorAdapter :public CommandProcessor {
public:
	/*
	FileLineReader* FileProcessor;

	FileCommandProcessorAdapter(FileLineReader*);

	Command* readCommand();

	*/
};


