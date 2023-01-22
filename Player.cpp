//
// Created by Kevin Li Zhong Hao on 2021-02-06.
//

#include <string>
#include <iostream>
#include <list>
#include "Cards.h"
#include "Player.h"
#include "BiddingFacility.h"
#include "GameEngine.h"
#include "PlayerStrategies.h"

using namespace std;

extern GameEngine *mainGame;

//parameterized constructor
Player::Player(int id, string name, int turn, int coins, int unplacedCity, int unplacedArmy, int numOfFlight, int numOfExtraMovePerTurn, int numOfExtraPlacementPerTurn, PlayerStrategy *initStrategy) {
    this->id = id;
    this->name = name;
    this->turn = turn;
    this->coins = coins;
    this->unplacedCity = unplacedCity;
    this->unplacedArmy = unplacedArmy;
    this->numOfFlight = numOfFlight;
    this->numOfExtraMovePerTurn = numOfExtraMovePerTurn;
    this->numOfExtraPlacementPerTurn = numOfExtraPlacementPerTurn;
    this->immunity = false;
    this->strategy = initStrategy;
    this->ownedCards = new vector<Card *>; //create an empty vector of card pointers
    this->biddingFacility = new BiddingFacility(id, name, coins); //create a new bidding facility
    this->victoryPoints=0;
}

//default constructor
Player::Player() {
    this->id = -1;
    this->name = "";
    this->turn = 0;
    this->coins = 0;
    this->unplacedCity = 0;
    this->unplacedArmy = 0;
    this->numOfFlight = 0;
    this->numOfExtraMovePerTurn = 0;
    this->numOfExtraPlacementPerTurn = 0;
    this->immunity = false;
    this->strategy = NULL;
    this->ownedCards = new vector<Card *>(); //create an empty vector of card pointers
    this->biddingFacility = new BiddingFacility(-1, "", 0); //create a new bidding facility
    if (SHOW_DEBUG_PLAYER) {
        cout << "DEBUG: DECK CONSTRUCTOR CALLED" << endl;
    }
    this->victoryPoints=0;
}

//copy constructor
Player::Player(const Player &oldPlayer) {
    this->id = oldPlayer.id;
    this->name = oldPlayer.name;
    this->turn = oldPlayer.turn;
    this->coins = oldPlayer.coins;
    this->unplacedCity = oldPlayer.unplacedCity;
    this->unplacedArmy = oldPlayer.unplacedArmy;
    this->numOfFlight = oldPlayer.numOfFlight;
    this->numOfExtraMovePerTurn = oldPlayer.numOfExtraMovePerTurn;
    this->numOfExtraPlacementPerTurn = oldPlayer.numOfExtraPlacementPerTurn;
    this->immunity = oldPlayer.immunity;
    this->strategy = oldPlayer.strategy;
    this->biddingFacility = new BiddingFacility(*oldPlayer.biddingFacility);
    this->ownedCards = new vector<Card *>(); // create a new vector of pointers
    vector<Card *> *temp = new vector<Card *>(*oldPlayer.ownedCards);
    vector<Card *>::iterator it; //create an iterator to go over the the old vector of card pointers
    for (it = temp->begin(); it < temp->end(); it++) {
        this->ownedCards->push_back(new Card(**it));
    }
    this->victoryPoints=oldPlayer.victoryPoints;
    temp->clear(); //clear the temp pointer
    delete temp; //delete temp pointer
}

//overloaded assignment operator
Player &Player::operator=(const Player &other) {
    if (this != &other) { //check if this player is not equal to the passed one
        //delete existing old cards and bidding facility
        for (Card *&c : *this->ownedCards) {
            delete c;
        }
        this->ownedCards->clear();
        delete this->biddingFacility;

        this->id = other.id;
        this->name = other.name;
        this->turn = other.turn;
        this->coins = other.coins;
        this->unplacedCity = other.unplacedCity;
        this->unplacedArmy = other.unplacedArmy;
        this->numOfFlight = other.numOfFlight;
        this->numOfExtraMovePerTurn = other.numOfExtraMovePerTurn;
        this->numOfExtraPlacementPerTurn = other.numOfExtraPlacementPerTurn;
        this->immunity = other.immunity;
        this->strategy = other.strategy;
        this->biddingFacility = new BiddingFacility(*other.biddingFacility);
        vector<Card *> *temp = new vector<Card *>(*other.ownedCards);
        vector<Card *>::iterator it;
        for (it = temp->begin(); it < temp->end(); it++) {
            this->ownedCards->push_back(new Card(**it));
        }
        this->victoryPoints=other.victoryPoints;
        temp->clear();
        delete temp;
        return *this; //return this new player
    }
    return *this; //just return the original player untouched
}

