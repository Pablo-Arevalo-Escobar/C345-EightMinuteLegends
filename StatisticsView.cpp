//
// Created by Derek James on 2021-04-08.
//

#include "StatisticsView.h"
#include "GameEngine.h"

extern GameEngine *mainGame;

//method called by Subject
void StatisticsView::update() {
    displayStats();
}


//Dumps the previously created viewLines to the screen.  Does NOT recalculate.
void StatisticsView::displayStats() {
    static int counter{0};
    counter++;
    //version 2 after clarification with Nora
    //Make sure we actually need to display a new stats (this might have been called for a different reason altogether)
    bool needRefresh{false};
    if (_subject->refreshStats==true || _subject->mainMap->refreshStats==true) {
        needRefresh=true;
    }
    for (int i=0;i<_subject->getNumberOfPlayers();i++) {
        if (_subject->players[i]->refreshStats==true) {
            needRefresh=true;
        }
    }
    if (needRefresh==false) {
        return;
    }
    delete viewLines;

    //Convinced we need a new stats view.  Proceed.

    viewLines=new vector<string>;

    string s; //temp vars
    stringstream str;

    const list<Territory*>* territories=mainGame->mainMap->getTerritories(); //does not need to be deleted
    const int numTer=territories->size();
    int tcount{0}; //territory count
    int ccount{0}; //continent count
    int cities{0}; //count of cities
    int armies{0}; //count total armies for this player
    int vp{0};
    const int PLAYER_NAME_WIDTH=20;


    //build "graph" as Nora suggested in class
    for (int p=0;p<mainGame->getNumberOfPlayers();p++) {
        tcount=mainGame->mainMap->howManyTerritoriesDoesPlayerControl(p);
        ccount=mainGame->mainMap->howManyContinentsDoesPlayerControl(p);
        cities=mainGame->mainMap->howManyCitiesDoesPlayerHave(p);
        armies=mainGame->mainMap->howManyArmiesDoesPlayerHaveOnMap(p);
        str.str("");
        s = mainGame->players[p]->getPlayerShortString().substr(0,PLAYER_NAME_WIDTH);
        s.append(PLAYER_NAME_WIDTH-s.length(),' ');
        s =  (SHOW_DEBUG_STATS?"[" + to_string(counter) + "]":"") + s;
        str << s << ": ";
        str<< "Territories [" + string(tcount,'*') + string(numTer-tcount,'.') + "] ";
        str<< "Continents [" + string(ccount,'*') + string(mainGame->mainMap->getNumberOfContinents()-ccount,'.') + "] ";
        str<< left << setw(10) << "Cities:" + to_string(cities) + " " ;
        str<< setw(12) << "Armies:" + to_string(armies) + " " ;
        str<<setw(10) << "Coins:" + to_string(mainGame->players[p]->getCoins()) + " ";
        str<<setw(10) << "VP:"+ to_string(mainGame->players[p]->getVictoryPoints()) + " ";

        viewLines->push_back(str.str());
    }
    //display the lines just built
    for (string & s : *viewLines) {
        cout << ANSI_STATS_DEFAULT;
        cout << s << endl;
        cout << ANSI_COLOR_RESET;
    }
    //clear viewlines (not needed anymore)
    viewLines->clear();

}

StatisticsView::StatisticsView(GameEngine *g) {
    _subject=g;
    _subject->attach(this);
    for (int i=0;i<_subject->getNumberOfPlayers();i++) {
        _subject->players[i]->attach(this);
    }
}

StatisticsView::~StatisticsView() {
    //detach in reverse order
    for (int i=0;i<_subject->getNumberOfPlayers();i++) {
        _subject->players[i]->detach(this);
    }
    _subject->detach(this);
    _subject= nullptr;
    viewLines->clear();
    viewLines= nullptr;
}


void GameEndView::update() {
    if (_subject->refreshEndGame) {
        cout << ANSI_STATS_LIGHT << "* * * * * * * * * * * * * * * * * * * * " << ANSI_COLOR_RESET << endl;
        cout << ANSI_STATS_LIGHT << "        G A M E     O V E R             " << ANSI_COLOR_RESET << endl;
        cout << ANSI_STATS_LIGHT << "* * * * * * * * * * * * * * * * * * * * " << ANSI_COLOR_RESET << endl;
    }
}

GameEndView::GameEndView(GameEngine *g) {
    _subject=g;
    _subject->attach(this);
}

GameEndView::~GameEndView() {
    _subject->detach(this);
    _subject=nullptr;
}

//demo.  Better to run game.
void ass3Part3Demo() {
    cout << "Welcome to the Part3 Demo" << endl;
    cout << "We feel the normal game is sufficient for the Demo, but just in case we created a mini-driver that shows the part3 observers." << endl;
    cout << "It creates 2 bots and lets them play to completion" << endl;
    cout << "The solid blue lines are the Game Statistics Observers" << endl;
    cout << "Press Enter to start" << endl;
    cin.get();
    mainGame=new GameEngine();
    mainGame->createTestObjects();
    GameEndView *viewGameEnd = new GameEndView(mainGame);
    StatisticsView *viewStats = new StatisticsView(mainGame);
    while (!mainGame->isGameOver()) {
        mainGame->mainMap->showMapASCIIonScreen();
        mainGame->takeTurn(); //part 3/4/5 combined
    }
    cout << "End of Part 3 Demo" << endl;
    delete viewGameEnd;
    delete viewStats;
    delete mainGame;
    viewGameEnd=nullptr;
    viewStats= nullptr;
    mainGame= nullptr;

}



