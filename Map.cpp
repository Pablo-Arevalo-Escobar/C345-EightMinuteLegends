//
// Created by Derek James on 2021-01-27.
//

#include "Map.h"
#include <iostream>
#include <iomanip>  //for prettier table-like printing using setw (see showMapLoadResults for example)
#include <sstream>
#include <stdlib.h> //for random numbers

using std::cout;
using std::endl;
using std::to_string;
using std::setw;
using std::left;
using std::right;
using std::stringstream;
using std::srand;
using std::rand;

//constructors

//default constructor
Territory::Territory() {
    adjacentTerritories = new list<MapEdge *>();
    id=-1;
    x=0;
    y=0;
    visited= false;
    continentID=-1;
    name="<Unnamed>";
}

//standard parameterized constructor
Territory::Territory(int ID, int continentID, string name) : Territory() {
    id = ID;
    x = -1;
    y = -1;
    visited = false;
    this->continentID = continentID;
    if (name == "AUTO") {
        this->name = "Territory " + to_string(id);
    } else {
        this->name = name;
    }
}

//optional constructor when specifying x and y coordinates.
Territory::Territory(int ID, int continentID, int x, int y, string name) : Territory(ID, continentID, name) {
    this->x = x;
    this->y = y;
}

//copy constructor (deep copy).  Note that MapEdges are NOT copied since they may point to territories on another map (if this is called from Map copy constructor)
Territory::Territory(const Territory &obj) : Territory() {
    cout << "DEBUG:  Territory Copy Constructor called" << endl;
    this->x = obj.x;
    this->y = obj.y;
    this->id = obj.id;
    this->visited = obj.visited;
    this->continentID = obj.continentID;
    this->npcArmyCount=obj.npcArmyCount;
    for (int i = 0; i < 4; i++) {
        this->playerHasCity[i] = obj.playerHasCity[i];
        this->armyCount[i] = obj.armyCount[i];
    }
    this->name = obj.name;
    //copy items in the adjacency list
    // BE VERY CAREFUL with this because it might refer to territories that no longer exist (for example, in Map copy constructor)
    this->deepCopyMapEdges(obj);
}

//assignment operator
Territory &Territory::operator=(const Territory &source) {
    if (SHOW_DEBUG_MAP) {
        cout << "DEBUG: Territory Assignment Operator= started" << endl;
    }

    if (this == &source) {
        return *this;
    }
    this->deleteHeapItems();  //prevent sneaky memory leak

    this->x = source.x;
    this->y = source.y;
    this->id = source.id;
    this->visited = source.visited;
    this->continentID = source.continentID;
    for (int i = 0; i < 4; i++) {
        this->playerHasCity[i] = source.playerHasCity[i];
        this->armyCount[i] = source.armyCount[i];
    }
    this->name = source.name;
    //copy items in the adjacency list
    // BE VERY CAREFUL with this because it might refer to territories that no longer exist (for example, in Map copy constructor)
    this->deepCopyMapEdges(source);
    return *this;
}

//Helper function for copying MapEdges (common to both copy constructor and assignment operator)
void Territory::deepCopyMapEdges(const Territory &other) {
    MapEdge *eCopy = nullptr;
    for (MapEdge *&e : *other.adjacentTerritories) {
        eCopy = new MapEdge();
        eCopy->connectedTerritory = e->connectedTerritory; //<-- possible source of dangling pointer.  Be careful how you use.
        eCopy->pathType = new PATH_TYPE{*e->pathType};
        this->adjacentTerritories->push_back(eCopy);
    }
}

//helper function for destructor and assignment operator
void Territory::deleteHeapItems() {
    if (this->adjacentTerritories != nullptr) {
        int edgesDeleted = 0;
        for (MapEdge *&e: *this->adjacentTerritories) {
            delete e;
            edgesDeleted++;
        }
        adjacentTerritories->clear(); //clear but do NOT delete the heap item.  Might need to re-use it.
        if (SHOW_MEMORY_MAP) {
            cout << "MEMORY: " << edgesDeleted << " edges deleted." << endl;
        }

    }
}

//Destructor
Territory::~Territory() {
    if (SHOW_MEMORY_MAP) {
        cout << "MEMORY: Territory Destructor Started for " << this->name << endl;
    }
    this->deleteHeapItems();
    delete this->adjacentTerritories;
    this->adjacentTerritories = nullptr;
}

