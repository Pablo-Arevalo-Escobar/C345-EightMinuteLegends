//
// Created by Derek James on 2021-02-01.
//

#include "MapDriver.h"
#include "Map.h"
#include <iostream>

using std::cout;
using std::endl;


void testMap() {
    cout << "Map tester started..." << endl;

    Map *myMap=new Map();

    Territory *t1 = new Territory(0,0,"Nora's Farm");
    Territory *copyTest=new Territory(*t1);
    delete t1; //test deleting
    t1= nullptr;

    myMap->setStartZone();

    //myMap->addTerritory(new Territory(0,0,"Nora's Farm"));
    myMap->addTerritory(copyTest);
    cout << "Copy Constructor done on " << *copyTest << endl;
    myMap->setStartZone();
    //test assignment operator
    Territory *t2 = new Territory(1,0,"Rocky Peak");
    Territory *empty=new Territory();
    *empty=*t2; //assignment operator test
    delete t2;

    //myMap->addTerritory(new Territory(1,0,"Rocky Peak"));
    myMap->addTerritory(empty);
    //delete empty;

    myMap->addTerritory(new Territory(2,1,"Warsong Gulch"));
    myMap->addTerritory(new Territory(3,1,"The Barrens"));
    myMap->addTerritory(new Territory(4,2,"Land O'No'Where"));
    myMap->addTerritory(new Territory(5,1));  //using no name
    myMap->addTerritory(new Territory(1,1,"Duplicate ID test"));  //should fail.
    myMap->addTerritory(new Territory(10,4,"Centurio!"));
    myMap->addTerritory(new Territory(11,10,"Cosmo!"));
    myMap->addTerritory(new Territory(12,10,"Whatever-o!"));

    cout << "Creating paths...." << endl;
    myMap->createPath(0,1,PATH_LAND);
    myMap->createPath(1,0,PATH_LAND); //should fail.  Previously connected.
    myMap->createPath(0,2,PATH_LAND);
    myMap->createPath(0,3,PATH_SEA);


    myMap->createPath(0,4,NOT_CONNECTED); // should fail
    myMap->createPath(10,10,PATH_LAND); // should fail because links to itself.
    myMap->createPath(999,10,PATH_LAND); // should fail because first doesn't exist.
    myMap->createPath(10,999,PATH_LAND); // should fail because second doesn't exist.
    myMap->createPath(999,9999,PATH_LAND); // should fail because both don't exist.
    myMap->createPath(10,11,PATH_LAND); // Should work
    myMap->createPath(0,11,PATH_LAND); // Should work
    myMap->createPath(0,11,PATH_LAND); // Should fail since already connected
//
    myMap->createPath(4,5,PATH_LAND); // Connected these two, but are unrachable from 0

    cout << "About to validate map...." << *myMap <<  endl;  //stream test
    bool validated=myMap->validate();
    myMap->showMapLoadResults();
    cout << "Map validation = " + std::to_string(validated) << endl;


    myMap->setArmies(2,2,20); //add 20 armies on territory 2 for player 2
    myMap->setArmies(2,3,10); //add 10 armies on territory 3 for player 2
    myMap->buildCity(3,5);

    //test listing continents
    list<int>* continents=myMap->getAllContinentIDs();
    cout <<"List of continents: ";
    for (int & i : *continents) {
         cout << std::to_string(i) << " ";
    }
    cout << endl;
    delete continents;

    //test getting list of territories per player
    list<Territory*>* playerLands;
    for (int i=0;i<4;i++) {
        playerLands=myMap->getTerritoriesOfPlayer(i);
        cout << "Player " << i << " in involved with " << playerLands->size() << " territories." << endl;
        delete playerLands;
    }
    playerLands= nullptr;



    myMap->showMapAndPlayers();

//    //test Map copy constructor
//    cout << "TESTING MAP COPY CONSTRUCTOR..... " << endl;
//    Map *mapCopy=new Map(*myMap);
//    mapCopy->validate();
//    mapCopy->showMapAndPlayers();
//    delete mapCopy;
//    mapCopy= nullptr;


//    //test Map Assignment Operator
//    Map *dummy=new Map();
//    *dummy=*myMap;
//    dummy->validate();
//    delete dummy;


    //cleanup
    delete myMap;
    myMap= nullptr;



}


