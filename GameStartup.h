//
// Created by Jay on 2021-03-20.
//

#ifndef C345_ASS2_GAMESTARTUP_H
#define C345_ASS2_GAMESTARTUP_H

#include "GameEngine.h"

class GameStartup {
public:
    int totalArmies; // number of armies placed on the board
    GameStartup(); // default constructor
    ~GameStartup(); // destructor
    void startupPhase(GameEngine* game); // startup phase
};

void part2Demo(); // test driver

#endif //C345_ASS2_GAMESTARTUP_H