//Map default constructor
Map::Map() {
    territories = new list<Territory *>();
}

//Map copy constructor (deep copy).  Re-creates the Map Edges
Map::Map(const Map &obj) : Map() {
    this->startZone = obj.startZone;
    //deep-copy all the territories.
    this->deepCopyAndRelinkTerritories(obj);
}

//helper function to handle common copy procedure of map copy constructor and assignment operator
void Map::deepCopyAndRelinkTerritories(const Map &other) {
    int countNodes = 0;
    int countEdges = 0;
    Territory *tCopy = nullptr;

    //Copy territories and strip away MapEdges since they point to old map's territories
    for (Territory *&t : *other.territories) {
        tCopy = new Territory(*t);
        tCopy->deleteHeapItems();  //Throw away any map edges in the copy since they will still point to old map. (Will recreate after all added)
        this->territories->push_back(tCopy);
        countNodes++;
    }

    //now re-create all the map edges on the copy the same way as the original. Now all the territory pointers are correct for this copy of the map.
    int id1, id2;
    for (Territory *&t : *other.territories) {
        id1 = t->id;
        for (MapEdge *&e : *t->adjacentTerritories) {
            id2 = e->connectedTerritory->id;
            if (this->checkPath(id1, id2) == NOT_CONNECTED) //only call once instead of twice when connecting
            {
                this->createPath(id1, id2, *e->pathType);
                countEdges++;
            }
        }
    }
    cout << "DEBUG: Map deep-copied " << countNodes << " territories with " << countEdges << " edges" << endl;
}

//helper function for destructor and assignment operator
void Map::deleteHeapItems() {
    if (this->territories != nullptr) {
        int count = 0;
        for (Territory *&t: *this->territories) {
            delete t;
            count++;
        }
        territories->clear(); //clear but do NOT delete the heap item.  Might need to re-use it.
        if (SHOW_MEMORY_MAP) {
            cout << "MEMORY: " << count << " territories deleted from map." << endl;
        }

    }
}

//Map destructor
Map::~Map() {
    if (SHOW_MEMORY_MAP) {
        cout << "MEMORY: Map Destructor Started" << endl;
    }

    this->deleteHeapItems();
    delete this->territories;
    this->territories = nullptr;
}

// Functions
//getters
int Territory::getID() const { return this->id; }

int Territory::getContinentID() const { return this->continentID; }

int Territory::getX() const { return this->x; }

int Territory::getY() const { return this->y; }

string Territory::getName() { return this->name; }

//Create edge in graph.  Both territories must already exist in the Map.
bool Map::createPath(int ID1, int ID2, PATH_TYPE pathType) {
    Territory *t1 = this->getTerritory(ID1);
    Territory *t2 = this->getTerritory(ID2);
    if (t1 == nullptr || t2 == nullptr) {
        cout << "ERROR: Both territories must be added to map before a path can be created (ID " << ID1 << "," << ID2
             << ")" << endl;
        return false;
    }
    if (ID1 == ID2) {
        cout << "ERROR: Cannot create a path between a territory and itself! (ID=" << ID1 << ")" << endl;
        return false;
    }
    if (this->checkPath(ID1, ID2) != NOT_CONNECTED) {
        cout << "ERROR: Territory " << ID1 << " and " << ID2 << " are already connected" << endl;
        return false;
    }
    if (pathType == NOT_CONNECTED) {
        cout << "ERROR: Cannot create path of type NOT_CONNECTED" << endl;
        return false;
    }
    MapEdge *e12 = new MapEdge(); //map in forward direction
    MapEdge *e21 = new MapEdge(); //edge in reverse direction
    e12->connectedTerritory = t2;
    e12->pathType = new PATH_TYPE{pathType};
    //add to BOTH adjacency lists
    t1->adjacentTerritories->push_back(e12);
    e21->connectedTerritory = t1;
    e21->pathType = new PATH_TYPE{pathType};
    t2->adjacentTerritories->push_back(e21);
    return true;
}

