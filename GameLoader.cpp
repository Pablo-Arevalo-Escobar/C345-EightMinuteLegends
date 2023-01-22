//
// Created by Talha Multani on 2021-03-22.
//

#include "GameLoader.h"
#include <iostream>
#include <string>

using namespace std;

  void GameLoader::welcomeBanner() {
    cout << "----------------------------------------------\n";
    cout << "    Welcome to Eight Minute Empire Legends:\n";
    cout << "----------------------------------------------\n";
    cout << "Please complete the setup phase in order to play the game. \n\n" ;
}
GameLoader::GameLoader()=default;
//Destructor

GameLoader::~GameLoader() {
    if (SHOW_MEMORY_GAMEELOADER) {
        cout << "MEMORY: Game Loader Destructor Complete" << endl;
    }


}

//Copy Constructor

GameLoader::GameLoader(const GameLoader &gameLoader) {
    cout << "Copy Constructor Called" << endl;

}
//Overloaded Assignment Operator

GameLoader &GameLoader::operator=(const GameLoader &gameLoader) {
    cout << "Overloaded Assignment Opertator Called" << endl;
    return *this;
}
//insertion stream operator

ostream &operator<<(ostream &stream, GameLoader &gameLoader) {
    return stream << "Game Loader";
}

//gamestart takes a pointer to a GameEngine object as argument and then uses maplaoder functions to request user map choice and to validate the map
//thereafter the player decides how many players will play
//default start zone is set
//depending on the number of players the corresponding number of coins are attributed to each player
//each players name and details are intialized
//armies are set in starting zone
//deck of cards is created using function from cards which takes a deck and number of players as parameters to allocate the appropriate cards
void GameLoader::gameStart(GameEngine* gameEngine) {
    MapLoader* mapLoader=new MapLoader();
    Player* players[4];
    int n ;
    string playername;
    int coins;
    bool samename=false; //catches identical name

    //print welcome banner
    welcomeBanner();
//uses maplaoder functions to request user map choice and to validate the map
    do{
        gameEngine->mainMap= mapLoader->readFile(mapLoader->fileInput());
        if( gameEngine->mainMap==nullptr)
           cout<< "The selected Map File is invalid. Please select a valid Map File from the options below"<<endl;

    }while(gameEngine->mainMap==nullptr);
//the player decides how many players will play

            cout << "How many players will be playing(2, 3 or 4)?" << endl;
            cin >> n;
            cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        while(cin.fail() || n<2 || n>4 ) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            cout << "Please enter a valid number(2, 3 or 4):" << endl;            cin >> n;
        }

        //set start zone
        gameEngine->mainMap->setStartZone();
//depending on the number of players the corresponding number of coins are attributed to each player

     switch(n){
         case 2: coins=12;
            break;

         case 3: coins=11;
           break;

         case 4: coins=9;
           break;
         default:
            cout<<"The number of players is not valid"<<endl;
     }
     gameEngine->setNumberOfPlayers(n);

