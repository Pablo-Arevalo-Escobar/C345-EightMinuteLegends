//
// Created by Talha Multani on 2021-02-16.
//

#ifndef C345_ASS1_MAPLOADER_H
#define C345_ASS1_MAPLOADER_H
#define SHOW_MEMORY_MAPLOADER true
#define SHOW_DEBUG_MAPLOADER false

#include <string>
#include "Map.h"

using namespace std;


class MapLoader {

  private:
    static void validator(Map *myMap);  //for future use
    bool is_empty(ifstream &file); //check if file is empty
    string pathPrefix; //to Locate position of map files on your HD
  public:
    MapLoader(); //default
    ~MapLoader(); //destructor
    MapLoader(const MapLoader& maploader); //copy
    MapLoader& operator=(const MapLoader& maploader); //assugnment
    friend ostream & operator<< (ostream &stream,  MapLoader &maploader); //stream output


    Map *readFile(string name = "filename");
    string fileInput();

};

#endif //C345_ASS1_MAPLOADER_H