//overloaded stream insertion operator
ostream &operator<<(ostream &stream, const Player &player) {
    //Note:  I changed this to make Player stream usable in GameEngine
    string shortName=player.name;
    if (shortName.length()>8) {
        shortName.resize(8);
    }
    return stream << player.getPlayerShortString();
}

//destructor
Player::~Player() {
    if (SHOW_MEMORY_PLAYER) {
        cout << "MEMORY: Destructor on Player " << this->name << endl;
    }

    int count = 0;
    for (Card *&c : *this->ownedCards) {
        delete c;
        count++;
    }
    ownedCards->clear();
    delete ownedCards; //free memory of ownedCards
    ownedCards = nullptr;
    delete biddingFacility; //free memory of biddingFacility
    biddingFacility = nullptr;
    if (SHOW_MEMORY_PLAYER) {
        cout << "MEMORY: Player " << this->name << " destroyed with " << count << " cards" << endl;
    }
    delete strategy;
}

//add card to the current player's card stash
void Player::AddCard(Card *card) {
    this->ownedCards->push_back(card);
}

//pay coin to perform action. In this method, check if current player has enough coins to perform action
void Player::PayCoin(int coins) {
    if (this->coins == 0)
        cout << this->name << " has 0 coins left. Action denied." << endl;
    else if (this->coins - coins < 0) {
        cout << "Insufficient funds. Action denied." << endl;
    } else {
        this->coins -= coins;
        cout << this->name << " paid " << coins << " coins and has " << this->coins << " left." << endl;
        refreshStats=true;
        notify();
        refreshStats= false;
    }
}

//place a number of armies in a given region
void Player::PlaceNewArmies(int region, int armies, Map* mainMap) {
    // increase player presence in region using the mainMap global Map object
    if (this->unplacedArmy < armies){
        cout << "Illegal move, not enough armies of player " << this->name << " left. Try again." << endl;
    } else {
        if ((mainMap->hasCity(this->id, region) || region == mainMap->getStartZone()) && this->unplacedArmy >= armies) {
            mainMap->setArmies(this->id, region, mainMap->getArmies(this->id, region) + armies);
            this->unplacedArmy -= armies;
            cout << armies << " new army(s) of " << this->name << " has arrived in region " << region << endl;
            refreshStats=true;
            notify();
            refreshStats=false;
        } else {
            cout << "Illegal move. The region has no city of player " << this->name << ", and is not the default spawning region. Try again." << endl;
        }
    }
}

//move a given number of armies from a start region to a destination
void Player::MoveArmies(int startRegion, int newRegion, int numOfArmies, ABILITIES ability, Map *mainMap) {

    int originalTerritoriesControlled=mainMap->howManyTerritoriesDoesPlayerControl(id); //to see if we should bother with observer notify
    int originalContinentsControlled=mainMap->howManyContinentsDoesPlayerControl(id);//to see if we should bother with observer notify

    int numOfArmiesInStartRegion = mainMap->getArmies(this->id, startRegion);
    int numOfArmiesInEndRegion = mainMap->getArmies(this->id, newRegion);
    if (numOfArmiesInStartRegion >= numOfArmies && !(numOfArmiesInStartRegion <= 0)) { //check if player has enough armies to move
        mainMap->setArmies(this->id, newRegion, numOfArmiesInEndRegion + numOfArmies); //increase player presence at destination
        mainMap->setArmies(this->id, startRegion, numOfArmiesInStartRegion - numOfArmies); //decrease player presence at destination
        cout << "Player " << this->name << " moved " << numOfArmies << " army(ies) from region " << startRegion << " to region " << newRegion << "." << endl;

        //refresh stats only if necessary
        if (originalTerritoriesControlled!=mainMap->howManyTerritoriesDoesPlayerControl(id) ||
                originalContinentsControlled!=mainMap->howManyContinentsDoesPlayerControl(id)) {
            refreshStats=true;
            notify();
            refreshStats=false;
        }

    } else
        cout << "Illegal move." << endl;
}

