#pragma once

#include <iostream>
#include <cstring>
#include<list>
#include<vector>
#include <regex>
using std::cin;
using std::cout;


class Command {

public:



	Command();
	Command(std::string newCommand, std::string newState);
	Command(const Command& newCommandObject);
	Command& operator =(const Command& newCommandObject);
	friend std::ostream& operator<<(std::ostream& os, const Command& newCommandObject);

	// getCommand and getEffect is like the getters
	// saveEffect is like the setter 

	std::string getCommand() const;
	std::string getState() const;
	void saveState(std::string state);


private:

	std::string command;
	std::string state;

};

class CommandProcessor {

public: 
	CommandProcessor(); 
	~CommandProcessor();
	CommandProcessor(const CommandProcessor& newCP);
	CommandProcessor& operator= (const CommandProcessor& newCP);

	virtual  Command* getCommand();

	// just to see what is inside the vector 

	void showCommands(); 

	virtual std::string readCommand();

	Command* saveCommand(std::string fromReadCommand);

	void validate(std::string aCommand); 

	const string LOADMAP = "LOADMAP";
	const string VALIDATEMAP = "VALIDATEMAP";
	const string ADDPLAYER = "ADDPLAYER";
	const string GAMESTART = "GAMESTART";



private: 
	std::vector<Command*> listOfCommands;

	


};