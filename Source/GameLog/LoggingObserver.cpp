#include "LoggingObserver.h"
#include <iostream>
#include <fstream>
#include <list>

Observer::Observer() {

}
Observer::~Observer() {
	std::fstream outfile;
	outfile.open("log.txt", std::ios::trunc);
	outfile.close();

}

Subject::Subject() {
	_observers = new std::list<Observer*>;

}

Subject::~Subject() {
	delete _observers;
}

void Subject::Attach(Observer* o) {
	_observers->push_back(o);
	
}

void Subject::Detach(Observer* o) {
	_observers->remove(o);
}

void Subject::Notify(ILoggable& iLoggable) {
	std::list<Observer* >::iterator i = _observers->begin();
	for (; i != _observers->end(); ++i) {
		(*i)->Update(iLoggable);
	}
}

LogObserver::LogObserver() {
};

LogObserver::LogObserver(Subject * s) {
	_subject = s;
	_subject->Attach(this);

};

LogObserver::LogObserver(std::list<Subject*>* s) {
	std::list<Subject*>::iterator i =s->begin();
	for (; i != s->end(); i++) {
		(*i)->Attach(this);
	}
	//_subject->Attach(this);

};
void LogObserver::Update(ILoggable& iLoggable) {
	std::fstream outfile;
	outfile.open("log.txt", std::ios::app);
	outfile << iLoggable.stringToLog()<<std::endl;
	outfile.close();
}
dummy::dummy() {

	this->name = "paule";
}

void dummy::changeName(std::string text) {
	this->name = text;
	this->Notify(*this);
}

std::string dummy::stringToLog() {
	return this->name;
}