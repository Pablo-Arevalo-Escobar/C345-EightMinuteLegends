//
// Created by Pablo Arevalo on 23/03/2021.
//

#include "GameEnd.h"
#include "GameLoader.h"
#include "GameEngine.h"
#include "GameStartup.h"
#include "CardsDriver.h"
#include "GameEnd.h"

extern GameEngine *mainGame;

//endGame routine once GameEngine has identified the end/
void GameEnd::endGame(GameEngine& engine) {
    //Note:  Code for 3 & 4 players not done on purpose
    if(engine.getNumberOfPlayers() == 2){
        int playerScore[2] = {engine.players[0]->ComputeScore(), engine.players[1]->ComputeScore()};
        if(playerScore[0] != playerScore[1]){
            if(playerScore[0] > playerScore[1]){
                cout << "\nPlayer 0 score: " << playerScore[0] << endl;
                cout << "Player 1 score: " << playerScore[1] << endl;
                cout << "The winner is player 0: " << engine.players[0]->getName() << endl;
            }
            else{
                cout << "\nPlayer 0 score: " << playerScore[0] << endl;
                cout << "Player 1 score: " << playerScore[1] << endl;
                cout << "The winner is player 1:" << engine.players[1]->getName() << endl;
            }
        }

        //Draw/Tie handling level 1
        else{
            cout << "\nSCORE IS A TIE, CHECKING NUMBER OF COINS" << endl;
            if(engine.players[0]->getCoins() > engine.players[1]->getCoins()){
                cout << "Player 0 #Coins: " << engine.players[0]->getCoins() << endl;
                cout << "Player 1 #Coins: " << engine.players[1]->getCoins() << endl;
                cout << "The winner is player 0:" << engine.players[0]->getName() << endl;
            }
            if(engine.players[0]->getCoins() < engine.players[1]->getCoins()){
                cout << "Player 0 #Coins: " << engine.players[0]->getCoins() << endl;
                cout << "Player 1 #Coins: " << engine.players[1]->getCoins() << endl;
                cout << "The winner is player 1:" << engine.players[1]->getName() << endl;
            }

            //Draw/Tie handling level 2
            else {
                cout << "\nNUMBER OF COINS IS A TIE, CHECKING NUMBER OF ARMIES" << endl;
                int numOfArmies1 = 0;
                int numOfArmies2 = 0;

                //Summing the number of armies in each territory for player 1
                for(Territory* t: *engine.mainMap->getTerritoriesOfPlayer(engine.players[0]->getId())){
                    numOfArmies1 += engine.mainMap->getArmies(engine.players[0]->getId(),t->getID());
                }

                //Summing the number of armies in each territory for player 2
                for(Territory* t: *engine.mainMap->getTerritoriesOfPlayer(engine.players[1]->getId())){
                    numOfArmies2 += engine.mainMap->getArmies(engine.players[1]->getId(),t->getID());
                }

                if(numOfArmies1 > numOfArmies2){
                    cout << "Player 0 #Armies: " << numOfArmies1 << endl;
                    cout << "Player 1 #Armies: " << numOfArmies2 << endl;
                    cout << "The winner is player 0:" << engine.players[0]->getName() << endl;
                }
                else if(numOfArmies1 < numOfArmies2){
                    cout << "Player 0 #Armies: " << numOfArmies1 << endl;
                    cout << "Player 1 #Armies: " << numOfArmies2 << endl;
                    cout << "The winner is player 1:" << engine.players[1]->getName() << endl;
                }

                //Draw/Tie handling level 3
                else{
                    cout << "\nNUMBER OF ARMIES IS A TIE, CHECKING NUMBER OF TERRITORIES" << endl;
                    int numOfTerritories1 = engine.mainMap->getTerritoriesOfPlayer(engine.players[0]->getId())->size();
                    int numOfTerritories2 = engine.mainMap->getTerritoriesOfPlayer(engine.players[1]->getId())->size();

                    if(numOfTerritories1 > numOfTerritories2){
                        cout << "Player 0 #Territories: " << numOfTerritories1 << endl;
                        cout << "Player 1 #Territories: " << numOfTerritories2 << endl;
                        cout << "The winner is player 0:" << engine.players[0]->getName() << endl;
                    }
                    else if(numOfTerritories1 < numOfTerritories2){
                        cout << "Player 0 #Territories: " << numOfTerritories1 << endl;
                        cout << "Player 1 #Territories: " << numOfTerritories2 << endl;
                        cout << "The winner is player 1:" << engine.players[1]->getName() << endl;
                    }

                    else{
                        cout << "All options exhausted: DRAW" << endl;
                    }


                }

            }


        }
    }
}

//for demo
void part6Demo(){
    mainGame = new GameEngine();  //for testing.  should be created by loader
    mainGame->createTestObjects(); // only for testing.  will be replaced by other parts

    int rounds=1;
    while (rounds>0) {
        rounds--;
        mainGame->mainMap->showMapASCIIonScreen();
        mainGame->takeTurn(); //part 3
    }
    mainGame->showMapAndHandAndPlayers(); //show one more time

    GameEnd::endGame(*mainGame); //call static method directly without instantiating an object.

    delete mainGame;
    mainGame=nullptr;
}