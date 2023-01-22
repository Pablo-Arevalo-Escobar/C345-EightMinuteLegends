//
// Created by Kevin Li Zhong Hao on 2021-04-12.
//
#include "PlayerStrategies.h"
#include <iostream>
#include <string>
#include "GameEngine.h"
#include "Cards.h"
#include "Player.h"

using namespace std;

extern GameEngine *mainGame;

//concrete human strategy
void HumanPlayerStrategy::execute(){
    Card* boughtCard{};
    Player *currentPlayer = mainGame->players[mainGame->getCurrentPlayer()];
    boughtCard = mainGame->buyCardFromHand(currentPlayer); //entire routine to buy card from hand.
    mainGame->setCurrentCard(boughtCard); //triggers observer
    currentPlayer->ComputeScore(true); //new in ass 3:  Compute score every round to update victory points which might trigger Statistics view.
    mainGame->processPlayerActions(boughtCard, currentPlayer);  //Now the player processes the card for any actions.
}

//concrete greedy bot strategy
void GreedyComputerStrategy::execute(){
    processBotMove("Greedy", "build", "remove"); //greedy bot focuses on building cities or destroying opponents

}

//concrete moderate bot strategy
void ModerateComputerStrategy::execute(){
    processBotMove("Moderate", "move", "get"); //moderate bot focuses on controlling region (i.e. move and occupy)
}

//helper function to process bot move depending on type of bot (i.e. greedy vs. moderate)
void PlayerStrategy::processBotMove(string typeOfComputer, string targetAbility1, string targetAbility2) {

    Player *currentPlayer = mainGame->players[mainGame->getCurrentPlayer()];
    Card *currentCard{};

    //loop over hand
    for (int i = 0; i < 6; i++){

        //check card by card if it includes description of preferred action by type of bot
        currentCard = mainGame->hand->getCard(i);
        if (((currentCard->getCardAbilityDescription()).find
                (targetAbility1) != string::npos || (currentCard->getCardAbilityDescription()).find(targetAbility2) != string::npos) && mainGame->botCardCost(i) <= currentPlayer->getCoins()){

            //give card to bot, perform exchange with hand
            currentCard = mainGame->hand->exchange(i);
            mainGame->setCurrentCard(currentCard); //triggers observer

            //add card to player stash
            currentPlayer->AddCardToOwned(currentCard); //creates handle for Observer

            //deduce cost of card from owned coins
            currentPlayer->setCoins(currentPlayer->getCoins()-mainGame->botCardCost(i));
            //output card picked by bot

            break;
        }

        //if none of the card is favored by greedy bot, or if card cost exceed owned coins, pick card 0
        else if (i == 5 || mainGame->botCardCost(i) > currentPlayer->getCoins()){
            currentCard = mainGame->hand->exchange(0);
            mainGame->setCurrentCard(currentCard); //triggers observer
            currentPlayer->AddCardToOwned(currentCard); //creates handle for Observer
            break;
        }

    }

    //extract card ability info into string format
    string ability1S = enum_to_string(currentCard->getAbility1());
    string ability2S = enum_to_string(currentCard->getOptionalAbility());

    //check if the cards offer goods (stackable abilities/top of card effect). If so, check if there is a second one.
    if (!enum_to_string(currentCard->getGood()).empty()) {
        mainGame->processCardGood(currentCard->getGood(), currentPlayer);
        if (!enum_to_string(currentCard->getGood2()).empty())
            mainGame->processCardGood(currentCard->getGood2(), currentPlayer);
    }

    //auto perform OR ability preferred by bot type. If none, default uses LHS ability
    if (currentCard->getAbilityOr()){
        if (enum_to_string(currentCard->getOptionalAbility()).find
                (targetAbility1) != string::npos || enum_to_string(currentCard->getOptionalAbility()).find(targetAbility2) != string::npos){
            cout << "Performing " << enum_to_string(currentCard->getOptionalAbility()) << "." << endl;
            mainGame->processBotPossibleAction(currentCard->getOptionalAbility(), currentPlayer);
        } else{
            cout << "Performing " << enum_to_string(currentCard->getAbility1()) << "." << endl;
            mainGame->processBotPossibleAction(currentCard->getAbility1(), currentPlayer);
        }
    }

    //auto perform AND abilities
    else if (currentCard->getAbilityAnd()){
        cout << "Performing both actions: " << ability1S << " AND " << ability2S << endl;
        cout << "First: " << ability1S << endl;
        mainGame->processBotPossibleAction(currentCard->getAbility1(), currentPlayer);
        cout << "Now: " << ability2S << endl;
        mainGame->processBotPossibleAction(currentCard->getOptionalAbility(), currentPlayer);
    }
    else { //auto perform ability if only one given by the picked card
        cout << "Perform the action: " << ability1S << endl;
        mainGame->processBotPossibleAction(currentCard->getAbility1(), currentPlayer);
    }

    currentPlayer->ComputeScore(true); //triggers observer
}
//Demo.  Better to just run game
#include "PhaseView.h"

void ass3Part1Demo() {
    cout << "Welcome to the Part1 Demo" << endl;
    cout << "We feel the normal game is sufficient for the Demo, but just in case we created a mkini-driver that shows the part1 Strategies." << endl;
    cout << "It creates 2 bots and lets them play to completion" << endl;
    cout << "We will do 3 rounds, first 2 bots, then change 1 bot to a human, then 2 humans" << endl;
    cout << "Press Enter to start" << endl;
    cin.get();
    int round=0;
    mainGame=new GameEngine();
    mainGame->createTestObjects(); // creates 2 bots by default
    PhaseView *phaseView = new PhaseView(mainGame); //need phase view to help distinguish who is playing
    while (round<3) {
        cout << "DEMO::::  ROUND " << round << endl;
        mainGame->mainMap->showMapASCIIonScreen();
        mainGame->takeTurn();
        round++;
        if (round==1) {
            mainGame->players[0]->setPlayerStrategy(new HumanPlayerStrategy());
        }
        if (round==2) {
            mainGame->players[1]->setPlayerStrategy(new HumanPlayerStrategy());
        }
    }
    cout << "End of Part 1 Demo" << endl;
    delete phaseView;
    phaseView=nullptr;
    delete mainGame;
    mainGame=nullptr;

}