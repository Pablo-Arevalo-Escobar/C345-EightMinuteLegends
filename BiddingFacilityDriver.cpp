//
// Created by Jay on 2021-02-13.
//

#include <string>
#include <iostream>
#include <vector>
#include "Player.h"
#include "BiddingFacilityDriver.h"

int testBid() {
    // card setup
    list<Card*> p1Cards;
    Card c1{"Stronghold",DIRE,PLUS_1VP_PER_DIRE_CARD,NO_GOOD,true,false,BUILD_NEW_CITY,BUILD_NEW_CITY};
    Card c2{"Graveyard",NO_TYPE,PLUS_1VP_PER_CURSED_CARD,NO_GOOD,false,false,NO_ABILITY,NO_ABILITY};
    p1Cards.push_back(&c1);
    p1Cards.push_back(&c2);

    list<Card*> p2Cards;
    Card c3{"Noble Knight",DIRE,PLUS_1VP_PER_DIRE_CARD,NO_GOOD,false,false,BUILD_NEW_CITY,NO_ABILITY};
    Card c4{"Forest Gnome",NO_TYPE,PLUS_1VP_PER_CURSED_CARD,NO_GOOD,true,false,BUILD_NEW_CITY,BUILD_NEW_CITY};
    p2Cards.push_back(&c3);
    p2Cards.push_back(&c4);

    // player setup
    Player* player1;

    Player p1(1, "Kevin", 0, 10, 3, 18, 0, 0 ,0);
    player1 = &p1;

    Player* player2;
    Player p2(2, "James", 0, 10, 3, 18, 0, 0, 0);
    player2 = &p2;

    // create simple map and add a few armies
    Map *mainMap=new Map(); // create empty map for testing
    for (int i=0;i<10;i++) {
        mainMap->addTerritory(new Territory(i,0,"Dummy Territory " + to_string(i)));
        mainMap->setArmies(p1.getId(),i,5); //add 5 armies to this territory i for player 1
        mainMap->setArmies(p2.getId(),i,2);//add 2 armies to this territory i for player 3
    }

    cout << "==================== Show that the player who bid the most coins wins ====================" << endl;
    player1->getBiddingFacility()->placeBid(5);
    player2->getBiddingFacility()->placeBid(4);
    vector<BiddingFacility*> bids = {player1->getBiddingFacility(), player2->getBiddingFacility()};
    int winner = player1->getBiddingFacility()->determineWinningBidder(bids);

    cout << "==================== Show that the player with the alphabetical name wins when all bids are tied ====================" << endl;
    player1->getBiddingFacility()->placeBid(3);
    player2->getBiddingFacility()->placeBid(3);
    vector<BiddingFacility*> bids2 = {player1->getBiddingFacility(), player2->getBiddingFacility()};
    int winner2 = player1->getBiddingFacility()->determineWinningBidder(bids2);

    cout << "==================== Show that the player with the alphabetical name wins when all bids are zero ====================" << endl;
    player1->getBiddingFacility()->placeBid(0);
    player2->getBiddingFacility()->placeBid(0);
    vector<BiddingFacility*> bids3 = {player1->getBiddingFacility(), player2->getBiddingFacility()};
    int winner3 = player1->getBiddingFacility()->determineWinningBidder(bids3);

    delete mainMap;
    mainMap= nullptr;

    return 0;
}