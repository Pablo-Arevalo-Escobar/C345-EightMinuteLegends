//
// Created by Derek James on 2021-04-09.
//

#ifndef C345_ASS2_SUBJECT_H
#define C345_ASS2_SUBJECT_H

#include <iostream>
#include <list>

#define SHOW_MEMORY_SUBJECT false
#define SHOW_DEBUG_SUBJECT false

using namespace std;

class Observer;

class Subject {
  public:
    Subject();
    virtual ~Subject();
    void attach(Observer* o);
    void detach(Observer* o);
    void notify();
  private:
    list<Observer*> *_observers;
};


#endif //C345_ASS2_SUBJECT_H