//each players name and details are intialized

     for(int i =0; i<4;i++){
      if(i<n)
      {
          do {
              samename=false;
              cout << "Please enter the name of Player " << i << endl;
              cin >> playername;
              //check if name is duplicate
              for (int j = 0; j < i; j++) {
                  if (playername.compare(gameEngine->players[j]->getName()) == 0) {
                      samename = true;
                      cout<<"Your name matches with another players name. A different name is required."<<endl;
                      break;
                  }
              }
          }while(samename);
          gameEngine->players[i]=new Player(i, playername, 0, coins, 3, 18,0,0,0,new HumanPlayerStrategy);
          cout<< playername << " has been created with " << coins << " coins, 3 unplaced cities and 18 unplaced armies." << endl;

       }

      else
          gameEngine->players[i]= nullptr;
     }

     //deck of cards is created using function from cards which takes a deck and number of players as parameters to allocate the appropriate cards
    gameEngine->mainDeck=new Deck();
    fillDeck(gameEngine->mainDeck, n);
    gameEngine->hand=new Hand(*gameEngine->mainDeck); //create hand,  make link with deck, and draw first 6 cards


    //clean up memory
    delete mapLoader;
    mapLoader= nullptr;


}
//deck of cards is created taking  a deck and number of players as parameters to allocate the appropriate cards

  void GameLoader::fillDeck(Deck* mainDeck, int n) {
    //string title, CARD_TYPE type,GOODS good, GOODS good2, bool abilityOr, bool abilityAnd, ABILITIES ability1, ABILITIES optional_ability
      //mainDeck = new Deck();


    if (n>=2) {
        //2 player cards (27 cards):
        mainDeck->addCard(
                new Card("Ancient Phoenix", ANCIENT, FLIGHT, NO_GOOD, false, false, MOVE_FIVE_ARMIES, NO_ABILITY));
        mainDeck->addCard(new Card("Ancient Sage", ANCIENT, PLUS_1VP_PER_ANCIENT, NO_GOOD, false, false, MOVE_THREE_ARMIES,
                                   NO_ABILITY));
        mainDeck->addCard(new Card("Ancient Tree Spirit", ANCIENT, THREE_ELIXIRS, NO_GOOD, false, false, MOVE_FOUR_ARMIES,
                                   NO_ABILITY));
        mainDeck->addCard(
                new Card("Ancient Woods", ANCIENT, PLUS_1_ARMY, NO_GOOD, false, true, BUILD_NEW_CITY, GET_ONE_ARMY));

        mainDeck->addCard(
                new Card("Cursed Banshee", CURSED, TWO_ELIXIRS, NO_GOOD, false, false, MOVE_SIX_ARMIES, NO_ABILITY));
        mainDeck->addCard(
                new Card("Cursed Gargoyles", CURSED, FLIGHT, NO_GOOD, false, false, MOVE_FIVE_ARMIES, NO_ABILITY));
        mainDeck->addCard(
                new Card("Cursed King", CURSED, ONE_ELIXIR, NO_GOOD, true, false, GET_THREE_ARMIES, MOVE_FOUR_ARMIES));
        mainDeck->addCard(
                new Card("Cursed Mausoleum", CURSED, PLUS_1_MOVE, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));
        mainDeck->addCard(
                new Card("Cursed Tower", CURSED, PLUS_1VP_PER_FLYING, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));

        mainDeck->addCard(
                new Card("Dire Dragon", DIRE, FLIGHT, NO_GOOD, false, true, GET_THREE_ARMIES, REMOVE_ONE_ENEMY_ARMY));
        mainDeck->addCard(new Card("Dire Eye", DIRE, FLIGHT, NO_GOOD, false, false, GET_FOUR_ARMIES, NO_ABILITY));
        mainDeck->addCard(new Card("Dire Giant", DIRE, IMMUNE_TO_ATTACK, NO_GOOD, false, true, GET_THREE_ARMIES,
                                   REMOVE_ONE_ENEMY_ARMY));
        mainDeck->addCard(new Card("Dire Goblin", DIRE, ONE_ELIXIR, NO_GOOD, false, false, MOVE_FIVE_ARMIES, NO_ABILITY));
        mainDeck->addCard(
                new Card("Dire Ogre", DIRE, PLUS_1VP_PER_3_COINS, NO_GOOD, false, false, MOVE_TWO_ARMIES, NO_ABILITY));

        mainDeck->addCard(
                new Card("Forest Elf", FOREST, PLUS_1_ARMY, NO_GOOD, true, false, GET_THREE_ARMIES, MOVE_TWO_ARMIES));
        mainDeck->addCard(
                new Card("Forest Gnome", FOREST, THREE_ELIXIRS, NO_GOOD, false, false, MOVE_TWO_ARMIES, NO_ABILITY));
        mainDeck->addCard(
                new Card("Forest Pixie", FOREST, PLUS_1_ARMY, NO_GOOD, false, false, MOVE_FOUR_ARMIES, NO_ABILITY));
        mainDeck->addCard(new Card("Forest Tree Town", FOREST, PLUS_1_MOVE, NO_GOOD, false, false, BUILD_NEW_CITY,
                                   NO_ABILITY));

        mainDeck->addCard(new Card("Graveyard", NO_TYPE, PLUS_1VP_PER_CURSED_CARD, NO_GOOD, false, false, GET_TWO_ARMIES,
                                   NO_ABILITY));
        mainDeck->addCard(new Card("Lake", NO_TYPE, PLUS_1VP_PER_FOREST_CARD, NO_GOOD, true, false, GET_TWO_ARMIES,
                                   MOVE_THREE_ARMIES));

        mainDeck->addCard(
                new Card("Night Hydra", NIGHT, PLUS_1_ARMY, NO_GOOD, false, true, MOVE_FIVE_ARMIES, REMOVE_ONE_ENEMY_ARMY));
        mainDeck->addCard(new Card("Night Village", NIGHT, PLUS_1_ARMY, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));
        mainDeck->addCard(new Card("Night Wizard", NIGHT, PLUS_1VP_PER_NIGHT_CARD, NO_GOOD, false, true, GET_THREE_ARMIES,
                                   REMOVE_ONE_ENEMY_ARMY));

        mainDeck->addCard(
                new Card("Noble Hills", NO_TYPE, PLUS_4VP_FOR_3_NOBLE_CARDS, NO_GOOD, false, false, GET_THREE_ARMIES,
                         NO_ABILITY));
        mainDeck->addCard(new Card("Noble Knight", NOBLE, PLUS_1_MOVE, NO_GOOD, false, true, GET_FOUR_ARMIES,
                                   REMOVE_ONE_ENEMY_ARMY));
        mainDeck->addCard(
                new Card("Noble Unicorn", NOBLE, PLUS_1_MOVE, NO_GOOD, false, true, MOVE_FOUR_ARMIES, GET_ONE_ARMY));

        mainDeck->addCard(
                new Card("Stronghold", NO_TYPE, PLUS_1VP_PER_FLYING, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));
        //end of 2 player cards
    }

    if(n>=3)
    { //additional 3 player cards (32 cards in total)
        mainDeck->addCard(new Card("Arcane Sphinx", ARCANE, FLIGHT, NO_GOOD, true, false, GET_THREE_ARMIES, MOVE_FOUR_ARMIES));
        mainDeck->addCard(new Card("Arcane Manticore", ARCANE, PLUS_1_MOVE, NO_GOOD, false, true, GET_FOUR_ARMIES, REMOVE_ONE_ENEMY_ARMY));
        mainDeck->addCard(new Card("Arcane Temple", ARCANE, PLUS_1VP_PER_ARCANE, NO_GOOD, false, false, MOVE_THREE_ARMIES, NO_ABILITY));
        mainDeck->addCard(new Card("Mountain Dwarf",MOUNTAIN,PLUS_3VP_IF_YOU_HAVE_2_MOUNTAIN_CARDS,NO_GOOD,false,true,GET_TWO_ARMIES,REMOVE_ONE_ENEMY_ARMY));
        mainDeck->addCard(new Card("Mountain Treasury",MOUNTAIN,ONE_ELIXIR,GAIN_TWO_COINS,false,false,MOVE_THREE_ARMIES,NO_ABILITY));
        // End of additional 3 player cards

    }

    if(n==4)
    {
        //additional 4 player cards (34 cards in total)
        mainDeck->addCard(new Card("Castle",NO_TYPE,ONE_ELIXIR,NO_GOOD,true,false,GET_THREE_ARMIES,BUILD_NEW_CITY));
        mainDeck->addCard(new Card("Castle",NO_TYPE,ONE_ELIXIR,NO_GOOD,false,true,MOVE_THREE_ARMIES,BUILD_NEW_CITY));
        //End of additional 4 player cards
    }

    mainDeck->shuffle();

    return;

}