//how is this territory connected to another?
PATH_TYPE Territory::isConnectedTo(Territory *otherTerritory) {
    for (MapEdge *&e: *this->adjacentTerritories) {
        if (e->connectedTerritory->id == otherTerritory->id) {
            return *e->pathType;
        }
    }
    return NOT_CONNECTED;
}

//soon to be obsolete.
string Territory::toString() {
    return "Region: " + name;
}

//required for external map drawing.  possible privacy leak.
list<MapEdge *> *Territory::getAdjacentTerritories() const {
    return this->adjacentTerritories;
}

//stream insertion operator
ostream &operator<<(ostream &stream, const Territory &other) {
    return stream << "Territory " << other.id << " [" << other.name << "]";
}

//main method to add a territory during map creation process.
bool Map::addTerritory(Territory *t) {
    if (this->getTerritory(t->id) != nullptr) {
        cout << "ERROR: Territory with ID " << t->id << " already exists on map!  Cannot add. Object deleted from heap." << endl;
        delete t; //clear heap of failed addition
        return false;
    }
    this->territories->push_back(t);
    return true;
}

//another way to see how two territories are connected
PATH_TYPE Map::checkPath(int territory1, int territory2) const {
    PATH_TYPE answer = NOT_CONNECTED;
    const Territory *t1 = this->getTerritory(territory1);
    const Territory *t2 = this->getTerritory(territory2);
    if (territory1 != territory2) {
        if (t1 != nullptr && t2 != nullptr) {
            for (MapEdge *&e : *t1->adjacentTerritories) {
                if (e->connectedTerritory->id == territory2) {
                    answer = *e->pathType;
                }
            }
        }
    }
    return answer;
}

//pretty-print path types
string enum_to_string(PATH_TYPE pathType) {
    switch (pathType) {
        case PATH_LAND:
            return "Land";
        case PATH_SEA:
            return "Sea";
        case NOT_CONNECTED:
            return "<Not Connected>";
        default:
            return "<Invalid PATH_TYPE>";
    }
}

//used during file load process
PATH_TYPE string_to_enum(string pathType) {
    if (pathType.compare("PATH_LAND") == 0)
        return PATH_LAND;
    else if (pathType.compare("PATH_SEA") == 0)
        return PATH_SEA;
    else if (pathType.compare("NOT_CONNECTED") == 0)
        return NOT_CONNECTED;
    else {
        cout << "ERROR: Cannot determined PATH_TYPE of '" << pathType << "'" << endl;
        return NOT_CONNECTED;
    }
}

//get access to a territory by id.
Territory *Map::getTerritory(int ID) const {
    for (Territory *&t: *territories) {
        if (t->id == ID) {
            return t;
        }
    }
    return nullptr;
}

