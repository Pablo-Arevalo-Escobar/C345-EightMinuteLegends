//
// Created by Kevin Li Zhong Hao on 2021-02-07.
//
#include "PlayerDriver.h"
#include "Cards.h"
#include "Player.h"

int testPlayer(){
    cout << "testPlayer depricated in ass2" << endl;
   /* Card *c1=new Card{"Stronghold",DIRE,PLUS_1VP_PER_DIRE_CARD,NO_GOOD,true,false,BUILD_NEW_CITY,BUILD_NEW_CITY}; //create a card
    Card *c2=new Card{"Strongholdv2",DIRE,PLUS_1VP_PER_DIRE_CARD,NO_GOOD,true,false,BUILD_NEW_CITY,BUILD_NEW_CITY}; //create a card


    Player p1 = Player(1, "Kevin", 0, 10, 3, 18); //create player1

    Player* ptr_player2{}; //create a player pointer
    Player p2 = Player(2, "Bob", 0, 10, 3, 18); //create player2
    ptr_player2 = &p2; //assign pointer to player2

    p1.AddCard(c1); //add a card to p1's card stash
    cout << " Added card: " << *c1 << " to player " << p1.getName() << endl;
    p2.AddCard(c2); //add a card to p2's card stash
    cout << " Added card: " << *c2 << " to player " << p2.getName() << endl;

    cout << "\n==Test stream operator==" << endl;
    cout << p1; //test stream operator

    cout << "\n==Test coin payment==" << endl;
    p1.PayCoin(5); //test payCoin

    // create simple map and add a few armies
    mainMap=new Map(); //create empty map for testing
    for (int i=0;i<10;i++) {
        mainMap->addTerritory(new Territory(i,0,"Dummy Territory " + to_string(i)));
        mainMap->setArmies(p1.getId(),i,5); //add 5 armies to this territory i for player 1
        mainMap->setArmies(p2.getId(),i,2);//add 2 armies to this territory i for player 3
    }
    mainMap->setArmies(p2.getId(),5,100); //make player 2 have 1 big territory

    cout << "\n==Test placing a city in region 0 for player one==" << endl;
    p1.BuildCity(0); //p1 builds a city in region 0

    cout << "\n==Test placing 5 armies of player one in region 0==" << endl;
    p1.PlaceNewArmies(0, 5); //p1 puts 5 armies in region 0

    cout << "\n==Test placing 2 armies of player one in region 3 without having a city and without the region being the default spawn==" << endl;
    p1.PlaceNewArmies(3, 2); //p1 puts 2 armies in region 3, but region 3 has no city

    cout << "\n==Test placing more armies than the user has in hand in region 0==" << endl;
    p1.PlaceNewArmies(0, 100); //p1 puts 100 armies in region 0

    cout << "\n==Test moving one army of player one from region 0 to region 1==" << endl;
    //p1.MoveArmies(c1, 0, 1, 1); //p1 moves 1 army from region 0 to region 1

    cout << "\n==Test destroying one of player two's army in region 0==" << endl;
    p1.DestroyArmy(0, ptr_player2); //p1 destroys an army of p2 in region 0

    cout << "\n==Print out player one post actions==" << endl;
    cout << p1;

    cout << "\nOwnership details post actions" << endl;
    cout << "Player1: " << p1.getOwnedStats() << endl; //player 1 owns regions 0, 1, 2, 3, 4, 6, 7, 8, 9, but no continent
    cout << "Player2: " << p2.getOwnedStats() << endl; //player 2 owns region 5 only but the entire continent with id 0

    cout << "Player1: COMPUTING SCORE - " << p1.ComputeScore() << endl;
    cout << "Player2: COMPUTING SCORE - " << p2.ComputeScore() << endl;


    cout << "\nTest assignment operator, making p1 into p2" << endl;
    p1 = p2; //test assignment operator
    cout << p1;
*/
    return 0;
}
