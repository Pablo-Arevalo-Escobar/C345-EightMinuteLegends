//
// Created by Pablo Arevalo on 10/02/2021.
//

#include "Cards.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>



// ENUM TO STRING FUNCTIONS
string enum_to_string(ABILITIES type) {
    switch (type) {
        case NO_ABILITY:
            //return "NO_ABILITY";
            return "no ability";
        case GET_ONE_ARMY:
            //return "GET_ONE_ARMY";
            return "get 1 army";
        case GET_TWO_ARMIES:
            //return "GET_TWO_ARMIES";
            return "get2 armies";
        case GET_THREE_ARMIES:
            //return "GET_THREE_ARMIES";
            return "get 3 armies";
        case GET_FOUR_ARMIES:
            //return "GET_FOUR_ARMIES";
            return "get 4 armies";
        case GET_FIVE_ARMIES:
            //return "GET_FIVE_ARMIES";
            return "get 5 armies";
        case MOVE_ONE_ARMY:
            //return "MOVE_ONE_ARMY";
            return "move 1 army";
        case MOVE_TWO_ARMIES:
            //return "MOVE_TWO_ARMIES";
            return "move 2 armies";
        case MOVE_THREE_ARMIES:
            //return "MOVE_THREE_ARMIES";
            return "move 3 armies";
        case MOVE_FOUR_ARMIES:
            //return "MOVE_FOUR_ARMIES";
            return "move 4 armies";
        case MOVE_FIVE_ARMIES:
            //return "MOVE_FIVE_ARMIES";
            return "move 5 armies";
        case MOVE_SIX_ARMIES:
            //return "MOVE_SIX_ARMIES";
            return "move 6 armies";
        case REMOVE_ONE_ENEMY_ARMY:
            //return "REMOVE_ONE_ENEMY_ARMY";
            return "remove 1 enemy";
        case BUILD_NEW_CITY:
            //return "BUILD_NEW_CITY";
            return "build new city";
        default:
            return "<UNDEFINED ABILITY>";
    }
}

string enum_to_string(GOODS type) {
    switch (type) {
        case NO_GOOD:
            //return "NO_GOOD";
            return "No good";
        case FLIGHT:
            //return "FLIGHT";
            return "Flight";
        case PLUS_1VP_PER_FOREST_CARD:
            //return "PLUS_1VP_PER_FOREST_CARD";
            return "+1VP/Forest";
        case PLUS_1VP_PER_CURSED_CARD:
            //return "PLUS_1VP_PER_CURSED_CARD";
            return "+1VP/Cursed";
        case PLUS_1VP_PER_DIRE_CARD:
            //return "PLUS_1VP_PER_DIRE_CARD";
            return "+1VP/Dire";
        case PLUS_1VP_PER_FLYING:
            //return "PLUS_1VP_PER_FLYING";
            return "+1VP/Flying";
        case PLUS_1VP_PER_ARCANE:
            //return "PLUS_1VP_PER_ARCANE";
            return "+1VP/Arcane";
        case PLUS_1VP_PER_ANCIENT:
            //return "PLUS_1VP_PER_ANCIENT";
            return "+1VP/Ancient";
        case PLUS_1VP_PER_NIGHT_CARD:
            return "+1VP/Night";
        case PLUS_3VP_IF_YOU_HAVE_2_MOUNTAIN_CARDS:
            return "+2VP/2Mountain";
        case PLUS_4VP_FOR_3_NOBLE_CARDS:
            return "+4VP/3Noble";
        case PLUS_1VP_PER_3_COINS:
            //return "PLUS_1VP_PER_3_COINS";
            return "+1VP/3 coins";
        case PLUS_1_ARMY:
            //return "PLUS_1_ARMY";
            return "+1 Army";
        case PLUS_1_MOVE:
            //return "PLUS_1_MOVE";
            return "+1 Move";
        case ONE_ELIXIR:
            //return "ONE_ELIXIR";
            return "1 Elixir";
        case TWO_ELIXIRS:
            //return "TWO_ELIXIRS";
            return "2 Elixirs";
        case THREE_ELIXIRS:
            //return "THREE_ELIXIRS";
            return "3 Elixir";
        case GAIN_TWO_COINS:
            //return "GAIN_TWO_COINS";
            return "+2 Coins";
        case CARD_SET_GAIN_4_VP:
            //return "CARD_SET_GAIN_4_VP";
            return "+4VP/Card Set";
        case IMMUNE_TO_ATTACK:
            //return "IMMUNE_TO_ATTACK";
            return "Immune";
//        case MOVE_ONE_ARMY_GOOD:
//            //return "MOVE_ONE_ARMY_GOOD";
//            return "Move 1 Army good"; //??????
        default:
            return "<UNDEFINED GOOD>";
    }
}