//Show state of Map creation process.  Only intended to be used during map creation.
//is called from within validate.  If called directly, make sure validate has already occured.
void Map::showMapLoadResults() const {

    const int WIDTH_ID = 2;
    const int WIDTH_NAME = 20;
    const int WIDTH_CONTINENT = 2;
    const int WIDTH_ADJ_LAND = 3;
    const int WIDTH_ADJ_SEA = 3;
    const int WIDTH_OK = 3;
    const int WIDTH_ADJ_TYPE = 4;

    int adj_land = 0;
    int adj_sea = 0;
    bool error = false;
    string s;

    // PART 1:  LIST OF TERRITORIES
    cout << "LIST OF TERRITORIES:" << endl;
    cout << string(WIDTH_ID + WIDTH_NAME + WIDTH_CONTINENT + WIDTH_ADJ_LAND + WIDTH_ADJ_SEA + WIDTH_OK + 6, '-')
         << endl;
    cout << setw(WIDTH_ID) << left << "ID" << " ";
    cout << setw(WIDTH_NAME) << left << "Territory Name" << " ";
    cout << setw(WIDTH_CONTINENT) << left << "CO#" << " ";
    cout << setw(WIDTH_ADJ_LAND) << left << "LND" << " ";
    cout << setw(WIDTH_ADJ_SEA) << left << "SEA" << " ";
    cout << setw(WIDTH_OK) << left << "OK?" << " ";
    cout << endl;
    cout << string(WIDTH_ID + WIDTH_NAME + WIDTH_CONTINENT + WIDTH_ADJ_LAND + WIDTH_ADJ_SEA + WIDTH_OK + 6, '-')
         << endl;

    for (Territory *&t : *this->territories) {
        adj_land = 0;
        adj_sea = 0;
        error = false;
        for (MapEdge *&m : *t->adjacentTerritories) {
            switch (*m->pathType) {
                case PATH_LAND:
                    adj_land++;
                    break;
                case PATH_SEA:
                    adj_sea++;
                    break;
                default:
                    error = true;
            }
        }
        if ((adj_land + adj_sea) == 0) {
            error = true;
        }
        if (t->visited == false) {
            error = true;
        }
        cout << setw(WIDTH_ID) << left << t->id << " ";
        cout << setw(WIDTH_NAME) << left << t->name << "  ";
        cout << setw(WIDTH_CONTINENT) << left << t->continentID << " ";
        cout << setw(WIDTH_ADJ_LAND) << left << adj_land << " ";
        cout << setw(WIDTH_ADJ_LAND) << left << adj_sea << "";
        cout << setw(WIDTH_OK) << left << (error ? "Err" : "Ok") << " ";
        cout << endl;
    }
    cout << string(WIDTH_ID + WIDTH_NAME + WIDTH_CONTINENT + WIDTH_ADJ_LAND + WIDTH_ADJ_SEA + WIDTH_OK + 6, '-')
         << endl;

    //PART 2: ADJANCENCY LISTS
    cout << "LIST OF ADJACENCIES:" << endl;
    cout << string(WIDTH_ID * 2 + WIDTH_NAME * 2 + WIDTH_ADJ_TYPE + 9, '-') << endl;
    cout << setw(WIDTH_ID) << left << "ID" << " ";
    cout << setw(WIDTH_NAME) << left << "Territory" << " <--> ";
    cout << setw(WIDTH_ID) << left << "ID" << " ";
    cout << setw(WIDTH_NAME) << left << "Territory" << " ";
    cout << setw(WIDTH_ADJ_TYPE) << left << "Type" << " ";
    cout << endl;
    cout << string(WIDTH_ID * 2 + WIDTH_NAME * 2 + WIDTH_ADJ_TYPE + 9, '-') << endl;

    for (Territory *&t : *this->territories) {
        for (MapEdge *&m : *t->adjacentTerritories) {
            switch ((int) *m->pathType) {
                case PATH_LAND:
                    s = "LAND";
                    break;
                case PATH_SEA:
                    s = "SEA";
                    break;
                default:
                    s = "???";
            }

            cout << setw(WIDTH_ID) << left << t->id << " ";
            cout << setw(WIDTH_NAME) << left << t->name << " <--> ";
            cout << setw(WIDTH_ID) << left << m->connectedTerritory->id << " ";
            cout << setw(WIDTH_NAME) << left << m->connectedTerritory->name << " ";
            cout << setw(WIDTH_ADJ_TYPE) << left << s << " ";
            cout << endl;
        }
    }
    cout << string(WIDTH_ID * 2 + WIDTH_NAME * 2 + WIDTH_ADJ_TYPE + 9, '-') << endl;
}

//check if map is playable.  Checks for obvious errors and that the graph is connected.
bool Map::validate() const {
    bool answer = true;
    if (this->territories->empty()) {
        answer = false;
        cout << "VALIDATE ERROR: No Territories in Map!" << endl;
        return answer;
    }
    //check if connected.
    //Reset visited
    for (Territory *&t: *territories) {
        t->visited = false;
        if (t->id < 0 || t->id > 99) {
            cout << "VALIDATE ERROR:  TerritoryID must be between 0 and 99" << endl;
            answer = false;
        }
        if (t->continentID < 0 || t->continentID > 99) {
            cout << "VALIDATE ERROR:  ContinentID must be between 0 and 99" << endl;
            answer = false;
        }
    }
    //DO DFS
    Territory *root = this->territories->front();
    dfs(root);
    //now check for any unvisited teritories
    for (Territory *&t: *this->territories) {
        if (t->visited == false) {
            answer = false;
            cout << "VALIDATE ERROR: Territory " << t->id << " cannot be reached from #" << root->id
                 << "  (not a connected graph)" << endl;
        }
    }
    return answer;
}

