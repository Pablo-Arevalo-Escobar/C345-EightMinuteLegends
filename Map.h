//
// Created by Derek James on 2021-01-27.
//

#ifndef C345_ASS1_MAP_H
#define C345_ASS1_MAP_H

#include <list>
#include <vector>
#include <string>
#include "Subject.h"

#if defined(__APPLE__) || defined(__linux__) //https://stackoverflow.com/questions/56679782/how-to-use-ansi-escape-sequence-color-codes-for-psreadlineoption-v2-in-powershel
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\e[0;32m" //territories, Card ACTION
#define ANSI_COLOR_YELLOW  "\x1b[33m" //coins,
#define ANSI_COLOR_BLUE    "\x1b[34m" //card TYPE (forest, ancient)
#define ANSI_COLOR_CYAN    "\x1b[36m" //Card GOODS
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_PURPLE   "\e[0;35m"  //card title
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_ACTIVE_PLAYER   "\e[30;103m"
#define ANSI_WHITE_UNDERLINE "\e[4;37m"
#define ANSI_COLOR_BLACK_BACKGROUND "\x1b[30m"
#define ANSI_MAP_LAND "\e[37;43m"
#define ANSI_MAP_LAND_PLAYER_OWNS "\e[30;43m" //has army or city
#define ANSI_MAP_LAND_PLAYER_ABSENT "\e[33;43m" //no army or city
#define ANSI_MAP_LAND_START_ZONE "\e[93;42m" //green start zone text
#define ANSI_MAP_ID "\e[93;45m"
#define ANSI_MAP_LAND_CONNECTED_BY_SEA "\e[30;44m"
#define ANSI_MAP_WATER "\e[37;44m"
#define ANSI_CLEAR_SCREEN "\033[2J"
#define ANSI_YELLOW_ON_BLACK "\e[33;40m"
#define ANSI_STATS_DEFAULT "\e[30;46m"
#define ANSI_STATS_LIGHT "\e[97;101m"
#else
#define ANSI_COLOR_RED     ""
#define ANSI_COLOR_GREEN   ""
#define ANSI_COLOR_YELLOW  ""
#define ANSI_COLOR_BLUE    ""
#define ANSI_COLOR_MAGENTA ""
#define ANSI_COLOR_CYAN    ""
#define ANSI_COLOR_WHITE    ""
#define ANSI_COLOR_BLACK    ""
#define ANSI_COLOR_PURPLE ""
#define ANSI_COLOR_RESET   ""
#define ANSI_ACTIVE_PLAYER ""
#define ANSI_WHITE_UNDERLINE ""
#define ANSI_COLOR_BLACK_BACKGROUND ""
#define ANSI_MAP_LAND ""
#define ANSI_MAP_LAND_PLAYER_OWNS ""
#define ANSI_MAP_LAND_PLAYER_ABSENT ""
#define ANSI_MAP_LAND_START_ZONE ""
#define ANSI_MAP_ID ""
#define ANSI_MAP_LAND_CONNECTED_BY_SEA ""
#define ANSI_MAP_WATER ""
#define ANSI_CLEAR_SCREEN ""
#define ANSI_YELLOW_ON_BLACK ""
#define ANSI_STATS_DEFAULT ""
#define ANSI_STATS_LIGHT ""

#endif

#define SHOW_MEMORY_MAP false
#define SHOW_DEBUG_MAP true

using std::string;
using std::list;
using std::ostream;
using std::vector;

const int MAP_WIDTH=70;
const int MAP_HEIGHT=25;

//The possible ways territories can be connected
enum PATH_TYPE {
    PATH_LAND,
    PATH_SEA,
    NOT_CONNECTED
};
string enum_to_string(PATH_TYPE pathType); //for displaying
PATH_TYPE string_to_enum(string pathType); //for map loading (part 2)

class Territory; //pre-declare to use in struct

//Edge graph struct
struct MapEdge {
    Territory *connectedTerritory;
    PATH_TYPE *pathType;
};

struct MapCell {
    char c;
    string color;
    MapCell()=default;
    MapCell(char c,string color) : c{c},color{color}{};
};

//The smallest division of the map.  Represents one cell of a card
//In terms of a Graph, this is a VERTEX
// Most interactions with the map should be done through the global MAP object, not the territories
class Territory {
  public:
    //constructors/destructors
    Territory(int ID, int continentID, string name = "AUTO");
    Territory(int ID, int continentID, int x, int y, string name = "AUTO");  //used when specifying coordinates
    Territory(); //Default constructor
    Territory(const Territory &obj); // copy constructor.
    Territory &operator=(const Territory &source); //assignment operator
    ~Territory(); //destructor

    //getters
    int getID() const;
    int getContinentID() const;
    int getX() const;  //horizontal position of node (0-99), relative to left;
    int getY() const;  //vertical position of node (0-99), relative to top;
    string getName(); // optional string name of territory.
    string toString();
    friend ostream &operator<<(ostream &stream, const Territory &other); //stream insertion operator

    //Used for external map drawing only
    list<MapEdge *> *getAdjacentTerritories() const;

  private:
    int armyCount[4] = {0, 0, 0, 0}; //track number of armies each player has in this territory
    int npcArmyCount{0}; //number of NPC armies on this spot.
    bool playerHasCity[4] = {false, false, false, false}; //track if player has a city in this territory.
    list<MapEdge *> *adjacentTerritories; //list of edges as adjacency list
    int id{}; //can be set by constructor only
    int continentID{};  //can be set by constructor only
    int x{}, y{}; //for relational mapping.  set by constructor only. optional for now.
    string name; //optional name of territory.  set in constructor
    PATH_TYPE isConnectedTo(Territory *otherTerritory);  //Only used for create-path.  Outside this class, go through Map.checkpath
    bool visited{};//used to validate if graph is connected during DFS.
    void deleteHeapItems(); //helper function to help memory management
    void deepCopyMapEdges(const Territory &other); //common helper function used by deep copy of c.c and operator=

    friend class Map; //share privates with Map class.
};

//Full map.  Contains all territories.
//Most game operations should be done though the Map object instead of the individual territories.
class Map {
  public:
    //constructors/destructors
    Map(); //default constrcutor
    Map(const Map &obj); //copy constructor.  Map edges ARE recreated accurately.
    Map &operator=(const Map &source); //assignment operator
    ~Map(); //destructor

    //methods
    //Used during Map Creation
    bool addTerritory(Territory *t); //using during the map-creation process
    bool createPath(int ID1, int ID2, PATH_TYPE pathType); //link territories together by Path_Type
    bool validate() const; //check that the map is valid.  Use after map creation.  Also dumps lists to screen.
    void showMapLoadResults() const; //show results of Map Creation Process.  Used to help map loader

    //Used during setup of game
    void setStartZone();  //players choose start zone at start of game.

    //Oberservable Methods
    void buildCity(int playerid, int territoryid);// add city belonging to playerid to a territory
    void setNPCArmies(int territoryid, int numArmies); //set number of NPC armies on a territory
    void setArmies(int playerid, int territoryid, int numArmies); // set armies of playerid to this territory.  Also use for delete.
    bool refreshStats{false}; //tells observers WHY notify was called

    //Used during the Game
    void showMapAndPlayers() const; //dump current "map" as list to console.  Also show Player info.  Used during the console version of game.
    std::vector<string>* generateASCIIMap(const int fromid=-1) const; //Build ascii map.  Use FROMID when presenting a choice of connected territories
    void showMapASCIIonScreen(const int fromid=-1); //Prints map (in color) to the screen, but not the whole board.  fromid used for path perspective
    PATH_TYPE checkPath(int territory1, int territory2) const; //Used to check if two territories are adjacent.  Returns PATH_LAND, PATH_SEA, or NOT_CONNECTED
    bool hasCity(int playerid, int territoryid) const; //returns true/false if player has city in that territory
    int getStartZone(); //get territoryid of start zone.
    int getArmies(int playerid, int territoryid) const; //returns # of armies of player in this territory.  (Note that cites are NOT counted for this operation).
    int getNPCArmies(int territoryid) const; //returns number of NPC armies on a territory
    int whichPlayerControlsTerritory(int territoryid) const; //returns -1 if no one controls it (or npc). otherwise 0-3
    int whichPlayerControlsContinent(int continentid) const; //returns -1 if no one controls it (or npc). otherwise 0-3
    int howManyTerritoriesDoesPlayerControl(int playerid); //used to determine when to update Stats observer
    int howManyContinentsDoesPlayerControl(int playerid); //used to determine when to update Stats observer
    int howManyArmiesDoesPlayerHaveOnMap(int playerid); //used to determine when to update Stats observer
    int howManyCitiesDoesPlayerHave(int playerid); //used to determine when to update Stats observer
    Territory *getTerritory(int ID) const; //In case we need access to a territory object.  Shouldn't have to (better to use other Map methods).
    bool doesTerritoryExist(int ID); //check if ID is valid;
    friend ostream &operator<<(ostream &stream, const Map &other); //stream insertion operator
    list<Territory *> *getTerritoriesOfPlayer(int playerid); //return list of pointers to territories player has an army or city on.  Created on stack to prevent forgetting to clear from heap
    list<int>* getAllContinentIDs(); //return a list of continent ids.  Created on stack to prevent forgetting to clear from heap.
    int getNumberOfContinents(); // How many continents do we have?
    int getRandomValidTerritoryID(); //ensures the random territory is a valid one.

    //Used for external map drawing only
    list<Territory *> *getTerritories() const; //return list of territories.  Possible privacy leak.

  private:
    int startZone = -1; //-1 indicates not set.
    list<Territory *> *territories; //internal list of territories contained in this map.
    void dfs(Territory *t) const; //depth-first search to see if graph fully connected.
    void deleteHeapItems(); //helper function to help memory management
    void deepCopyAndRelinkTerritories(const Map &other); //common helper function used by deep copy of c.c and operator=
};

#endif //C345_ASS1_MAP_H
