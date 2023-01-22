//
// Created by Talha Multani on 2021-02-16.
//


#include "MapLoader.h"
#include "Map.h"
#include <fstream>
#include <iostream>
#include <filesystem> //to test if files exist

using namespace std;

MapLoader::MapLoader() = default;

//Destructor
MapLoader::~MapLoader() {
    cout << "MEMORY: Map Loader Destructor Complete" << endl;
    //map not deleted from heap because passed to calling function
}

//Copy Constructor
MapLoader::MapLoader(const MapLoader &maploader) {
    cout << "Copy Constructor Called" << endl;
}

//Overloaded Assignment Operator
MapLoader &MapLoader::operator=(const MapLoader &maploader) {
    cout << "Overloaded Assignment Opertator Called" << endl;
    return *this;
}

//insertion stream operator
ostream &operator<<(ostream &stream, MapLoader &maploader) {
    return stream << "Map Loader";
}

// Attempt to read file into map.  Returns nullptr if failed
Map *MapLoader::readFile(string filename) {

    Map *testMap = new Map(); //Map object to be used and returned for valid map file
    bool checkerror = false; //one catch variable for all errors

    //look for file in either default path ("") or in custom path.  Required since we're testing in different systems.
    //string pathPrefix="";
    filesystem::path checkForFile{pathPrefix+"RectangleMap.txt"};
    if (!std::filesystem::exists(checkForFile)) {
        //pathPrefix = "/Users/pabloarevalo/CLionProjects/C345-Team-Assignment/";
        pathPrefix="/Users/derek/dev/CLionProjects/C345_Ass2/";
        //pathPrefix="/Users/Talha/Documents/COMP 345/C345-Team-Assignment/";
        //"/Users/kevinli/Desktop/branch_v3/C345-Team-Assignment/"
    }


    ifstream mapFile(pathPrefix + filename);
    string answer;
    int id; //RegionID
    int x; //X-coordinate
    int y; //y-coordinate
    int id1; //Path ID1
    int id2; //Path ID2
    int continentID; //ContinentID
    string line;
    string name; //Continent Name
    string pathtype; //path type
    //if file is open read else print that file does not exist or can not be opened return null Map pointer
    if (mapFile.is_open()) {
        //check if file is empty, else read file
        if (is_empty(mapFile)) {
            cout << "File is empty. Can't Read." << endl;
            checkerror = true;
        } else {
            //try/catch so if file does not follow correct format it will print an error f invalid format
            try {
                getline(mapFile, line);//Removes first line of useless info
                //Read file
                while (getline(mapFile, line)) {
                    //if path information part arrives than break and move on to path information dissection
                    if (line.compare("Path Information") == 0) {
                        break;
                    }

                    id = stoi(line.substr(0, 2));
                    continentID = stoi(line.substr(3, 4));
                    //name = line.substr(5, line.size()); //version before x,y
                     x= stoi(line.substr(5, 7)); //added support for x,y
                     y= stoi(line.substr(8, 10)); //added support for x,y
                     name = line.substr(11, line.size()); //added support for x,y


                    // add territory and if false returned make checkerror true
                    if (!(testMap->addTerritory(new Territory(id, continentID,x,y, name))))
                        checkerror = true;
                }
                //reading paths from file and creating them
                while (mapFile >> id1 >> id2 >> pathtype) {


                    //if path creation returns false error check is true
                    if (!(testMap->createPath(id1, id2, string_to_enum(pathtype))))
                        checkerror = true;
                }
            }
                //if try fails Map is not correct format return null pointer
            catch (...) {
                cout << "Map can not be created because Map file is not in a correct format.  Please make the necessary changes and try again!" << endl;
                mapFile.close();
                delete testMap;
                testMap = nullptr;
                return testMap;
            }
        }

        //if checkerror is true or testMap validation fails. print error statemetn and return null pointer map
        if (checkerror || testMap->validate() == 0) {
            if (testMap->validate() == 0) {
                testMap->showMapLoadResults();
            }
            cout << "Map can not be created due to above errors. Please make the necessary changes and try again!" << endl;
            mapFile.close();
            delete testMap;
            testMap = nullptr;
            return testMap;
        }

            //if test Map is valid print details issue wass details were printing each time validatei s being called and we only need it once,it's safe to assume that if it reaches this else it means it's valid
        else {
            if (SHOW_DEBUG_MAPLOADER) {
                cout << "The Map file is valid, would you like to see the details(Y/N)?" << endl;
                cin >> answer;
                if (answer.compare("Y") == 0 || answer.compare("y") == 0) {
                    testMap->showMapLoadResults();
                    cout << "Map validation = " + std::to_string(testMap->validate()) << endl;
                } else
                    cout << "Moving on to Players then" << endl;
            } else {
                cout << "Map loaded successfully:" << endl;
                testMap->showMapASCIIonScreen();
            }

            return testMap;
        }
    } else {
        //file could not be found at all.
        cout << "This file can not be opened. Please try again!" << endl;
        cout << "Check your directory path in MapLoader (about line 50)." << ANSI_COLOR_RED << " Currently set to " + pathPrefix <<  ANSI_COLOR_RESET << endl;
        mapFile.close();
        delete testMap;
        testMap = nullptr;
        return testMap;
    }

}

//Validates Map file and ask if user wants details
void MapLoader::validator(Map *myMap) {
    bool validated = myMap->validate();
    string answer;
    if (validated) {
        cout << "The Map file is valid, would you like to see the details(Y/N)?" << endl;
        cin >> answer;
        if (answer.compare("Y") == 0) {
            myMap->showMapLoadResults();
            cout << "Map validation = " + std::to_string(validated) << endl;
        } else
            cout << "Thank you and have a good day" << endl;
    }

}

//Checks if file is empty
bool MapLoader::is_empty(ifstream &file) {
    return file.peek() == ifstream::traits_type::eof();
}


//ask user for file
string MapLoader::fileInput() {
    bool invalid_selection=false;
    string filename;
    do {
        cout << "Map Directory:" << endl << "0: RectangleMap" << endl << "1: LShapeMap" << endl
             << "2: DuplicateID(Error)" << endl << "3: Territory Non-existent (Error)"
             << endl << "4: Territory No-path (Error)" << endl << "5: Empty File (Error)" << endl
             << "6: Territory Not Connected (Error)" << endl << "7: Gibberish(Error)"<<endl;
        cout << "Please enter a number corresponding to the game board you wish to play on:" << endl;
        int x;
        cin >> x;
        switch (x) {
            case 0:
                filename = "RectangleMap.txt";
                break;
            case 1:
                filename = "LShapeMap.txt";
                break;
            case 2:
                filename = "DuplicateID.txt";
                break;
            case 3:
                filename = "Territory_NE.txt";
                break;
            case 4:
                filename = "Territory_NP.txt";
                break;
            case 5:
                filename = "Empty_Test.txt";
                break;
            case 6:
                filename = "Not_Connected_Test.txt";
                break;
            case 7:
                filename = "Gibberish_Test.tx";
                break;
            default:{
                cout << "Please enter a valid number!"<<endl;
                invalid_selection=true;
            }

        }
    }while(invalid_selection);
    return filename;
}
