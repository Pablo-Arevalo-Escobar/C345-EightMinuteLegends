//
// Created by Jay on 2021-03-20.
//

#include "GameStartup.h"
#include "GameLoader.h"

extern GameEngine *mainGame;

// default constructor
GameStartup::GameStartup() {
    this->totalArmies = 0;
}

// destructor
GameStartup::~GameStartup() {
    totalArmies = 0;
}
//Called as Part2, after the Map has been loaded.  Must send same game object that GameLoader modified.
void GameStartup::startupPhase(GameEngine *game) {
    vector<BiddingFacility*> bids{}; // player bids.  Created on the stack, will get destroyed as scope exits.
    // card labelled 'Card Cost' is set aside
    cout << "The card 'Card Cost' has been set aside." << endl;

    // the '4' and '3' cards are removed
    cout << "The card '4' has been removed." << endl;
    cout << "The card '3' has been removed." << endl;

    // shuffle deck
    game->mainDeck->shuffle();

    // cards are drawn into the hand from GameLoader when hand was created after deck created
    cout << "Six cards are drawn." << endl;

    // place the 'Card Cost' above the row of cards
    cout << "The card 'Card Cost' has been set." << endl;

    // each player places their armies
    for (auto player : game->players) {
        if (player == nullptr) continue; // no player here

        cout << "It is currently " << player->getName() << "'s turn (Player ID: " << player->getId() << ")." << endl;
        game->mainMap->setArmies(player->getId(), game->mainMap->getStartZone(), 4);
        player->setCoins(14); // give each player 14 coins
    }

    // put down the 10 NPC armies in 2 player game
    while (totalArmies < 10) {
        for (auto player : game->players) {
            if (player == nullptr) continue; // no player here
            if (player->getPlayerStrategy()->isBot()) {
                //randomize placement
                cout << "Bot is placing an NPC randomly..." << endl;
                int guess=mainGame->mainMap->getRandomValidTerritoryID();
                mainGame->mainMap->setNPCArmies(guess,mainGame->mainMap->getNPCArmies(guess)+1);
                totalArmies++;
                continue;
            }
            // player places an army for the NPC
            int territoryId = -1; // user input
            game->mainMap->showMapASCIIonScreen();
            cout << "It is currently " << player->getPlayerShortString() << "'s turn." << endl;
            while (!(game->mainMap->doesTerritoryExist(territoryId))) {
                if (territoryId != -1) cout << "Invalid territory ID." << endl;
                bool validChoice = false;
                while (!validChoice) {
                    cout << "Please enter the territory ID to place one army for the NPC: ";
                    cin >> territoryId;
                    while (cin.fail()) {
                        cout << "Invalid territory ID. Try again: ";
                        cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cin>>territoryId;
                    }
                    validChoice = true;
                }
            }

            game->mainMap->setNPCArmies(territoryId, game->mainMap->getNPCArmies(territoryId)+1);
            totalArmies++;
        }
    }
    game->mainMap->showMapASCIIonScreen(); //show final result.

    // each player places their bids
    for (auto player : game->players) {
        if (player == nullptr) continue; // no player here

        int bidAmount = 0;
        if (player->getPlayerStrategy()->isBot()) {
            bidAmount=rand()%(player->getCoins()/2); //bot bets up to 50% of its coins
            cout << "Bot randomly bid " << bidAmount << endl;
        } else {
            //human player
            cout << player->getName() << " (Player ID: " << player->getId() << ") has " << player->getCoins() << " coins available." << endl;
            bool validChoice = false;
            while (!validChoice) {
                cout << "Enter your bid amount: ";
                cin >> bidAmount;
                while (cin.fail()) {
                    cout << "Invalid bid amount. Try again: ";
                    cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cin >> bidAmount;
                }
                validChoice = true;
            }
        }

        player->getBiddingFacility()->placeBid(bidAmount);
        bids.push_back(player->getBiddingFacility());
    }

    // players place the coin tokens in a pile next to the board
    cout << "Players place their coin tokens in a pile next to the board." << endl;

    // determine winning bidder
    int winningBidder = game->players[0]->getBiddingFacility()->determineWinningBidder(bids);
    game->setStartingPlayer(winningBidder); //tell game engine who starts

    // winning bidder pays their coins
    for (auto player : game->players) {
        if (player == nullptr) continue; // no player here

        if (player->getId() == winningBidder) {
            cout << "*****" << player->getName() << " (Player ID: " << winningBidder << ") is the winning bidder and will start first " << "*****" << endl;
            player->PayCoin(player->getBiddingFacility()->getBid());
        }
    }
}
//for demonstration only
void part2Demo() {
    cout << "Starting demo for part 2..." << endl;
    mainGame = new GameEngine();
    GameLoader *loader = new GameLoader();
    loader->gameStart(mainGame);
    delete loader;
    GameStartup *starter = new GameStartup();
    starter->startupPhase(mainGame);

    cout << "Cards shuffled and 6 put into hand: " << *mainGame->hand << endl;
    for (int i=0;i<4;i++) {
        if (mainGame->players[i]==nullptr)
        {
            continue;
        }
        cout << "DEMO: " << *mainGame->players[i]<< " has " << mainGame->players[i]->getCoins() << " coins." << endl;
        cout << "DEMO: " << *mainGame->players[i]<< " has " << mainGame->players[i]->getUnplacedArmy() << " unplaced armies." << endl;
        cout << "DEMO: " << *mainGame->players[i]<< " has " << mainGame->players[i]->getUnplacedCity() << " unplaced cities." << endl;
        for (Territory* & t : *mainGame->mainMap->getTerritories()) {
            if (mainGame->mainMap->getArmies(i,t->getID())>0) {
                cout << "Territory " << t->getID() << " : " << mainGame->mainMap->getArmies(i,t->getID()) << " armies" << endl;
            }
        }

    }

}
