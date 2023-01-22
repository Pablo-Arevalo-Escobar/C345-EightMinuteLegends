//
// Created by Kevin Li Zhong Hao on 2021-02-06.
//

#ifndef C345_ASS1_PLAYER_H
#define C345_ASS1_PLAYER_H

#include <string>
#include <iostream>
#include <list>
#include "Cards.h"
#include "Map.h"
#include "BiddingFacility.h"
#include "Subject.h"
#include "PlayerStrategies.h"

#define SHOW_MEMORY_PLAYER false
#define SHOW_DEBUG_PLAYER false

using namespace std;

/*A player owns the cubes, disks, and tokens armies and a card*/
class Player : public Subject {

    //stream insertion operator
    friend ostream& operator<<(ostream& stream, const Player& other);

private:
    int id; //player id
    string name; //Player name
    int turn; //the turn of the player
    int coins; //the number of coins the player possesses
    int unplacedCity; //number of cities left in hand that can still be placed on the map
    int unplacedArmy; //number of armies left in hand that can still be placed on the map
    int numOfFlight; //number of wings accumulated from card goods
    int numOfExtraMovePerTurn; //number of extra move of army per turn accumulated from card goods
    int numOfExtraPlacementPerTurn; //number of extra placement of army accumulated from card goods
    bool immunity; //check if player has immunity against attacks
    PlayerStrategy *strategy; //strategy object to set current player to human, greedy computer or moderate computer mode
    int victoryPoints; // number of victory points the user has.
    vector<Card*>* ownedCards; //all the cards the player owns
    BiddingFacility* biddingFacility; //player's bidding facility

public:
    Player(); //default constructor
    Player(int id, string name, int turn, int coins, int unplacedCity, int unplacedArmy, int numOfFlight=0, int numOfExtraMovePerTurn=0, int numOfExtraPlacementPerTurn=0, PlayerStrategy *initStrategy = NULL); //parameterized constructor
    Player(const Player &oldPlayer); //copy constructor
    Player& operator = (const Player& other); //overloaded assignment operator
    ~Player(); //destructor
    void AddCard(Card* card); //add card to the user's stash.  Responsibility of Player to delete card from heap.
    void PlaceNewArmies(int region, int armies, Map *mainMap); //place a selected number of armies in a target region
    void MoveArmies(int startRegion, int newRegion, int numOfArmies, ABILITIES ability, Map *mainMap); //move a selected number of armies from a starting region to a destination region
    void MoveOverLand(); //unsure method not implemented yet
    void BuildCity(int region, Map *mainMap); //build a city in a region
    void DestroyArmy(int region, Player* enemy, Map* mainMap); //destroy the army of an enemy on a given region
    string getOwnedStats(); //returns the regions and continents controlled by the player
    string getPlayerShortString() const; //for displaying in GameEngine.  (overloading << didn't work when using string stream libraries)
    friend class GameEngine;
    int ComputeScore(bool silent=false); //compute final score of the game
    int moveOverLand(int cost, int allowedMoves, int startRegion, int finalRegion, int numOfArmiesToPlace, ABILITIES ability, Map* mainMap); //move armies over land
    int moveOverWater(int cost, int allowedMoves, int startRegion, int finalRegion, int numOfArmiesToPlace, ABILITIES ability, Map* mainMap); //move armies over sea
    void AddCardToOwned(Card*);  //Add the bought card to the player

    //Observable
    void setVictoryPoints(int v); //Triggers Observers if value changed.
    void PayCoin(int coins); //pay coins to perform action (i.e. to buy cards)
    bool refreshPhaseVP{false};
    bool refreshStats{false}; //tells observers WHY notify was called

    //Strategy pattern executer
    void executePlayerStrategy();

    //getters and setters
    void setCoins(int coins);
    int getCoins();
    int getNumberOfCards();
    void setId(int id);
    int getId();
    void setName(string name);
    string getName();
    void setUnplacedCity(int nbr);
    int getUnplacedCity();
    void setUnplacedArmy(int nbr);
    int getUnplacedArmy();
    void setNumOfFlight(int nbr);
    int getNumOfFlight();
    void setNumOfExtraMovePerTurn(int nbr);
    int getNumOfExtraMovePerTurn();
    void setNumOfExtraPlacementPerTurn(int nbr);
    int getNumOfExtraPlacementPerTurn();
    void setImmunity(bool immunityStatus);
    bool getImmunity();
    int getVictoryPoints() {return victoryPoints;}

    void setPlayerStrategy(PlayerStrategy *newStrategy);
    PlayerStrategy* getPlayerStrategy() {return strategy;}
    BiddingFacility* getBiddingFacility();
};

#endif //C345_ASS1_PLAYER_H
