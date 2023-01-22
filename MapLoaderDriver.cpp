//
// Created by Talha Multani on 2021-02-16.
//

#include "MapLoader.h"
#include "Map.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void testMapLoader();

 void testMapLoader(){
    //create Maploader objectc++

    MapLoader* mapLoader=  new MapLoader();

    cout << "Testing " << *mapLoader << endl;

//    bool valid_selection;
//    string filename;
//
//    //asking user for variou files and reading until user exits
//    do {
//        cout << "Map Directory:" << endl << "0: RectangleMap" << endl << "1: LShapeMap" << endl
//             << "2: DuplicateID(Error)" << endl << "3: Territory Non-existent (Error)"
//             << endl << "4: Territory No-path (Error)" << endl << "5: Empty File (Error)" << endl
//             << "6: Territory Not Connected (Error)" << endl << "7: Gibberish(Error)";
//        cout << "Please enter a number corresponding to the game board you wish to play on:" << endl;
//        int x;
//        cin >> x;
//        switch (x) {
//            case 0:
//                filename = "RectangleMap.txt";
//                break;
//            case 1:
//                filename = "LShapeMap.txt";
//                break;
//            case 2:
//                filename = "DuplicateID.txt";
//                break;
//            case 3:
//                filename = "Territory_NE.txt";
//                break;
//            case 4:
//                filename = "Territory_NP.txt";
//                break;
//            case 5:
//                filename = "Empty_Test.txt";
//                break;
//            case 6:
//                filename = "Not_Connected_Test.txt";
//                break;
//            case 7:
//                filename = "Gibberish_Test.tx";
//                break;
//            default:{
//                cout << "Please enter a valid number";
//                valid_selection=true;
//            }
//
//        }
//    }while(!(valid_selection));
////         string filename= mapLoader->fileInput();
////        if(filename.compare("N")==0){
////            break;
////        }
//        Map* myMap= mapLoader->readFile(filename);
//        delete myMap;
//        myMap= NULL;
//        delete mapLoader;
//        mapLoader=NULL;






    //store Map in Map Pointer
    Map* myMap= mapLoader->readFile("LShapeMap.txt");
    myMap->showMapAndPlayers();

    //Duplicate ID
//    Map* myMap= mapLoader->readFile("DuplicateID.txt");

//Rectangle Map
//    Map* myMap= mapLoader->readFile("RectangleMap.txt");

//   L-Shape Map
//    Map* myMap= mapLoader->readFile("LShapeMap.txt");

//Territory Non-existent
//    Map* myMap= mapLoader->readFile("Territory_NE.txt");

//Path non-existent between existing territories
//    Map* myMap= mapLoader->readFile("Territory_NP.txt");

//Empty Test
//   Map* myMap= mapLoader->readFile("Empty_Test.txt");

//Not Connected test
//    Map* myMap= mapLoader->readFile("Not_Connected_Test.txt");

//Gibberish Test
//    Map* myMap= mapLoader->readFile("Gibberish_Test.txt");

    //cleanup
    delete myMap;
    myMap= NULL;
    delete mapLoader;
    mapLoader=NULL;


};