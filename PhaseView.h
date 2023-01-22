//
// Created by Pablo Arevalo on 16/04/2021.
//

#pragma once
#include "Observer.h"
#include "GameEngine.h"

extern GameEngine *mainGame;

class PhaseView: public Observer {
public:
    PhaseView(GameEngine * s);
    ~PhaseView();
    void update(); //builds the view
    void displayTurn(); //dumps the view to the board (without rebuilding it)
    void displayCard();
    void displayVP();
private:
    GameEngine *subject;
};

void ass3Part2Demo();

