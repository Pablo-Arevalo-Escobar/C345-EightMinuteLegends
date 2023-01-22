//
// Created by Derek James on 2021-04-09.
//

#include "Subject.h"
#include "Observer.h"

Subject::Subject() {
    _observers=new list<Observer*>;
}
Subject::~Subject() {
    delete _observers;
}
void Subject::attach(Observer *o) {
    _observers->push_back(o);
}
void Subject::detach(Observer *o) {
    _observers->remove(o);
}
void Subject::notify() {
    if (SHOW_DEBUG_SUBJECT) {
        cout << "DEBUG: Notify being called on " << _observers->size() << " observers..." << endl;
    }
    for (Observer *&o : *_observers) {
        o->update();
    }
}