//let's player build a city in a given region. Must check if the region is default start region or has a city already.
void Player::BuildCity(int region, Map* mainMap) {
    // check if current player has army an in the region, if they don't have a city already built, and if they still have cities left in hand
    // if the previous conditions are satisfied, build a city in the region
    if (mainMap->getArmies(this->id, region) != 0 && this->unplacedCity != 0) {
        mainMap->buildCity(this->id, region);
        setUnplacedCity(this->unplacedCity - 1);
        cout << "The region " << region << " now has a city of player " << this->name << endl;
        refreshStats=true;
        notify();
        refreshStats=false;
    }
    else
        cout << "Illegal action. Try again." << endl;
}

//destroy one army unit of the selected enemy
void Player::DestroyArmy(int region, Player *enemy, Map* mainMap) {
    //check if enemy has army in the region, and if the region has troop of current attacking player

    //check if target is an npc's army
    if (enemy == nullptr){
        int npc = mainMap->getNPCArmies(region);
        if (npc != 0 && mainMap->getArmies(this->id, region) != 0) {
            mainMap->setNPCArmies(region, npc - 1);
            cout << this->name << " has destroyed 1 of NPC army in the region " << region << "." << endl;
            refreshStats=true;
            notify();
            refreshStats=false;
        } else
            cout << "Illegal move. Try again." << endl;
    }
    else { //destroy enemy army
        int enemyArmies = mainMap->getArmies(enemy->id, region);
        if (enemyArmies != 0 && mainMap->getArmies(this->id, region) != 0) {
            mainMap->setArmies(enemy->id, region, enemyArmies - 1); //delete enemy army
            this->ComputeScore(true); //deleting an army may change the this player's score
            enemy->ComputeScore(true); //deleting an army may change the enemy's score
            cout << this->name << " has destroyed 1 of " << enemy->getName() << "'s army in the region " << region << "." << endl;
            refreshStats=true;
            notify();
            refreshStats=false;
        } else
            cout << "Illegal move. Try again." << endl;
    }
}

//move units over land, returns the number of moves left possible
int Player::moveOverLand(int cost, int allowedMoves, int startRegion, int finalRegion, int numOfArmiesToPlace, ABILITIES ability, Map* mainMap){
    if (allowedMoves >= numOfArmiesToPlace*cost){
        this->MoveArmies(startRegion, finalRegion, numOfArmiesToPlace, ability, mainMap);
    }
    return allowedMoves - numOfArmiesToPlace*cost;
}

//move units over water, returns the number of moves left possible
int Player::moveOverWater(int cost, int allowedMoves, int startRegion, int finalRegion, int numOfArmiesToPlace, ABILITIES ability, Map* mainMap){
    //for reach wing, cost-1
    cost -= this->numOfFlight;
    if (allowedMoves >= numOfArmiesToPlace*cost){
        if (this->numOfFlight != 0)
            cout << "Since you have " << this->numOfFlight << " wings, your cost to cross the water is reduced to " << cost << endl;
        this->MoveArmies(startRegion, finalRegion, numOfArmiesToPlace, ability, mainMap);
    }
    return allowedMoves - numOfArmiesToPlace*cost;
}

//getters and setters
void Player::setCoins(int coins) {
    this->coins = coins;
    this->biddingFacility->setCoins(coins);
}

int Player::getCoins() {
    return this->coins;
}

void Player::setId(int id) {
    this->id = id;
}

int Player::getId() {
    return this->id;
}

void Player::setName(string name) {
    this->name = name;
}

string Player::getName() {
    return this->name;
}

void Player::setUnplacedCity(int nbr) {
    this->unplacedCity = nbr;
}

int Player::getUnplacedCity() {
    return this->unplacedCity;
}

void Player::setUnplacedArmy(int nbr) {
    this->unplacedArmy = nbr;
}

int Player::getUnplacedArmy() {
    return this->unplacedArmy;
}

void Player::setNumOfFlight(int nbr) {
    if (nbr > 3){
        cout << "You already have the max amount of wings." << endl;
        return;
    }
    else
        this->numOfFlight = nbr;
}

