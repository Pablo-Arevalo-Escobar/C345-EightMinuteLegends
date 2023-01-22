//
// Created by Jay on 2021-02-11.
//

#ifndef C345_ASS1_BIDDINGFACILITY_H
#define C345_ASS1_BIDDINGFACILITY_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

#define SHOW_MEMORY_BIDDING false
#define SHOW_DEBUG_BIDDING false

class BiddingFacility {
  private:
    int playerId; //gets copied from the Player object on player instantiation
    string playerName; //gets copied from the Player object on player instantiation
    int availableCoins;  // starting coins
    int bid; // how much the player is bidding

  public:
    BiddingFacility(); // default constructor
    BiddingFacility(int playerId, string playerName, int availableCoins); // constructor
    BiddingFacility(const BiddingFacility &bf); // copy constructor
    BiddingFacility &operator=(const BiddingFacility &bf); // assignment operator
    ~BiddingFacility(); // destructor
    void placeBid(int bidAmount);
    int getPlayerId();
    string getPlayerName();
    int getCoins(); // available coins
    int getBid(); // get player's current bid
    void setCoins(int numCoins); // set available coins
    int determineWinningBidder(vector<BiddingFacility*> bids); // determines who starts first
    friend ostream &operator<<(ostream &stream, const BiddingFacility &other); // stream insertion operator
};

#endif //C345_ASS1_BIDDINGFACILITY_H