//recursive depth-first-search function to check if map connected.
void Map::dfs(Territory *t) const {
    t->visited = true;
    for (MapEdge *&e: *t->adjacentTerritories) {
        if (e->connectedTerritory->visited == false) {
            dfs(e->connectedTerritory);
        }
    }
}

//Show Map in-game as console version.
void Map::showMapAndPlayers() const {
    //for IN-GAME-LISTING
    const int BOARD_HEIGHT=25; //how many lines will the "Board" (series of lines) be?
    string lines[BOARD_HEIGHT];
    const int WIDTH_ID = 2;
    const int WIDTH_NAME = 20;
    const int WIDTH_CONTINENT = 2;
    const int WIDTH_ARMIES = 2;
    const int WIDTH_CITIES = 2;

    int adj_land = 0;
    int adj_sea = 0;
    bool error = false;
    string s;
    string line;

    //cout << "GAME MAP:" << endl;
    cout << string(WIDTH_ID + WIDTH_NAME + WIDTH_CONTINENT + 1, '-') << "- A R M I E S --- CITIES --"
         << endl;
    cout << setw(WIDTH_ID) << left << "ID" << " ";
    cout << setw(WIDTH_CONTINENT) << left << "CO" << " ";
    cout << setw(WIDTH_NAME) << left << "Territory" << " ";

    for (int i = 0; i < 4; i++) {
        cout << setw(WIDTH_CONTINENT) << left << "P" + std::to_string(i) << " ";
    }
    cout << " ";
    for (int i = 0; i < 4; i++) {
        cout << setw(WIDTH_CONTINENT) << left << "P" + std::to_string(i) << " ";
    }
    cout << endl;
    line = string(WIDTH_ID + WIDTH_NAME + WIDTH_CONTINENT + (WIDTH_ARMIES + 1) * 4 + (WIDTH_CITIES + 1) * 4 + 8, '-');
    cout << line << endl;

    for (Territory *&t : *this->territories) {

        cout << ANSI_COLOR_YELLOW << setw(WIDTH_ID) << right << t->id << ANSI_COLOR_RESET << " ";
        cout << setw(WIDTH_CONTINENT) << right << t->continentID << " ";
        cout << ANSI_COLOR_BLUE << setw(WIDTH_NAME) << left << t->name << ANSI_COLOR_RESET << " ";
        for (int i = 0; i < 4; i++) {
            cout << ANSI_COLOR_RED << setw(WIDTH_ARMIES) << right << this->getArmies(i, t->getID()) << ANSI_COLOR_RESET
                 << " ";
        }
        cout << " ";
        for (int i = 0; i < 4; i++) {
            cout << ANSI_COLOR_CYAN << setw(WIDTH_CITIES) << left << (this->hasCity(i, t->getID()) ? "*" : " ")
                 << ANSI_COLOR_RESET << " ";
        }
        cout << ANSI_COLOR_RESET << endl;
    }
    cout << line << endl;
    cout << ANSI_COLOR_RESET;
}

//check if player has a city in a territory.
bool Map::hasCity(int playerid, int territoryid) const {
    if (playerid < 0 || playerid > 3) {
        cout << "ERROR: Attempt to access invalid playerid" << playerid << " in Map::hasCity()" << endl;
        return false;
    }
    const Territory *t = this->getTerritory(territoryid);
    if (t == nullptr) {
        cout << "ERROR: Attempt to access invalid territoryid" << territoryid << " in Map::hasCity()" << endl;
        return false;
    }
    return t->playerHasCity[playerid];

}

//build a city for a player in a territory
void Map::buildCity(int playerid, int territoryid) {
    if (playerid < 0 || playerid > 3) {
        cout << "ERROR: Attempt to access invalid playerid" << playerid << " in Map::buildCity()" << endl;
        return;
    }
    Territory *t = this->getTerritory(territoryid);
    if (t == nullptr) {
        cout << "ERROR: Attempt to access invalid territoryid" << territoryid << " in Map::buildCity()" << endl;
        return;
    }
    t->playerHasCity[playerid] = true;
}

//return how many armies a player has on a territory.
int Map::getArmies(int playerid, int territoryid) const {
    if (playerid < 0 || playerid > 3) {
        cout << "ERROR: Attempt to access invalid playerid" << playerid << " in Map::getArmies()" << endl;
        return 0;
    }
    Territory *t = this->getTerritory(territoryid);
    if (t == nullptr) {
        cout << "ERROR: Attempt to access invalid territoryid" << territoryid << " in Map::getArmies()" << endl;
        return 0;
    }
    return t->armyCount[playerid];
}