string enum_to_string(CARD_TYPE type) {
    switch (type) {
        case NO_TYPE:
            return "NO_TYPE";
        case MOUNTAIN:
            return "MOUNTAIN";
        case NIGHT:
            return "NIGHT";
        case NOBLE:
            return "NOBLE";
        case FOREST:
            return "FOREST";
        case CURSED:
            return "CURSED";
        case DIRE:
            return "DIRE";
        case ANCIENT:
            return "ANCIENT";
        case ARCANE:
            return "ARCANE";
        default:
            return "<UNDEFINED TYPE>";
    }
}


//**********************
//*      CARD          *
//**********************
//constructor + copy constructor + assignment operator + stream insertion operator
Card::Card() {
    this->title = "NULL";
    this->type = NO_TYPE;
    this->good = NO_GOOD;
    this->good2 = NO_GOOD;
    this->abilityOr = false;
    this->abilityAnd = false;
    this->ability1 = NO_ABILITY;
    this->optional_ability = NO_ABILITY;
}

Card::Card(string title, CARD_TYPE type, GOODS good, GOODS optionalGood, bool abilityOr, bool abilityAnd, ABILITIES ability1, ABILITIES optional_ability) {
    // if either bool is true then the card must have two abilities
    if (((abilityOr || abilityAnd) && optional_ability == NO_ABILITY) || (!(abilityOr || abilityAnd) && optional_ability != NO_ABILITY)) {
        throw logic_error("Error: Card: " + title + " cannot have a bool and no optional ability (or vice versa)");
    }
    this->title = title;
    this->type = type;
    this->good = good;
    this->good2 = optionalGood;
    this->abilityOr = abilityOr;
    this->abilityAnd = abilityAnd;
    this->ability1 = ability1;
    this->optional_ability = optional_ability;
}

//Copy Constructor
Card::Card(const Card &c2) {
    this->title = c2.title;
    this->type = c2.type;
    this->good = c2.good;
    this->good2 = c2.good2;
    this->abilityOr = c2.abilityOr;
    this->abilityAnd = c2.abilityAnd;
    this->ability1 = c2.ability1;
    this->optional_ability = c2.optional_ability;
}

//Assignment Operator
Card &Card::operator=(const Card &source) {
    //Check for self-assignment card1=card1
    if (this == &source) {
        return *this;
    }

    this->title = source.title;
    this->type = source.type;
    this->good = source.good;
    this->good2 = source.good2;
    this->abilityOr = source.abilityOr;
    this->abilityAnd = source.abilityAnd;
    this->ability1 = source.ability1;
    this->optional_ability = source.optional_ability;
    return *this;
}

//Stream Insertion Operator
ostream &operator<<(ostream &stream, const Card &other) {
    if (other.title == "NULL") {
        return stream << "<EMPTY>";
    }
    if (other.abilityAnd || other.abilityOr) {
        return stream << other.title << " -- Good:" << enum_to_string(other.good) << " Ability:" << enum_to_string(other.ability1) << (other.abilityOr ? " OR " : " AND ") << enum_to_string(other.optional_ability);
    } else {
        if (other.good2 != NO_GOOD) {
            return stream << other.title << " -- Good:" << enum_to_string(other.good) << " AND " << enum_to_string(other.good2) << " Ability:" << enum_to_string(other.ability1);
        }
        return stream << other.title << " -- Good:" << enum_to_string(other.good) << " Ability:" << enum_to_string(other.ability1);
    }
}

string Card::toString() {
    return title;
}

bool Card::isEmptyCard() {
    return this->title == "NULL";
}

string Card::getCardType() {
    return enum_to_string(this->type);
}

string Card::getCardGoodDescription() {
    string s=enum_to_string(this->good);
    if (this->good2!=NO_GOOD) {
        s += "/" + enum_to_string(good2);
    }
    return s;
}