int Player::getNumOfFlight() {
    return this->numOfFlight;
}

void Player::setNumOfExtraMovePerTurn(int nbr) {
    this->numOfExtraMovePerTurn = nbr;
}

int Player::getNumOfExtraMovePerTurn() {
    return this->numOfExtraMovePerTurn;
}

void Player::setNumOfExtraPlacementPerTurn(int nbr) {
    this->numOfExtraPlacementPerTurn = nbr;
}

int Player::getNumOfExtraPlacementPerTurn() {
    return this->numOfExtraPlacementPerTurn;
}

void Player::setImmunity(bool immunityStatus){
    this->immunity = immunityStatus;
}

bool Player::getImmunity(){
    return this->immunity;
}

void Player::setPlayerStrategy(PlayerStrategy *newStrategy){
    if (strategy!= nullptr) {
        delete this->strategy;
    }
    this->strategy = newStrategy;
}

void Player::executePlayerStrategy(){
    this->strategy->execute();
};

BiddingFacility *Player::getBiddingFacility() {
    return this->biddingFacility;
}

//obtain information regarding which regions and continents are owned by the current player
string Player::getOwnedStats() {
    string regionsInfo = "";
    string continentsInfo = "";
    list<Territory *> *allTerritories = mainGame->mainMap->getTerritories();
    //build list of regions
    for (Territory *&t: *allTerritories) {
        if (mainGame->mainMap->whichPlayerControlsTerritory(t->getID()) == this->id) {
            if (regionsInfo != "") { regionsInfo = regionsInfo + ", "; }
            regionsInfo = regionsInfo + to_string(t->getID());
        }
    }
    //do NOT delete allTerritories since it points to the Map's territories object directly
    allTerritories = nullptr;

    //build list of continents
    list<int> *allContinents = mainGame->mainMap->getAllContinentIDs();
    for (int &i: *allContinents) {
        if (mainGame->mainMap->whichPlayerControlsContinent(i) == this->id) {
            if (continentsInfo != "") { continentsInfo = continentsInfo + ", "; }
            continentsInfo = continentsInfo + to_string(i);
        }
    }
    delete allContinents;
    allContinents = nullptr;
    if (regionsInfo == "") { regionsInfo = "None"; }
    if (continentsInfo == "") { continentsInfo = "None"; }

    return "Regions=" + regionsInfo + ", Continents=" + continentsInfo;
}

string Player::getPlayerShortString() const {
    string shortName=name;
    if (shortName.length()>8) {
        shortName.resize(8);
    }
    return  string{"Player-"} + to_string(id) + string{" ["} + shortName + string{"]"};
}

