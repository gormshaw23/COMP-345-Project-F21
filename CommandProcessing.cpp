/**
 * Compilation unit for the CommandProcessor, FileCommandProcessorAdapter,
 * and Command classes.
 *
 * @author Mark Nasol
 */

#include <iostream>
#include <iterator>
#include <list>
#include <string>
using namespace std;
#include "CommandProcessing.h"

/********************************************************************
 * Command class function definitions
 ********************************************************************/

/**
* Constructor of the Command class
*/
Command::Command()
{
	command = "";
	effect = ";";
}

/**
 * Destructor of the Command class
 */
Command::~Command()
{
}

/**
 * Copy constructor of the Command class
 */
Command::Command(const Command& com)
{
	this->command = com.command;
	this->effect = com.effect;
}

/**
 * Assignment operator of the Command class
 */
Command& Command::operator=(const Command& com)
{
	this->command = com.command;
	this->effect = com.effect;
	return *this;
}

/**
 * Stream insertion operator of the Command class
 */
ostream& operator<<(ostream& out, Command& com)
{
	out << "Command: " << com.getCommand() << "\Effect: " << com.getEffect();
	return out;
}

/**
* Getter for the Command's command name
* 
* @return the Command's command name
*/
const string Command::getCommand() {
	return command;
}

/**
* Getter for the Command's effect
*
* @return the Command's effect
*/
const string Command::getEffect() {
	return effect;
}

/**
* Saves the Command's new effect
*
* @param the Command's new effect
*/
void Command::saveEffect(string newEffect) {
	effect = newEffect;
}


/********************************************************************
 * CommandProcessor class function definitions
 ********************************************************************/

 /********************************************************************
  * FileCommandProcessorAdapter class function definitions
  ********************************************************************/

  /********************************************************************
   * FileLineReader class function definitions
   ********************************************************************/