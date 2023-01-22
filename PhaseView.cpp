//
// Created by Pablo Arevalo on 16/04/2021.
//
#include "PhaseView.h"


PhaseView::PhaseView(GameEngine *s) {
    subject = s;
    subject->attach(this);
    for (int i=0;i<subject->getNumberOfPlayers();i++) {
        subject->players[i]->attach(this);
    }

}
PhaseView::~PhaseView()  {
    subject->detach(this);
    for (int i=0;i<subject->getNumberOfPlayers();i++) {
        subject->players[i]->detach(this);
    }
}

void PhaseView::update() {
    if(subject->players[subject->getCurrentPlayer()]->refreshPhaseVP){
        displayVP();
    }
    if(subject->refreshPhaseCard){
        displayCard();
    }
    if(subject->refreshPhase){
        displayTurn();
    }

}

void PhaseView::displayCard() {
    Player* player = subject->players[subject->getCurrentPlayer()];
    cout<< ANSI_YELLOW_ON_BLACK;
    if(subject->getCurrentCard() != nullptr){
        cout << player->getName() <<" has picked up " << *subject->getCurrentCard() << '\n';
    }
    cout<< ANSI_COLOR_RESET;
}

void PhaseView::displayVP(){
    cout<< ANSI_YELLOW_ON_BLACK;
    Player* player = subject->players[subject->getCurrentPlayer()];
    cout << player->getName() << " has gained VP\n" << player->getName() << " now has " << player->getVictoryPoints() << " VP" << endl;
    cout<< ANSI_COLOR_RESET;
}

void PhaseView::displayTurn() {
    Player* player = subject->players[subject->getCurrentPlayer()];
    cout<< ANSI_YELLOW_ON_BLACK;

    //Printing bot type if player is bot
    if(player->getPlayerStrategy()->isBot()){
        string strategy;
        if(player->getPlayerStrategy()->botType == 2){
             strategy = "GREEDY";
        }
        else if(player->getPlayerStrategy()->botType == 3){
             strategy = "MODERATE";
        }
        cout << strategy << " Computer:" << endl;
    }

    //Printing player name, card, and VP
    cout <<  "Player " << subject->getCurrentPlayer()<< " : " << player->getName() << " turn: " <<  ANSI_COLOR_RESET << endl;
}

void ass3Part2Demo(){
    cout << "Welcome to the Part2 Demo" << endl;
    cout << "We feel the normal game is sufficient for the Demo, but just in case we created a mini-driver that shows the part2 observers." << endl;
    cout << "It creates 2 bots and lets them play to completion" << endl;
    cout << "The yellow on black writing is the phase observer" << endl;
    cout << "Press Enter to start" << endl;
    cin.get();
    mainGame=new GameEngine();
    mainGame->createTestObjects();
    PhaseView *phaseView = new PhaseView(mainGame);
    while (!mainGame->isGameOver()) {
        mainGame->mainMap->showMapASCIIonScreen();
        mainGame->takeTurn();
    }
    cout << "End of Part 2 Demo" << endl;
    delete phaseView;
    delete mainGame;
    phaseView= nullptr;
    mainGame= nullptr;
}