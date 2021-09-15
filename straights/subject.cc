#include "subject.h"
#include "observer.h"
#include <vector>

// attach o to the subject
void Subject::attach(Observer * o) {
	observers.emplace_back(o);
}

// notify all observers of a change of state
void Subject::notifyObservers() {
	for (auto & ob : observers) {
		ob->notify(*this);
	}
}

Subject::~Subject() {}
