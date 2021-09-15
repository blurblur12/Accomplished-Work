#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include "info.h"

class Observer;

class Subject {
private:
	// the set of observers of the subject
	std::vector<Observer *> observers;
public:
	void attach(Observer * o);     // attaches o to the subject
	void detach(Observer * o);     // detaches o to the subject
	void notifyObservers();        // notify all observers
	virtual ~Subject() = 0;        // purely virtual destructor
	virtual Info getInfo() const = 0;
};

#endif