//set the number of armies a player has on a territory. Called during moving and destroy actions in game.
void Map::setArmies(int playerid, int territoryid, int numArmies) {
    if (playerid < 0 || playerid > 3) {
        cout << "ERROR: Attempt to access invalid playerid" << playerid << " in Map::setArmies()" << endl;
        return;
    }
    Territory *t = this->getTerritory(territoryid);
    if (t == nullptr) {
        cout << "ERROR: Attempt to access invalid territoryid" << territoryid << " in Map::setArmies()" << endl;
        return;
    }
    int origArmies=t->armyCount[playerid];
    t->armyCount[playerid] = numArmies;

}

//return which player controls this territory.  Considers cities in it's calculation.
int Map::whichPlayerControlsTerritory(int territoryid) const {
    int answer = -1;  //default answer:  -1 means no one controls territory
    const Territory *t = this->getTerritory(territoryid);
    if (t == nullptr) {
        cout << "ERROR: Attempt to access invalid territoryid" << territoryid
             << " in Map::whichPlayerControlsTerritory()" << endl;
        return -1;
    }

    int sum[4] = {0, 0, 0, 0};
    int highest = t->npcArmyCount;
    bool tieHighest = false;
    for (int i = 0; i < 4; i++) {
        sum[i] = t->armyCount[i];
        if (t->playerHasCity[i])
            sum[i]++;
        if (sum[i] > 0) {
            if (sum[i] == highest) {
                tieHighest = true;
            }
            if (sum[i] > highest) {
                tieHighest = false;
                highest = sum[i];
                answer = i;
            }
        }
    }
    if (highest > 0) {
        if (tieHighest == true || highest==t->npcArmyCount)
            return -1; //tie for highest, so no player winner.
        else
            return answer;
    } else {
        //no highest.
        return -1;
    }
}

//return which player controls this continent.  Considers cities in it's calculation.
int Map::whichPlayerControlsContinent(int continentid) const {
    int answer = -1;  //default answer:  -1 means no one controls territory
    int sum[4] = {0, 0, 0, 0};
    int npc=0;
    int highest = 0;
    bool tieHighest = false;
    for (Territory *&t: *this->territories) {
        if (t->continentID == continentid) {
            npc += t->npcArmyCount;
            for (int i = 0; i < 4; i++) {
                sum[i] += t->armyCount[i];
                if (t->playerHasCity[i])
                    sum[i]++;
            }
        }
    }
    //compare results and decide
    highest=npc;
    for (int i = 0; i < 4; i++) {
        if (sum[i] > 0) {
            if (sum[i] == highest) {
                tieHighest = true;
            }
            if (sum[i] > highest) {
                tieHighest = false;
                highest = sum[i];
                answer = i;
            }
        }
    }


    if (highest > 0) {
        if (tieHighest == true)
            return -1; //tie for highest, so no winner.
        else
            return answer;
    } else {
        //no highest.
        return -1;
    }
}
//get all territories.  Required for external map drawing.
list<Territory *> *Map::getTerritories() const {
    return territories;
}
//get map start zone
int Map::getStartZone() {
    return this->startZone;
}
//set start zone after map loaded, before game startys
void Map::setStartZone() {
    if (territories->size()==0) { //no territories, so cannot set.
        startZone=-1;
        return;
    }
    srand(time(0)); //randomize seed
    int guess=-1;
    while (!doesTerritoryExist(guess)) { //keep guessing until we get a valid zone
        guess=rand()%100;
    }
    startZone=guess;
}
//Map stream insertion operator
ostream &operator<<(ostream &stream, const Map &other) {
    return stream << "Map [" << other.territories->size() << " territories]";
}

//Map assignment operator
Map &Map::operator=(const Map &source) {
    cout << "DEBUG: Map Assignment Operator= started" << endl;
    if (this == &source) {
        return *this;
    }
    this->deleteHeapItems(); //delete existing heap items from THIS before copying over
    this->startZone = source.startZone;
    //deep-copy all the territories.
    this->deepCopyAndRelinkTerritories(source);
    return *this;

}

