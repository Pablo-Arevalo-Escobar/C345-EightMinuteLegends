//
// Created by Derek James on 2021-03-15.
//

#ifndef C345_ASS2_GAMEENGINE_H
#define C345_ASS2_GAMEENGINE_H
#include <iostream>
#include <iomanip>  //for prettier table-like printing using setw (see showMapLoadResults for example)
#include <string>
#include <sstream>
#include "Cards.h"
#include "Map.h"
#include "Player.h"
#include "GameLoader.h"
#include "MapLoader.h"

#define SHOW_MEMORY_GAMEENGINE false
#define SHOW_DEBUG_GAMEENGINE true
#define END_GAME_AFTER_1_TURN false
const int TEST_DATA_NUMBER_OF_BOTS=2; //legal values are 0,1,2

class Player;

class GameEngine : public Subject{
  public:
    Deck *mainDeck; //left public for future design.  May want to make private with getters/setters once code is more mature.
    Map *mainMap; //left public for future design.  May want to make private with getters/setters once code is more mature.
    Player *players[4];  //pointer to array of Players //left public for future design.  May want to make private with getters/setters once code is more mature.
    Hand *hand; //left public for future design.  May want to make private with getters/setters once code is more mature.
    GameEngine(); //default constructor
    GameEngine(const GameEngine& other); //copy constructor
    GameEngine &operator=(const GameEngine &source); //assignment operator
    ~GameEngine(); //destructor

    //Observer-related methods
    //StatisticsView* getStatisticsView() {return stats;}; //stats is the OBSERVER
    bool isGameOver(); //tell main loop if game is over.
    void setCurrentPlayer(int p); //observered for part2
    void setCurrentCard(Card* c); //observered for part2
    bool refreshStats{false}; //tells observers WHY notify was called
    bool refreshPhase{false}; //tells observers WHY notify was called
    bool refreshPhaseCard{false};
    bool refreshEndGame{false}; //tells observers WHY notify was called

    //if true, prompt shown at each players' turn to switch strategy or not. Set once at the start of the game. Set to false by default
    bool allowStrategyDynamicChange;

    //Other Methods
    void takeTurn(); //take 1 turn for each player
    void processPlayerActions(Card* card, Player* player);// part 4 stuff.  Called after a player has bought the card.
    void createTestObjects(); //test deck, test map, test players, test hand
    void showMapAndHandAndPlayers();//displays all elements of the tabletop
    string getCardTitleAndTypeAndGoodInColor(Card* c,int width=0) const; //for displaying in color in live game
    void setNumberOfPlayers(int p) {numberOfPlayers=p;} //sets the number of players
    int getNumberOfPlayers() const {return numberOfPlayers;} //gets the number of players
    void processPossibleAction(ABILITIES ability, Player *player); //process all card abilities when user chooses card from hand
    void getArmiesHelper(int numOfArmies, Player* player); //helper method to process all abilities that sets armies
    void moveArmiesHelper(int numOfArmies, Player* player, ABILITIES ability); //helper method to process all abilities that moves armies
    void processBotPossibleAction(ABILITIES ability, Player *player);
    void getBotArmiesHelper(int numOfArmies, Player* player);
    void moveBotArmiesHelper(int numOfArmies, Player* player, ABILITIES ability);
    void processCardGood(GOODS goods, Player* player); //part of step 4 to process drawn card.
    void setStartingPlayer(int i) {startingPlayer=i;}// called after bidding process
    int getCurrentPlayer();
    int botCardCost(int position); //position:  0="left", 5="right"
    Card* buyCardFromHand(Player *p); //routine to prompt user to buy a card from the hand.
    void setAllowStrategyDynamicChange(); //setter of allowStrategyDynamicChange
    bool getAllowStrategyDynamicChange(); //getter of allowStrategyDynamicChange
    Card* getCurrentCard(){return currentCard;}
    void tournamentStart(); //ask user whether or not to start tournament mode
    bool isTournament() {return tournamentMode;} //is gameengine in tournament mode?
    void showTournamentResults(); //as required by assignment
  private:
    int currentPlayer{0}; //who is current?
    int startingPlayer; //who starts?  (set after bidding)
    int numberOfPlayers; //used for loops
    int roundNumber{0}; //track round number (not very useful)
    int cardCost(int position); //position:  0="left", 5="right"
    bool tournamentMode{false}; //self-explanatory
    Card* currentCard;  //for ass3, which card is being played


};

void part3Demo(); //for demo driver
void part4Demo(); //for demo driver
void part5Demo(); //for demo driver


#endif //C345_ASS2_GAMEENGINE_H
