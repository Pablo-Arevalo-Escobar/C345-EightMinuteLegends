//
// Created by Talha Multani on 2021-03-22.
//

#ifndef C345_ASS2_GAMELOADER_H
#define C345_ASS2_GAMELOADER_H
#include "GameEngine.h"
#include "MapLoader.h"
#include <string>
#include <iostream>
#include "Map.h"
#include "Player.h"
#include "CardsDriver.h"

using namespace std;

#define SHOW_MEMORY_GAMEELOADER true
#define SHOW_DEBUG_GAMEELOADER true

class GameEngine;

class GameLoader {

public:

    GameLoader(); //default
    ~GameLoader(); //destructor
    GameLoader(const GameLoader& gameLoader); //copy
    GameLoader& operator=(const GameLoader& gameLoader); //assignment
    friend ostream & operator<< (ostream &stream,  GameLoader &gameLoader); //stream output
   static void fillDeck(Deck* mainDeck, int n);  //fills deck with cards depending on number of players
    void gameStart(GameEngine *gameEngine); //initializes game
   static void welcomeBanner();

private:

};


#endif //C345_ASS2_GAMELOADER_H
