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
	Command(Observer*);
	Command(std::string newCommand, std::string newState, Observer*);
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
	CommandProcessor(Observer* );
	~CommandProcessor();
	virtual std::string stringToLog() override; 
	//GameEngine* getGameEngine();
	virtual Command* getCommand();
	virtual bool validate(Command* c, std::string);

private:


	std::vector<Command*> listOfCommands;
	std::string commandINMemmory;
	virtual std::string readCommand();

	virtual Command* saveCommand(std::string fromReadCommand);


};




class FileCommandProcessorAdapter :virtual public Subject, virtual public ILoggable, public CommandProcessor {
private:
	CommandProcessor* comProcessor;
	std::string filepath;
	std::vector<Command*> listOfCommands;
	std::string commandINMemmory;
	int posInFile;

	virtual Command* saveCommand(std::string fromReadCommand);
	


public:
	FileCommandProcessorAdapter();
	FileCommandProcessorAdapter(Observer*);
	~FileCommandProcessorAdapter();
	FileCommandProcessorAdapter(CommandProcessor*);
	void setFilePath(std::string);

	virtual std::string readCommand();

	virtual Command* getCommand();
	

	
};