//used when Player needs to see all the territories he/she has an army and/or city and/or startzone. caller must delete from heap.
list<Territory *> *Map::getTerritoriesOfPlayer(int playerid) {
    list<Territory*> *result=new list<Territory*>();
    if (playerid<0 || playerid>3) {
        return result; //returns empty list
    }
    for (Territory* & t: *this->territories) {
        if (t->armyCount[playerid]>0 || t->playerHasCity[playerid]==true || (this->getStartZone()==t->id)) {
            result->push_back(t);
        }
    }
    return result;
}

//return a list of all continent ids.  caller must delete from heap.
list<int>* Map::getAllContinentIDs() {
    list<int>* result=new list<int>();
    int id=-1;
    bool found=false;
    for (Territory* & t: *this->territories) {
        id=t->continentID;
        found=false;
        for (int & i : *result) {
            if (i == id) { found = true; }
        }
        if (!found) { result->push_back(id);}
    }
    return result;
}




vector<string> *Map::generateASCIIMap(const int fromid) const {

    const int BOX_WIDTH=6;
    const int BOX_HEIGHT=3;
    const char WATER=' ';
    const char BORDER='+';

    const MapCell waterCell{fromid==-1?' ':' ',fromid==-1?ANSI_MAP_WATER:ANSI_COLOR_RESET};

    MapCell cells[MAP_WIDTH][MAP_HEIGHT];
    for (int y=0;y<MAP_HEIGHT;y++) {
        for (int x=0;x<MAP_WIDTH;x++) {
            cells[x][y]=waterCell;
        }
    }

    stringstream str;
    string s1,s2,s3;
    string s;

    //DRAW "LANDS" on the map
    int x,y;
    bool show=false;
    bool connectedBySeaMode=false;
    MapEdge edge;

    for (Territory* &t : *territories) {
        //decide if we are showing this territory or not
        connectedBySeaMode=false;
        show=false;
        if (fromid==-1) {
            show=true;
        } else {
            if (t->id==fromid) {
                show=true;
            } else {
                switch(checkPath(t->id,fromid)) {
                    case (NOT_CONNECTED):
                        show=false;
                        break;
                    case(PATH_SEA):
                        connectedBySeaMode=true;
                        show=true;
                        break;
                    case(PATH_LAND):
                        show=true;
                        break;
                    default:
                        show= false;
                };
            }
        }
        if (show==false) {
            continue;
        }
        //build cells of land.
        if (t->x>=0 && t->y>=0) { //ignore the territories with x=-1 or y=-1 (default values)
            //cout << "x=" << t->x << ", y=" << t->y << endl;
            //line 1
            x=t->x;
            y=t->y;
            //line 1
            str.str("");
            str <<"#" << setw(2) << left <<t->id  << "C" << setw(2) << left <<t->continentID;
            s=str.str();
            for (int i=0;i<BOX_WIDTH;i++) {
                cells[x+i][y+0]=MapCell{s[i],connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:ANSI_MAP_LAND};
            }
            s=ANSI_MAP_ID;
            if (t->id==startZone) {
                s=ANSI_MAP_LAND_START_ZONE;
            }
            cells[x+0][y].color=connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:s;
            cells[x+1][y].color=connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:s;
            cells[x+2][y].color=connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:s;
            //line 2 (Player 0)
            str.str(""); //clear the stringstream
            str  <<  "0:" << setw(2)<< right << getArmies(0,t->id)  << setw(2) << (hasCity(0,t->id)?"+C":"  ");
            s=str.str();
            for (int i=0;i<BOX_WIDTH;i++) {
                if (getArmies(0,t->id)>0 || hasCity(0,t->id)) {
                    cells[x+i][y+1]=MapCell{s[i],connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:ANSI_MAP_LAND_PLAYER_OWNS};
                } else {
                    cells[x+i][y+1]=MapCell{s[i],connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:ANSI_MAP_LAND_PLAYER_ABSENT};
                }
            }
            //line 3 (Player 1)
            str.str(""); //clear the stringstream
            str  <<  "1:" << setw(2)<< right << getArmies(1,t->id)  << setw(2) << (hasCity(1,t->id)?"+C":"  ");
            s=str.str();
            for (int i=0;i<BOX_WIDTH;i++) {
                if (getArmies(1,t->id)>0 || hasCity(1,t->id)) {
                    cells[x+i][y+2]=MapCell{s[i],connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:ANSI_MAP_LAND_PLAYER_OWNS};
                } else {
                    cells[x+i][y+2]=MapCell{s[i],connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:ANSI_MAP_LAND_PLAYER_ABSENT};
                }
            }
            //line 4 (NPC)
            str.str(""); //clear the stringstream
            str  <<  "NPC:" << setw(2)<< right << getNPCArmies(t->id);
            s=str.str();
            for (int i=0;i<BOX_WIDTH;i++) {
                if (getNPCArmies(t->id)>0) {
                    cells[x+i][y+3]=MapCell{s[i],connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:ANSI_MAP_LAND_PLAYER_OWNS};
                } else {
                    cells[x+i][y+3]=MapCell{s[i],connectedBySeaMode?ANSI_MAP_LAND_CONNECTED_BY_SEA:ANSI_MAP_LAND_PLAYER_ABSENT};
                }
            }
        }
    }

    //Cells completed.
    //create vector of strings and return it
    vector<string>* lines=new vector<string>();
    for (int y=0;y<MAP_HEIGHT;y++) {
        str.str("");
        for (int x=0;x<MAP_WIDTH;x++) {
            str << cells[x][y].color << cells[x][y].c;
        }
        str << ANSI_COLOR_RESET;
        lines->push_back(str.str());
    }

    return lines; //calling function must delete vector
}