string Card::getCardAbilityDescription() {
    string s=enum_to_string(this->ability1);
    if (!(this->optional_ability==NO_ABILITY)) {
        if (this->abilityAnd) {
            s+= " AND ";
        }
        if (this->abilityOr) {
            s+= " OR ";
        }
        s += enum_to_string(this->optional_ability);
    }
    return s;
}

string Card::getTitle() {
    return this->title;
}





//**********************
//*      DECK         *
//**********************
//Default constructor + copy constructor + assignment operator + stream insertion operator + destructor + functions

// Default Constructor
Deck::Deck() {
    this->cards = new vector<Card *>();
    if (SHOW_DEBUG_CARDS) {
        cout << "DEBUG: DECK CONSTRUCTOR CALLED" << endl;
    }

}

// Deep copy constructor
Deck::Deck(const Deck &d2) {
    cout << "DEBUG: DECK COPY CONSTRUCTOR CALLED" << endl;
    this->cards = new vector<Card *>(); // Allocating space on the heap object vector
    vector<Card *> *temp = new vector<Card *>(*d2.cards); // -- Using a temp vector because I can't access d2.cards directly atm
    vector<Card *>::iterator it;
    for (it = temp->begin(); it < temp->end(); it++) {
        this->cards->push_back(new Card(**it));
    }
    temp->clear();
    delete temp;
}

//Assignment operator
Deck &Deck::operator=(const Deck &source) {
    cout << "DEBUG: DECK ASSIGNMENT OPERATOR CALLED" << endl;
    //Check for self-assignment
    if (this == &source) {
        return *this;
    }

    //Deleting existing cards before adding from the source
    for (Card *toDelete: *this->cards) {
        delete toDelete;
    }
    this->cards->clear();

    //Copying Vector<Card*>
    vector<Card *> *temp = new vector<Card *>(*source.cards);; // -- Using a temp vector because I can't access source.cards directly atm
    vector<Card *>::iterator it;
    for (it = temp->begin(); it < temp->end(); it++) {
        this->cards->push_back(new Card(**it));
    }
    temp->clear();
    delete temp;
    return *this;
}

//Stream insertion operator
ostream &operator<<(ostream &stream, const Deck &other) {
    return stream << "Number of cards in deck: " << other.deckSize();
}

//Destructor
Deck::~Deck() {
    if (SHOW_MEMORY_CARDS) {
        cout << "MEMORY: ~Deck() started" << endl;
    }

    int count = 0;  // Debug count - Delete once debug done
    for (Card *toDelete: *cards) {
        delete toDelete;
        count++;  // Debug count - Delete once debug done
    }
    cards->clear();
    delete cards;
    if (SHOW_MEMORY_CARDS) {
        cout << "MEMORY: ~Deck() finished: " << count << " cards deleted from memory on deck destruction." << endl;
    }
}

//Functions
int Deck::deckSize() const {
    return cards->size();
}

//Used to pull a card from the deck to replace a slot in the hand/drawpile
Card *Deck::draw() {
    if (cards->empty()) {
        cout << "Deck is empty" << endl;
        return nullptr;
    } else {
        Card *drawn = cards->back();
        cards->back() = nullptr; // Memory Management: Not deleting pointer heap value as it would cause a memory leak. Heap space would be unallocated for the returned card
        cards->pop_back(); // Remove element from vector
        return drawn; // Make sure that whatever draws this, deletes when done
    }
}

//only used during game start
void Deck::addCard(Card *c) {
    cards->push_back(c);
}

