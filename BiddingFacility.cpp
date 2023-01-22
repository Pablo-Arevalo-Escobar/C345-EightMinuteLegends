//
// Created by Jay on 2021-02-11.
//

#include <string>
#include <iostream>
#include <vector>
#include "BiddingFacility.h"
using namespace std;

// default constructor
BiddingFacility::BiddingFacility() {
    this->playerId = -1;
    this->playerName = "";
    this->availableCoins = 0;
    this->bid = 0;
}

// constructor
BiddingFacility::BiddingFacility(int playerId, string playerName, int availableCoins) {
    this->playerId = playerId;
    this->playerName = playerName;
    this->availableCoins = availableCoins;
    this->bid = 0;
    //cout << "BiddingFacility creatwed with coins=" << availableCoins << endl;
}

// copy constructor
BiddingFacility::BiddingFacility(const BiddingFacility &bf) {
    this->playerId = bf.playerId;
    this->playerName = bf.playerName;
    this->availableCoins = bf.availableCoins;
    this->bid = bf.bid;
}

// assignment operator
BiddingFacility &BiddingFacility::operator=(const BiddingFacility &bf) {
    if (this == &bf) return *this;
    this->playerId = bf.playerId;
    this->playerName = bf.playerName;
    this->availableCoins = bf.availableCoins;
    this->bid = bf.bid;
    return *this;
}

// destructor
BiddingFacility::~BiddingFacility() {
    if (SHOW_MEMORY_BIDDING) {
        cout << "MEMORY: BiddingFacility destructor started."<< endl;
    }
    playerId = 0;
    playerName = "";
    availableCoins = 0;
    bid = 0;
    if (SHOW_MEMORY_BIDDING) {
        cout << "MEMORY: BiddingFacility destructor finished"<< endl;
    }
}
//Allow player to attempt to place bid.  If invalid, bid is 0
void BiddingFacility::placeBid(int bidAmount) {
    if (bidAmount < 0 || bidAmount > this->getCoins()) {
        cout << "Invalid bid amount. Your bid was set to 0." << endl;
        bidAmount = 0;
    }
    bid = bidAmount;
}

int BiddingFacility::getPlayerId() {
    return this->playerId;
}

string BiddingFacility::getPlayerName() {
    return this->playerName;
}

int BiddingFacility::getCoins() {
    return this->availableCoins;
}

int BiddingFacility::getBid() {
    return this->bid;
}

void BiddingFacility::setCoins(int numCoins) {
    this->availableCoins = numCoins;
}

// determines the winning bidder (playerId)
int BiddingFacility::determineWinningBidder(vector<BiddingFacility*> bids) {
    int winningPlayerId = -1;
    int highestBid = 0;
    string highestBidder = "";
    for (auto *bid : bids) {
        cout << bid->getPlayerName() << " (Player ID: " << bid->getPlayerId() << ") bid " << bid->getBid() << " coins." << endl;
        if (winningPlayerId < 0 || bid->getBid() > highestBid || (bid->getBid() == highestBid && bid->getPlayerName().compare(highestBidder) < 0)) {
            winningPlayerId = bid->getPlayerId();
            highestBid = bid->getBid();
            highestBidder = bid->getPlayerName();
        }
    }
    cout << highestBidder << " (Player ID: " << winningPlayerId << ") has the winning bid with " << highestBid << " coins and will start first." << endl << endl;
    return winningPlayerId;
}

// stream insertion operator
ostream &operator<<(ostream &stream, const BiddingFacility &other) {
    return stream << "Player ID: " << other.playerId << ", Name: " << other.playerName << ", Available Coins: " << other.availableCoins << ", Bid: " << other.bid;
}
