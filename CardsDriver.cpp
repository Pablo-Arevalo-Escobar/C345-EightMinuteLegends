//
// Created by Pablo Arevalo on 10/02/2021.
//

#include "CardsDriver.h"
#include "Cards.h"
#include "Player.h"
#include <iostream>

using std::cout;
using std::endl;

static Deck *mainDeck;

void fillDeck() {
    mainDeck = new Deck();
    //string title, CARD_TYPE type,GOODS good, GOODS good2, bool abilityOr, bool abilityAnd, ABILITIES ability1, ABILITIES optional_ability


    //THESE ARE THE OFFICIAL CARDS
    //2 player cards (27 cards):
    mainDeck->addCard(new Card("Ancient Phoenix", ANCIENT, FLIGHT, NO_GOOD, false, false, MOVE_FIVE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Sage",ANCIENT,PLUS_1VP_PER_ANCIENT,NO_GOOD,false,false,MOVE_THREE_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Tree Spirit", ANCIENT, THREE_ELIXIRS, NO_GOOD, false, false, MOVE_FOUR_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Woods", ANCIENT, PLUS_1_ARMY, NO_GOOD, false, true, BUILD_NEW_CITY, GET_ONE_ARMY));

    mainDeck->addCard(new Card("Cursed Banshee",CURSED,TWO_ELIXIRS,NO_GOOD,false,false,MOVE_SIX_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Cursed Gargoyles",CURSED,FLIGHT,NO_GOOD,false,false,MOVE_FIVE_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Cursed King",CURSED,ONE_ELIXIR,NO_GOOD,true,false,GET_THREE_ARMIES,MOVE_FOUR_ARMIES));
    mainDeck->addCard(new Card("Cursed Mausoleum",CURSED,PLUS_1_MOVE,NO_GOOD,false,false,BUILD_NEW_CITY,NO_ABILITY));
    mainDeck->addCard(new Card("Cursed Tower",CURSED,PLUS_1VP_PER_FLYING,NO_GOOD,false,false,BUILD_NEW_CITY,NO_ABILITY));

    mainDeck->addCard(new Card("Dire Dragon",DIRE,FLIGHT,NO_GOOD,false,true,GET_THREE_ARMIES,REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Dire Eye",DIRE,FLIGHT,NO_GOOD,false,false,GET_FOUR_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Dire Giant",DIRE,IMMUNE_TO_ATTACK,NO_GOOD,false,true,GET_THREE_ARMIES,REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Dire Goblin",DIRE,ONE_ELIXIR,NO_GOOD,false,false,MOVE_FIVE_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Dire Ogre",DIRE,PLUS_1VP_PER_3_COINS,NO_GOOD,false,false,MOVE_TWO_ARMIES,NO_ABILITY));

    mainDeck->addCard(new Card("Forest Elf",FOREST,PLUS_1_ARMY,NO_GOOD,true,false,GET_THREE_ARMIES,MOVE_TWO_ARMIES));
    mainDeck->addCard(new Card("Forest Gnome",FOREST,THREE_ELIXIRS,NO_GOOD,false,false,MOVE_TWO_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Forest Pixie",FOREST,PLUS_1_ARMY,NO_GOOD,false,false,MOVE_FOUR_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Forest Tree Town",FOREST,PLUS_1_MOVE,NO_GOOD,false,false,BUILD_NEW_CITY,NO_ABILITY));

    mainDeck->addCard(new Card("Graveyard",NO_TYPE,PLUS_1VP_PER_CURSED_CARD,NO_GOOD,false,false,GET_TWO_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Lake",NO_TYPE,PLUS_1VP_PER_FOREST_CARD,NO_GOOD,true,false,GET_TWO_ARMIES,MOVE_THREE_ARMIES));

    mainDeck->addCard(new Card("Night Hydra",NIGHT,PLUS_1_ARMY,NO_GOOD,false,true,MOVE_FIVE_ARMIES,REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Night Village",NIGHT,PLUS_1_ARMY,NO_GOOD,false,false,BUILD_NEW_CITY,NO_ABILITY));
    mainDeck->addCard(new Card("Night Wizard",NIGHT,PLUS_1VP_PER_NIGHT_CARD,NO_GOOD,false,true,GET_THREE_ARMIES,REMOVE_ONE_ENEMY_ARMY));

    mainDeck->addCard(new Card("Noble Hills",NO_TYPE,PLUS_4VP_FOR_3_NOBLE_CARDS,NO_GOOD,false,false,GET_THREE_ARMIES,NO_ABILITY));
    mainDeck->addCard(new Card("Noble Knight",NOBLE,PLUS_1_MOVE,NO_GOOD,false,true,GET_FOUR_ARMIES,REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Noble Unicorn",NOBLE,PLUS_1_MOVE,NO_GOOD,false,true,MOVE_FOUR_ARMIES,GET_ONE_ARMY));

    mainDeck->addCard(new Card("Stronghold",NO_TYPE,PLUS_1VP_PER_FLYING,NO_GOOD,false,false,BUILD_NEW_CITY,NO_ABILITY));
    //end of 2 player cards



    //additional 3 player cards (32 cards in total)
    mainDeck->addCard(new Card("Arcane Sphinx", ARCANE, FLIGHT, NO_GOOD, true, false, GET_THREE_ARMIES, MOVE_FOUR_ARMIES));
    mainDeck->addCard(new Card("Arcane Manticore", ARCANE, PLUS_1_MOVE, NO_GOOD, false, true, GET_FOUR_ARMIES, REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Arcane Temple", ARCANE, PLUS_1VP_PER_ARCANE, NO_GOOD, false, false, MOVE_THREE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Mountain Dwarf",MOUNTAIN,PLUS_3VP_IF_YOU_HAVE_2_MOUNTAIN_CARDS,NO_GOOD,false,true,GET_TWO_ARMIES,REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Mountain Treasury",MOUNTAIN,ONE_ELIXIR,GAIN_TWO_COINS,false,false,MOVE_THREE_ARMIES,NO_ABILITY));
    // End of additional 3 player cards

    //additional 4 player cards (34 cards in total)
    mainDeck->addCard(new Card("Castle",NO_TYPE,ONE_ELIXIR,NO_GOOD,true,false,GET_THREE_ARMIES,BUILD_NEW_CITY));
    mainDeck->addCard(new Card("Castle",NO_TYPE,ONE_ELIXIR,NO_GOOD,false,true,MOVE_THREE_ARMIES,BUILD_NEW_CITY));
    //End of additional 4 player cards

























    mainDeck->shuffle();
    return;

}


void deepCopyTestCards() {
    //Deep Copy Test
    //Initializing
    cout << "Testing cards....\n";
    cout << "Loading deck...\n";
    fillDeck();
    cout << "mainDeck size:" << mainDeck->deckSize() << "\n\n";

    //Creating deep copy, drawing (and therefore removing) all cards from the deck
    cout << "Creating deep copy....\n";
    Deck *copyDeck = new Deck(*mainDeck);
    int j = copyDeck->deckSize();
    cout << "deepCopy size: " << copyDeck->deckSize() << "\n\n";


    //Output - Deep Copy
    cout << "DRAWING (AND THEREFORE DELETING) CARDS IN DEEP COPY DECK\n";
    for (int i = 0; i < j; i++) {
        Card *ptr = copyDeck->draw();
        cout << *ptr << endl;
        delete ptr;
    }
    cout << "\ndeepCopy size: " << copyDeck->deckSize() << "\n";
    cout << "mainDeck size:" << mainDeck->deckSize() << "\n";


    //Output for mainDeck to show that deepcopy does not point to the same Cards as mainDeck
    cout << "\nPRINTING CARDS IN MAIN DECK\n";
    for (int i = 0; i < j; i++) {
        cout << *mainDeck->draw() << endl;
    }
}


void demoDriver() {
    cout << "Testing cards....\n";
    cout << "Loading deck...\n";
    fillDeck(); //creates deck on heap. Needs to be deleted.\
    cout << "mainDeck size:" << mainDeck->deckSize() << "\n\n";
    Hand mainHand = Hand(*mainDeck);  //local on stack
    Player tester = Player(); //local on stack
    Card *test;
    for (int i = 0; i < 7; i++) {
        test = mainHand.exchange();
        cout << "\n" << mainHand;
        if (test != nullptr) {
            cout << "Exchanged card is: " << *test << endl;
            tester.AddCard(test);
            cout << tester << endl;
        } else {
            cout << "You selected nothing" << endl;
        }
    }

    //clean up memory  (satisfy 4.4.3 error from ass1)
    delete mainDeck;
    mainDeck= nullptr;

}


void testCards() {
    //deepCopyTestCards();
    demoDriver();
}