void Deck::shuffle() {
    //Shuffles the deck using the system_clock as a random seed
    std::shuffle(cards->begin(), cards->end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
}

//**********************
//*      HAND         *
//**********************

//Default Constructor
Hand::Hand() {
    array = new Card[6];
    deck = nullptr;
}

//Parametrised Constructor
Hand::Hand(Deck &newDeck) : Hand() {
    Card *temp;
    //fill hand with first 6 cards if they exist
    for (int i = 0; i < 6; i++) {
        temp = newDeck.draw();
        if (temp == nullptr) { //check if there are cards left in deck
            array[i] = Card(); //default constructor is a null card
        } else {
            array[i] = *temp;
            delete temp;
            temp = nullptr;
        }
    }
    this->deck = &newDeck;
}

//Deep copy constructor
Hand::Hand(const Hand &h2) : Hand() {

    //Deep copy -- array
    for (int i = 0; i < 6; i++) {
        this->array[i] = Card(h2.array[i]); // Copying value not pointer
    }

    //Deep copy -- Deck
    this->deck = new Deck(*h2.deck); // call deck copy constructor
}

//Destructor
Hand::~Hand() {
    if (SHOW_MEMORY_CARDS) {
        cout << "MEMORY: Hand destructor started." << endl;
    }
    delete[] array; // delete[] deallocates the memory block assigned to the array
    array = nullptr; // Preventing dangling pointers
    if (SHOW_MEMORY_CARDS) {
        cout << "MEMORY: Hand destructor finished." << endl;
    }
}

//Assignment operator
Hand &Hand::operator=(const Hand &source) {
    cout << "DEBUG: HAND ASSIGNMENT OPERATOR CALLED" << endl;
    //Check for self-assignment
    if (this == &source) {
        return *this;
    }

    //Deep copy -- array
    //overwrite the cards in memory (Cards don't use heap)
    for (int i = 0; i < 6; i++) {
        this->array[i] = Card(source.array[i]); // Copying value not pointer
    }

    //Deep copy -- Deck
    //delete existing deck from heap
    delete this->deck;
    //create copy in heap of deck
    this->deck = new Deck(*source.deck);

    return *this;
}

//Stream insertion operator
ostream &operator<<(ostream &stream, const Hand &other) {
    return stream << "The current hand is: \n" << "---------------------\n" << "Position 0 - Cost 0: " << other.array[0]
                  << "\nPosition 1 - Cost 1: " << other.array[1] << "\nPosition 2 - Cost 1: " << other.array[2]
                  << "\nPosition 3 - Cost 2: " << other.array[3] << "\nPosition 4 - Cost 2: " << other.array[4]
                  << "\nPosition 5 - Cost 3: " << other.array[5] << "\n--------------------\n";
}






//  ****Functions****

// Helper function for exchange method: moves the cards in the Hand located after the players taken card to the left and adds a new card
// This is in the rules -- Done so that the cards in the hand previously cost less for the next round
Card *Hand::replaceTaken(Card *array1, int position) {
    int i = 5 - position;
    if (i != 0) {
        for (int j = 0; j < i; j++) {
            array1[position + j] = array1[position + j + 1];
        }
    }
    Card *temp = deck->draw();
    if (temp == nullptr) {
        array1[5] = Card();
    } else {
        array1[5] = *temp; //Puts drawn card in the array
        delete temp;
    }
    return array1;
}

//Exchange method used in GameEngine to pick up a card from the hand and replace it with a card from the deck
Card *Hand::exchange(int position) {
    if (position < 0 || position > 5) {
        return nullptr;
    }
    if (array[position].isEmptyCard()) {
        return nullptr;
    }
    Card *purchasedCard = new Card(array[position]);
    array = replaceTaken(array, position);
    return purchasedCard;
}

//Exchange method (used in tester) ould either take a position and return the card or Show the player the Hand and then exchange
Card *Hand::exchange() {
    //Getting Player-Input for their chosen card
    cout << *this;

    cout << "Please enter the position number of the desired card" << endl;
    int position = -1;
    cin >> position;

    //Exception Handling: Checking if card position is valid
    if (position < 0 || position > 5 || !cin.good()) {
        cout << "Invalid selection" << endl;
        return nullptr;
    }
    if (array[position].isEmptyCard()) {
        cout << "Invalid selection" << endl;
        return nullptr;
    }

    //Need to add method to take payment in part 2
    //Replacing the purchased card and returning said purchased card
    Card *purchasedCard = new Card(array[position]); //new heap memory managed in next line.
    array = replaceTaken(array, position); //overwrite the card in the hand with the newly drawn card.  Memory managed.
    return purchasedCard;
}

Card *Hand::getCard(int handPosition) const {
    if (handPosition<0 || handPosition >5) {
        return nullptr;
    } else {
        return &(array[handPosition]);
    }

}

ABILITIES Card::getAbility1(){
    return this->ability1;
}

ABILITIES Card::getOptionalAbility(){
    return this->optional_ability;
}

GOODS Card::getGood(){
    return this->good;
}

GOODS Card::getGood2(){
    return this->good2;
}

bool Card::getAbilityOr(){
    return this->abilityOr;
}

bool Card::getAbilityAnd(){
    return this->abilityAnd;
}






