//
// Created by Pablo Arevalo on 08/02/2021.
//

#pragma once

#include <string>
#include <vector>
#include <iostream>

#define SHOW_MEMORY_CARDS false
#define SHOW_DEBUG_CARDS false

using namespace std;

//Each has at least one
enum ABILITIES {
    NO_ABILITY,
    GET_ONE_ARMY,
    GET_TWO_ARMIES,
    GET_THREE_ARMIES,
    GET_FOUR_ARMIES,
    GET_FIVE_ARMIES,
    MOVE_ONE_ARMY,
    MOVE_TWO_ARMIES,
    MOVE_THREE_ARMIES,
    MOVE_FOUR_ARMIES,
    MOVE_FIVE_ARMIES,
    MOVE_SIX_ARMIES,
    REMOVE_ONE_ENEMY_ARMY,
    BUILD_NEW_CITY
};

//For certain counting options
enum CARD_TYPE {
    NO_TYPE,
    MOUNTAIN,
    NIGHT,
    NOBLE,
    FOREST,
    CURSED,
    DIRE,
    ANCIENT,
    ARCANE
};

//Each card has up to 2
enum GOODS {
    NO_GOOD,
    FLIGHT,
    PLUS_1VP_PER_FOREST_CARD, //+1vp/Forest
    PLUS_1VP_PER_CURSED_CARD, //+1vp/Cursed
    PLUS_1VP_PER_DIRE_CARD,
    PLUS_1VP_PER_FLYING,
    PLUS_1VP_PER_ARCANE,
    PLUS_1VP_PER_ANCIENT,
    PLUS_1VP_PER_NIGHT_CARD,
    PLUS_3VP_IF_YOU_HAVE_2_MOUNTAIN_CARDS,
    PLUS_4VP_FOR_3_NOBLE_CARDS,
    PLUS_1VP_PER_3_COINS,
    PLUS_1_ARMY,
    PLUS_1_MOVE,
    ONE_ELIXIR,
    TWO_ELIXIRS,
    THREE_ELIXIRS,
    GAIN_TWO_COINS,
    CARD_SET_GAIN_4_VP,
    IMMUNE_TO_ATTACK,
};

//Functions to convert enum to string as there are no inbuilt functions for such a process
string enum_to_string(ABILITIES type);
string enum_to_string(GOODS type);
string enum_to_string(CARD_TYPE type);


class Card {
    friend ostream &operator<<(ostream &stream, const Card &other); //stream operator
  private:
    ABILITIES ability1;
    ABILITIES optional_ability;
    GOODS good;
    GOODS good2; //optional.  nullptr if not used.
    CARD_TYPE type;
    bool abilityOr; //to check if card has 2 abilities and is an OR
    bool abilityAnd; //to check if card has 2 abilities and is an AND
    string title; //name of card
  public:
    Card(); //default constructor
    Card(string title, CARD_TYPE type, GOODS good, GOODS optionalGood, bool abilityOr, bool abilityAnd, ABILITIES ability1, ABILITIES optional_ability);
    Card(const Card &c2); //copy constrcutor
    Card &operator=(const Card &source); //assignment operator
    ~Card() = default; //default destructor
    string toString(); //return brief card description for advance output
    bool isEmptyCard(); //check if this card is an empty placeholder (used as empty placeholder in hand pile).
    string getCardType(); //forest, dire, etc.... for GameEngine
    string getCardGoodDescription(); //+1vp/forest, etc... For GameEngine
    string getCardAbilityDescription(); //Ability description, for GameEngine
    string getTitle();
    ABILITIES getAbility1();
    ABILITIES getOptionalAbility();
    GOODS getGood();
    GOODS getGood2();
    bool getAbilityOr();
    bool getAbilityAnd();
    friend class GameEngine;
};


class Deck {
    friend ostream &operator<<(ostream &stream, const Deck &other);
  private:
    vector<Card *> *cards;  //heap cards in deck

  public:
    Deck(); //default constrcutor
    Deck(const Deck &d2); //copy constcutor
    Deck &operator=(const Deck &source); //assignment operator
    ~Deck(); //destructor
    int deckSize() const;
    Card *draw(); //put into hand (draw pile)
    void addCard(Card *c); //used during game start
    void shuffle(); //used during game start

};

//Hand is the draw pile, NOT the player's hand.  (Use Player.ownedcards to get this)
class Hand {
    friend ostream &operator<<(ostream &stream, const Hand &other); //stream operator
  private:
    Card *array = new Card[6]; //memeory space in heap for the 6 cards
    Deck *deck = nullptr; //associated deck
    Card *replaceTaken(Card *, int); //helper method for exchange
  public:
    Hand(); //default constructor
    Hand(Deck &deck); //parametrized constructor
    Hand(const Hand &h2); //copy constructor
    Hand &operator=(const Hand &source); //asignment operator
    ~Hand(); //destrcutor
    Card *exchange(); //To prompt player to take a card from the heand (draw pile) (only used for testing).
    Card *exchange(int position); // exchange a card in the hand to the player.  Needed by GameEngine
    Card* getCard(int handPosition) const; //needed by GameEngine to show card on the board

    friend class GameEngine; //only used to give access to tester.
};



