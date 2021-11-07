/**
 * Class header for the CommandProcessor, FileCommandProcessorAdapter,
 * and Command classes.
 *
 * @author Mark Nasol
 */

#pragma once

#include <string>
#include <ostream>
#include <list>
using namespace std;

class Command {
public:
	Command();												//Constructor
	~Command();												//Destructor
	Command(const Command& com);							//Copy constructor
	Command& operator=(const Command& com);					//Assignment operator
	friend ostream& operator<<(ostream& out, Command& com);	//Stream insertion operator

	void saveEffect();

private:
	string command;
	string effect;
};

class CommandProcessor {
public:
	CommandProcessor();													//Constructor
	~CommandProcessor();												//Destructor
	CommandProcessor(const CommandProcessor& cp);						//Copy constructor
	CommandProcessor& operator=(const CommandProcessor& cp);			//Assignment operator
	friend ostream& operator<<(ostream& out, CommandProcessor& cp);	//Stream insertion operator

	virtual void readCommand();
	void getCommand();
	bool validate();

private:
	list<Command*> lc;
};

class FileCommandProcessorAdapter {
public:
	FileCommandProcessorAdapter();													//Constructor
	~FileCommandProcessorAdapter();													//Destructor
	FileCommandProcessorAdapter(const FileCommandProcessorAdapter& fcpa);			//Copy constructor
	FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& fcpa);//Assignment operator
	friend ostream& operator<<(ostream& out, FileCommandProcessorAdapter& fcpa);	//Stream insertion operator

	void readCommand();

private:
	FileLineReader* flr;
};

class FileLineReader {
public:
	FileLineReader();												//Constructor
	~FileLineReader();												//Destructor
	FileLineReader(const FileLineReader& flr);						//Copy constructor
	FileLineReader& operator=(const FileLineReader& flr);			//Assignment operator
	friend ostream& operator<<(ostream& out, FileLineReader& flr);	//Stream insertion operator

	void readLineFromFile();
};