void Map::showMapASCIIonScreen(int from) {
    vector<string>* lines= this->generateASCIIMap(from);
    for (string& s: *lines) {
        cout << s << endl;
    }
    lines->clear();
    delete lines;
    lines= nullptr;

}

void Map::setNPCArmies(int territoryid, int numArmies) {
    Territory *t = this->getTerritory(territoryid);
    if (t == nullptr) {
        cout << "ERROR: Attempt to access invalid territoryid" << territoryid << " in Map::setNPCArmies()" << endl;
        return;
    }
    t->npcArmyCount = numArmies;
}

int Map::getNPCArmies(int territoryid) const {
    Territory *t = this->getTerritory(territoryid);
    if (t == nullptr) {
        cout << "ERROR: Attempt to access invalid territoryid" << territoryid << " in Map::getNPCArmies()" << endl;
        return 0;
    }
    return t->npcArmyCount;
}

bool Map::doesTerritoryExist(int ID) {
    Territory *t = this->getTerritory(ID);
    if (t == nullptr) {
        return false;
    } else {
        return true;
    }
}

//since territoriy id's do NOT need to be continuous, we need way of picking a valid one properly.
int Map::getRandomValidTerritoryID() {
    if (territories->size()==0) {
        cout << "ERROR!  Map::getRandomValudTerritoryID called with no territories loaded!" << endl;
        exit(-1);
    }
    while (true) {
        int guess=rand()%100; //99 is the highest permitted territoryid
        if (doesTerritoryExist(guess)) {
            return guess;
        }
    }
}

int Map::howManyTerritoriesDoesPlayerControl(int playerid) {
    int count{0};
    for (Territory* t : *territories) {
        if (whichPlayerControlsTerritory(t->getID())==playerid) {
            count++;
        }
    }
    return count;
}

int Map::howManyContinentsDoesPlayerControl(int playerid){
    int count{0};
    list<int>* continents = getAllContinentIDs(); //needs to be deleted when done.
    for (int & i : *continents) {
        if (whichPlayerControlsContinent(i)==playerid) {
            count++;
        }
    }
    continents->clear();
    return count;
}

int Map::howManyArmiesDoesPlayerHaveOnMap(int playerid) {
    int count{0};
    for (Territory* t : *territories) {
        count += getArmies(playerid,t->id);
    }
    return count;

}

int Map::getNumberOfContinents() {
    list<int>* continents=getAllContinentIDs();
    int count=continents->size();
    continents->clear();
    return count;
}

int Map::howManyCitiesDoesPlayerHave(int playerid) {
    int count{0};
    for (Territory* t : *territories) {
        if (hasCity(playerid,t->id)) {
            count++;
        }
    }
    return count;
}



