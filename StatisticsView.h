//
// Created by Derek James on 2021-04-08.
//

#ifndef C345_ASS2_STATISTICSVIEW_H
#define C345_ASS2_STATISTICSVIEW_H

#include "Observer.h"
#include <iostream>
#include "Map.h"
#include "Player.h"
#include <vector>
#include <string>
#include "GameEngine.h"

#define SHOW_MEMORY_STATS false
#define SHOW_DEBUG_STATS false

using std::cout;
using std::endl;
using std::string;
using std::vector;


class StatisticsView : public Observer {
  public:
    StatisticsView()=default; //constrcutor
    StatisticsView(GameEngine* g); //parametrized constructor
    ~StatisticsView(); //desructor
    void update() override; //called by Subject
    void displayStats(); //creates stats view and displays it
  private:
    vector<string>* viewLines= nullptr; //simple list of lines that represents the stats view.
    GameEngine *_subject; //to be observed
};

class GameEndView : public Observer {
  public:
    GameEndView()=default;
    GameEndView(GameEngine* g);
    ~GameEndView();
    void update() override; //builds the view
  private:
    GameEngine *_subject;

};

void ass3Part1Demo();



#endif //C345_ASS2_STATISTICSVIEW_H
