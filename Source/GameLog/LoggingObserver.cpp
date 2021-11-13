#include "LoggingObserver.h"
#include <iostream>
#include <fstream>
#include <list>

Observer::Observer() {

}
Observer::~Observer() {

}

Subject::Subject() {
	_observers = new std::list<Observer*>;

}

Subject::Subject(const Subject& s) {
	_observers = s._observers;

}

std::list<Observer*>* Subject::getObserver() {
	return _observers;
};

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
	_subject = NULL;
	_subjectList = NULL;
};

LogObserver::LogObserver(const LogObserver& l) {
	_subject = l._subject;
	_subjectList = l._subjectList;

};

LogObserver::LogObserver(Subject * s) {
	_subject = s;
	_subject->Attach(this);
	_subjectList = NULL;


};

LogObserver::LogObserver(std::list<Subject*>* s) {
	std::list<Subject*>::iterator i =s->begin();
	_subjectList = s;
	for (; i != s->end(); i++) {					//Loop trought the list of subject and attach them.
		(*i)->Attach(this);
	}
	

};

LogObserver::~LogObserver() {
	delete _subject;
	delete _subjectList;
}

void LogObserver::Update(ILoggable& iLoggable) {
	std::fstream outfile;
	outfile.open("Log.txt", std::ios::app);
	outfile << iLoggable.stringToLog()<<std::endl;
	outfile.close();
}

void LogObserver::lateAddition(Subject* s) {
	_subjectList->push_back(s);
	s->Attach(this);
}
