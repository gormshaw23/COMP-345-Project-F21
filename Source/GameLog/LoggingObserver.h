#pragma once
#include<iostream>
#include <list>

class ILoggable {
public:
	virtual std::string stringToLog() = 0;
};

class Observer {
public:
	~Observer();
	virtual void Update(ILoggable &) = 0;
protected:
	Observer();

};

class Subject {
public:
	virtual void Attach(Observer* o);
	virtual void Detach(Observer* o);
	virtual void Notify(ILoggable &);
	Subject();
	Subject(const Subject&);
	~Subject();
private:
	std::list<Observer*>* _observers;

};


class LogObserver: public Observer {
public: 
	void Update(ILoggable &);
	LogObserver();
	~LogObserver();
	LogObserver(Subject*);
	LogObserver(std::list<Subject*>*);
	LogObserver(const LogObserver&);
	void lateAddition(Subject*);
	Subject* _subject;
	std::list<Subject*>* _subjectList;
};

class dummy : public Subject, public ILoggable {
public:
	std::string name;
	virtual std::string stringToLog();
	void changeName(std::string );
	dummy();
	~dummy();
};
