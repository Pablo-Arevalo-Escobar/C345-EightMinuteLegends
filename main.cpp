/**********************************************************
 * Comp 345 Winter 2021 Assignment PART 3
 *
 * Derek James 22688964
 * Talha Multani 40034526
 * Kevin Zhong Hao Li 40094223
 * Jay Chen 40053162
 * Pablo Arevalo Escobar 40081955
 *
 * Team 20
 **********************************************************/
#include <iostream>
#include "PhaseView.h"
#include "GameEngine.h"
#include "GameStartup.h"
#include "CardsDriver.h"
#include "GameEnd.h"
#include "GameLoader.h"
#include "GameLoaderDriver.h"
#include "GameStartup.h"
#include "StatisticsView.h"


//Turn on DEMO_MODE for demonstration with marker.
#define DEMO_MODE false
//Turn on USE_TEST_DATA to bypass loader and gamestartup phases (parts 1 and 2) and use testData.
#define USE_TEST_DATA false


using std::cout;
using std::cin;
using std::endl;

GameEngine *mainGame; //MAIN GAME

int main() {
    //demo drivers
    if (DEMO_MODE) {
        ass3Part1Demo();
        //ass3Part2Demo();
        //ass3Part3Demo();
        return 0;
    }

    //Actual GAME
    if (!USE_TEST_DATA) {
        //official way to play game
        mainGame = new GameEngine();
        GameLoader *loader = new GameLoader();
        mainGame->tournamentStart(); //asks user if they want tournament mode.
        if (mainGame->isTournament()==false) {
            loader->gameStart(mainGame); //go through normal human player menus
        }
        delete loader;
        loader= nullptr;

        GameStartup *starter = new GameStartup();
        starter->startupPhase(mainGame);
        delete starter;
        starter=nullptr;
    } else {
        //load test data for GameEngine instead of loader routimes/
        mainGame = new GameEngine();  //for testing.  should be created by loader
        mainGame->createTestObjects(); // only for testing.  will be replaced by other parts
    }

    //before game starts, ask if dynamic strategy change should be allowed in game
    mainGame->setAllowStrategyDynamicChange();
    //create observers
    GameEndView *viewGameEnd = new GameEndView(mainGame);
    StatisticsView *viewStats = new StatisticsView(mainGame);
    PhaseView* phase =  new PhaseView(mainGame);




    //Main Game Loop using whichever method used to populate GameEngine
    while (!mainGame->isGameOver()) {
        mainGame->mainMap->showMapASCIIonScreen();
        mainGame->takeTurn(); //part 3/4/5 combined
    }
    //Game ended.  Show map one last time
    mainGame->showMapAndHandAndPlayers();
    //Show Tournament results
    if (mainGame->isTournament()) {
        mainGame->showTournamentResults();
    }

    GameEnd::endGame(*mainGame); //use static method of class

    //cleanup
    delete phase;
    phase= nullptr;
    delete viewStats;
    viewStats= nullptr;
    delete viewGameEnd;
    viewGameEnd= nullptr;
    delete mainGame;
    mainGame= nullptr;

}


