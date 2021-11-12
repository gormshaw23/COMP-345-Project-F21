#pragma once 

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include "../Engine/GameEngine.h"
#include <map>

class Command {

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



	private:
		//command attributes 
		std::string command; 
		std::string effect; 
};

class CommandProcessor {

public: 

	CommandProcessor(); 
	~CommandProcessor(); 
	
	GameEngine* getGameEngine(); 
	virtual  Command* getCommand();
	virtual bool validate(Command* c);


private: 
	GameEngine* gameEngine; 
	

	std::vector<Command*> listOfCommands; 

	std::string readCommand(); 

	Command* saveCommand(std::string fromReadCommand);


};

class FileLineReader {

public:

	FileLineReader();

	FileLineReader(string path);

	FileLineReader(const FileLineReader& flr);

	~FileLineReader();

	string readLineFromFile();

	std::fstream input;

private:
	string filePath;

};

class FileCommandProcessorAdapter :public CommandProcessor {
public:
	FileLineReader* FileProcessor;

	FileCommandProcessorAdapter(FileLineReader*);

	Command* readCommand();
};