int Player::getNumberOfCards() {
    return this->ownedCards->size();
}
//ComputeScore - Counts the victory points for each player:
int Player::ComputeScore(bool silent){
    if (SHOW_DEBUG_PLAYER) {
        cout << "DEBUG:  COMPUTE SCORE CALLED!!!!" << endl;
    }

    // PART 1: COMPUTING COUNT OF REGIONS AND ISLANDS/CONTINENTS
    int regionCount = 0;
    int contiCount = 0;

    list<Territory *> *allTerritories = mainGame->mainMap->getTerritories();
    //build list of regions
    for (Territory *&t: *allTerritories) {
        if (mainGame->mainMap->whichPlayerControlsTerritory(t->getID()) == this->id) {
            regionCount++;
        }
    }
    //do NOT delete allTerritories since it points to the Map's territories object directly
    allTerritories = nullptr;

    //build list of continents
    list<int> *allContinents = mainGame->mainMap->getAllContinentIDs();
    for (int &i: *allContinents) {
        if (mainGame->mainMap->whichPlayerControlsContinent(i) == this->id) {
            contiCount++;
        }
    }
    delete allContinents;
    allContinents = nullptr;

    // PART 2: COMPUTING COUNT OF GOODS
    // Code stores needed information in arrays then computes VP count
    // Array that stores how many of each type a player has
    // Index 0 is mountain, 1 is night and so on (following the pattern from ENUM type in cards.h)
    int typeArray [9] = { };
    int vpArray [10] = { }; // Array organised as such (0:M 1:Night 2:Noble 3:Forest 4:Cursed, 5: Dire, 6: Ancient 7: Arcane, 8: Flying 9: Coins VP)
    // if vpArray[0] = 1, then VP for mountain cards is true
    vector<Card *>::iterator it;
    for (it = ownedCards->begin(); it < ownedCards->end(); it++) {
        Card temp = **it;

        //storing type
        string type = temp.getCardType();
        if(type.compare("MOUNTAIN") == 0){ typeArray[0] += 1;}
        else if(type.compare("NIGHT") == 0){ typeArray[1] += 1;}
        else if(type.compare("NOBLE") == 0){ typeArray[2] += 1;}
        else if(type.compare("FOREST") == 0){ typeArray[3] += 1;}
        else if(type.compare("CURSED") == 0){ typeArray[4] += 1;}
        else if(type.compare("DIRE") == 0){ typeArray[5] += 1;}
        else if(type.compare("ANCIENT") == 0){ typeArray[6] += 1;}
        else if(type.compare("ARCANE") == 0){ typeArray[7] += 1;}
        if(temp.getCardGoodDescription().compare("Flight") == 0){ typeArray[8] += 1;} // Flight is a good but is treated as a type as it provides VP at the end of the game

        //storing vp goods
        string s = temp.getCardGoodDescription();
        if(s.find("VP") != string::npos){
            if(s.compare(enum_to_string(PLUS_3VP_IF_YOU_HAVE_2_MOUNTAIN_CARDS)) == 0){ vpArray[0] = 1;}
            else if(s.compare(enum_to_string(PLUS_1VP_PER_NIGHT_CARD)) == 0){ vpArray[1] = 1;}
            else if(s.compare(enum_to_string(PLUS_4VP_FOR_3_NOBLE_CARDS)) == 0){ vpArray[2] = 1;}
            else if(s.compare(enum_to_string(PLUS_1VP_PER_FOREST_CARD)) == 0){ vpArray[3] = 1;}
            else if(s.compare(enum_to_string(PLUS_1VP_PER_CURSED_CARD)) == 0){ vpArray[4] = 1;}
            else if(s.compare(enum_to_string(PLUS_1VP_PER_DIRE_CARD)) == 0){ vpArray[5] = 1;}
            else if(s.compare(enum_to_string(PLUS_1VP_PER_ANCIENT)) == 0){ vpArray[6] = 1;}
            else if(s.compare(enum_to_string(PLUS_1VP_PER_ARCANE)) == 0){ vpArray[7] = 1;}
            else if(s.compare(enum_to_string(PLUS_1VP_PER_FLYING)) == 0){ vpArray[8] = 1;}
            else if(s.compare(enum_to_string(PLUS_1VP_PER_3_COINS)) == 0){ vpArray[9] = 1;}
        }
    }
    //Computing vp from cards
    int cardVP = 0;
    if(vpArray[0] == 1 && typeArray[0] >=2){ cardVP += 2;}
    if(vpArray[1] == 1){ cardVP += typeArray[1];}
    if(vpArray[2] == 1 && typeArray[2] >=3){ cardVP += 4;}
    if(vpArray[3] == 1){ cardVP += typeArray[3];}
    if(vpArray[4] == 1){ cardVP += typeArray[4];}
    if(vpArray[5] == 1){ cardVP += typeArray[5];}
    if(vpArray[6] == 1){ cardVP += typeArray[6];}
    if(vpArray[7] == 1){ cardVP += typeArray[7];}
    if(vpArray[8] == 1){ cardVP += typeArray[8];}
    if(vpArray[9] == 1){ cardVP += this->getCoins()/3;}

    setVictoryPoints(cardVP); //May trigger Observer

    if (silent==false) {
        cout << *this << " has " << regionCount << " regions."<< endl;
        cout << *this << " has " << contiCount << " continents."<< endl;
        cout << *this << " has " << cardVP << " victory points."<< endl;
    }

    //Returning value
    return (regionCount+contiCount+cardVP);
}

//Add card to owned (triggers observer)
void Player::AddCardToOwned(Card *c) {
    ownedCards->push_back(c);
}

//set victory point (triggers observer)
void Player::setVictoryPoints(int v) {
    if (v!=victoryPoints) {
        victoryPoints=v;

        refreshStats=true;
        refreshPhaseVP = true;
        notify(); //only notify if there is a change.
        refreshStats=false;
        refreshPhaseVP = false;
    }

}

