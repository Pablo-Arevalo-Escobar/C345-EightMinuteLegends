//
// Created by Talha Multani on 2021-03-25.
//

#include "GameLoaderDriver.h"


void part1Demo() {

//    GameLoader* loader=new GameLoader();
//    loader->gameStart(mainGame);
//    delete loader;


    //OR
    GameEngine* mainGame=new GameEngine();
    GameLoader* loader=new GameLoader();
    loader->gameStart(mainGame);
    for (int i=0;i<4;i++) {
        if (mainGame->players[i]!= nullptr) {
            cout << *mainGame->players[i] << " has bidding facility " << *mainGame->players[i]->getBiddingFacility()<< endl;
        } else {
            cout << "Player-" << i << " not created." << endl;
        }
    }

    delete loader;
    loader=nullptr;
    delete mainGame;
    mainGame=nullptr;
}