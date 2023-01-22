//
// Created by Derek James on 2021-03-15.
//

#include "GameEngine.h"
#include <map>
#include "Map.h"

extern GameEngine *mainGame; //used only for demo testing

//Default constructor
GameEngine::GameEngine() {
    mainDeck = new Deck();
    mainMap = new Map();
    hand=nullptr; //needs to be created only after the deck has been filled.
    for (int i=0;i<4;i++) {
        players[i]= nullptr; //only created as needed
    }
    allowStrategyDynamicChange = false; //default set to false
}

//Copy constructor
GameEngine::GameEngine(const GameEngine &other) {
    //attributes
    currentPlayer=other.currentPlayer;
    startingPlayer=other.startingPlayer;
    numberOfPlayers=other.numberOfPlayers;
    roundNumber=other.roundNumber;
    allowStrategyDynamicChange = other.allowStrategyDynamicChange;
    //deep copy stuff
    mainMap=new Map(*other.mainMap);
    mainDeck=new Deck(*other.mainDeck);
    hand= nullptr;
    if (other.hand!= nullptr) {
        hand=new Hand(*other.hand);
        delete hand->deck; //remove the extra deck created by Hand copy constructor
        hand->deck=mainDeck; //correctly point the hand to new deck just created.
    }

    for (int i=0;i<4;i++) {
        players[i]=nullptr;
        if (other.players[i]!= nullptr) {
            players[i]=new Player(*other.players[i]);
        }
    }
}


static void ignoreLine() //used to control cin
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Clear the screen. Doesn't work great in non-ansi computers
static void clearScreen() {
    if (mainGame->isTournament() || mainGame->players[mainGame->getCurrentPlayer()]->getPlayerStrategy()->isBot()) {
        return;
    }
    for (int i=0;i<10;i++) {
        cout << "\n\n\n\n\n\n\n\n\n\n";  //traditional pathetic way to clear screen
    }
    cout << ANSI_CLEAR_SCREEN; //works in osx terminal, but not in CLion console.
}

//ask for input and sets bool to allow dynamic strategy change during game
void GameEngine::setAllowStrategyDynamicChange() {
    if (tournamentMode==true) {
        return;
    }
    string changeMode{};
    cout << "Would you like to allow dynamic player mode change? (Y/N)" << endl;
    cin >> changeMode;
    while (cin.fail() || (changeMode != "Y" && changeMode != "y" && changeMode != "n" && changeMode != "N")) {
        cout << "Please enter Y or N" << endl;
        cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin >> changeMode;
    }
    if (changeMode == "Y" || changeMode == "y") {
        cout << "A prompt to change mode will be shown at each player's round..." << endl;
        this->allowStrategyDynamicChange = true;
    }
    else
        cout << "No dynamic strategy change for the duration of the game." << endl;

}

//getter
bool GameEngine::getAllowStrategyDynamicChange(){
    return this->allowStrategyDynamicChange;
}


//Polling by outside loop to decide when game is over based on number of players
bool GameEngine::isGameOver() {
    //how many cards per player until game is over
    int END_2PLAYER=END_GAME_AFTER_1_TURN?1:13;
    int END_3PLAYER=END_GAME_AFTER_1_TURN?1:10;
    int END_4PLAYER=END_GAME_AFTER_1_TURN?1:8;

    if (tournamentMode) { //override for Part 4 of assignment.
        END_2PLAYER=10;
        END_3PLAYER=10;
        END_4PLAYER=10;
    }

    if(this->numberOfPlayers == 2){
        if(players[0]->getNumberOfCards() == END_2PLAYER && players[1]->getNumberOfCards() == END_2PLAYER){
            refreshEndGame=true;
            refreshStats=true;
            notify(); //Notify Observer
            refreshEndGame=false;
            refreshStats=false;
            return true;
        }
        else{return false;}
    }

    else if(this->numberOfPlayers == 3){
        if(players[0]->getNumberOfCards() == END_3PLAYER && players[1]->getNumberOfCards() == END_3PLAYER && players[2]->getNumberOfCards() == END_3PLAYER){
            refreshEndGame=true;
            refreshStats=true;
            notify(); //Notify Observer
            refreshEndGame=false;
            refreshStats=true;
            return true;
        }
        else{return false;}
    }

    else{
        if(players[0]->getNumberOfCards() == END_4PLAYER && players[1]->getNumberOfCards() == END_4PLAYER && players[2]->getNumberOfCards() == END_4PLAYER && players[3]->getNumberOfCards() == END_4PLAYER){
            refreshEndGame=true;
            refreshStats=true;
            notify(); //Notify Observer
            refreshEndGame=false;
            refreshStats=false;
            return true;
        }
        else{return false;}
    }
}

//Takes 1 turn for each active player.  Handles all parts of Part 4 and 5.
void GameEngine::takeTurn() {
    cout << ANSI_COLOR_YELLOW << "Start of round " << ++roundNumber << ANSI_COLOR_RESET << endl;
    char c;
    for (int i = startingPlayer; i <startingPlayer+numberOfPlayers ; i++) {
        clearScreen();
        setCurrentPlayer(i%numberOfPlayers); //triggers step 2 oberver and sets currentCard=nullptr
        showMapAndHandAndPlayers(); //also shows stats

        //Buying card, setting CurrentCard, player-ComputeScore, ProcessPlayer Actions has been moved to PlayerStrategies

        refreshPhase=true;
        notify();
        refreshPhase=false;

        //check if game is set to allow strategy change in game
        if (allowStrategyDynamicChange){
            //Ask at beginning of each turn if player is to keep their strategy
            string changeMode{};
            cout << "Would you like to change this player's mode?" << endl;
            cin >> changeMode;
            while (cin.fail() || (changeMode != "Y" && changeMode != "y" && changeMode != "n" && changeMode != "N")){
                cout << "Please enter Y or N" << endl;
                cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> changeMode;
            }
            //if player changes strategy, offer 3 choices: Human, Greedy Bot, Moderate Bot
            if (changeMode == "Y" || changeMode == "y"){
                cout << "Select mode (1) Human (2) Greedy Bot (3) Moderate Bot:" << endl;
                int mode{};
                cin >> mode;
                while (cin.fail() || mode > 3 || mode < 1){
                    cout << "Please enter 1, 2 or 3" << endl;
                    cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cin >> mode;
                }
                //set strategy depending on input
                switch (mode) {
                    case 1: {
                        players[currentPlayer]->setPlayerStrategy(new HumanPlayerStrategy);
                        break;
                    }
                    case 2: {
                        players[currentPlayer]->setPlayerStrategy(new GreedyComputerStrategy);
                        break;
                    }
                    case 3: {
                        players[currentPlayer]->setPlayerStrategy(new ModerateComputerStrategy);
                        break;
                    }
                    default: {
                        cout << "ERROR: invalid mode" << endl;
                    }
                }
                (players[currentPlayer])->executePlayerStrategy(); //execute set strategy
            }
            else
                (players[currentPlayer])->executePlayerStrategy(); //if no change, directly execute previously set strategy
        } else
            (players[currentPlayer])->executePlayerStrategy(); //if game is set to disallow strategy change in game

    }
}

//Used for demo:  Skip map loader and create a fixed set of objects
void GameEngine::createTestObjects() {
    //test map
    mainMap->addTerritory(new Territory(0, 2,1,1, "Warsong Gulch"));
    mainMap->addTerritory(new Territory(1, 2,7,0, "The Barrens"));
    mainMap->addTerritory(new Territory(2, 1,15,3, "Land O'No'Where"));
    mainMap->addTerritory(new Territory(3, 1,9,5, "Due date extended"));
    mainMap->addTerritory(new Territory(4, 1,6,9, "Duplicate ID test"));
    mainMap->addTerritory(new Territory(5, 1,5,13, "Centurio!"));
    mainMap->addTerritory(new Territory(6, 2,15,7, "Cosmo!"));
    mainMap->addTerritory(new Territory(7, 3,23,5,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(8, 3,29,3,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(9, 3,29,7,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(10, 3,23,9,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(11, 3,25,13,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(12, 3,46,2,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(13, 3,40,3,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(14, 3,38,7,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(15, 3,39,11,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(16, 3,42,15,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(17, 6,51,8,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(18, 6,59,6,  "Whatever-o!"));
    mainMap->addTerritory(new Territory(19, 6,58,12,  "Whatever-o!"));
    mainMap->createPath(0, 1, PATH_LAND);
    mainMap->createPath(1, 2, PATH_SEA);
    mainMap->createPath(2, 3, PATH_LAND);
    mainMap->createPath(3, 4, PATH_SEA);
    mainMap->createPath(4, 5, PATH_LAND);
    mainMap->createPath(4, 6, PATH_LAND);
    mainMap->createPath(4, 7, PATH_LAND);
    mainMap->createPath(7, 8, PATH_LAND);
    mainMap->createPath(7, 9, PATH_LAND);
    mainMap->createPath(8, 9, PATH_LAND);
    mainMap->createPath(7, 10, PATH_LAND);
    mainMap->createPath(10, 9, PATH_LAND);
    mainMap->createPath(10, 11, PATH_LAND);
    mainMap->createPath(9, 14, PATH_SEA);
    mainMap->createPath(14, 13, PATH_LAND);
    mainMap->createPath(14, 15, PATH_LAND);
    mainMap->createPath(13, 12, PATH_LAND);
    mainMap->createPath(15, 16, PATH_LAND);
    mainMap->createPath(14, 17, PATH_SEA);
    mainMap->createPath(18, 17, PATH_SEA);
    mainMap->createPath(19, 17, PATH_SEA);
    mainMap->createPath(18, 19, PATH_SEA);

    //mainMap->setStartZone();
    if (mainMap->validate() == false) {
        cout << "ERROR:  Map does not validate!!" << endl;
    }

    //test cards
    mainDeck->addCard(new Card("Arcane Sphinx", ARCANE, FLIGHT, NO_GOOD, true, false, GET_THREE_ARMIES, MOVE_FOUR_ARMIES));
    mainDeck->addCard(new Card("Arcane Manticore", ARCANE, PLUS_1_MOVE, NO_GOOD, false, true, GET_FOUR_ARMIES, REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Arcane Temple", ARCANE, PLUS_1VP_PER_ARCANE, NO_GOOD, false, false, MOVE_THREE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Phoenix", ANCIENT, FLIGHT, NO_GOOD, false, false, MOVE_FIVE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Tree Spirit", ANCIENT, THREE_ELIXIRS, NO_GOOD, false, false, MOVE_FOUR_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Woods", ANCIENT, PLUS_1_ARMY, NO_GOOD, false, true, BUILD_NEW_CITY, GET_ONE_ARMY));
    mainDeck->addCard(new Card("Ancient Sage", ANCIENT, PLUS_1VP_PER_ANCIENT, NO_GOOD, false, false, MOVE_THREE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Cursed Banshee", CURSED, TWO_ELIXIRS, NO_GOOD, false, false, MOVE_SIX_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Cursed Gargoyles", CURSED, FLIGHT, NO_GOOD, false, false, MOVE_FIVE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Cursed King", CURSED, ONE_ELIXIR, NO_GOOD, true, false, GET_THREE_ARMIES, MOVE_FOUR_ARMIES));
    mainDeck->addCard(new Card("Cursed Mausoleum", CURSED, PLUS_1_MOVE, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));
    mainDeck->addCard(new Card("Cursed Tower", CURSED, PLUS_1VP_PER_FLYING, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));
    mainDeck->addCard(new Card("Dire Dragon", DIRE, FLIGHT, NO_GOOD, false, true, GET_THREE_ARMIES, REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Dire Giant", DIRE, IMMUNE_TO_ATTACK, NO_GOOD, false, true, GET_THREE_ARMIES, REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Dire Eye", DIRE, FLIGHT, NO_GOOD, false, false, GET_FOUR_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("TEST CARD", DIRE, FLIGHT, NO_GOOD, false, false, MOVE_SIX_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("TEST CARD 2", DIRE, PLUS_1_MOVE, PLUS_1_ARMY, false, true, MOVE_SIX_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("Arcane Sphinx", ARCANE, FLIGHT, NO_GOOD, true, false, GET_THREE_ARMIES, MOVE_FOUR_ARMIES));
    mainDeck->addCard(new Card("Arcane Manticore", ARCANE, PLUS_1_MOVE, NO_GOOD, false, true, GET_FOUR_ARMIES, REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Arcane Temple", ARCANE, PLUS_1VP_PER_ARCANE, NO_GOOD, false, false, MOVE_THREE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Phoenix", ANCIENT, FLIGHT, NO_GOOD, false, false, MOVE_FIVE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Tree Spirit", ANCIENT, THREE_ELIXIRS, NO_GOOD, false, false, MOVE_FOUR_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Ancient Woods", ANCIENT, PLUS_1_ARMY, NO_GOOD, false, true, BUILD_NEW_CITY, GET_ONE_ARMY));
    mainDeck->addCard(new Card("Ancient Sage", ANCIENT, PLUS_1VP_PER_ANCIENT, NO_GOOD, false, false, MOVE_THREE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Cursed Banshee", CURSED, TWO_ELIXIRS, NO_GOOD, false, false, MOVE_SIX_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Cursed Gargoyles", CURSED, FLIGHT, NO_GOOD, false, false, MOVE_FIVE_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("Cursed King", CURSED, ONE_ELIXIR, NO_GOOD, true, false, GET_THREE_ARMIES, MOVE_FOUR_ARMIES));
    mainDeck->addCard(new Card("Cursed Mausoleum", CURSED, PLUS_1_MOVE, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));
    mainDeck->addCard(new Card("Cursed Tower", CURSED, PLUS_1VP_PER_FLYING, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));
    mainDeck->addCard(new Card("Dire Dragon", DIRE, FLIGHT, NO_GOOD, false, true, GET_THREE_ARMIES, REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Dire Giant", DIRE, IMMUNE_TO_ATTACK, NO_GOOD, false, true, GET_THREE_ARMIES, REMOVE_ONE_ENEMY_ARMY));
    mainDeck->addCard(new Card("Dire Eye", DIRE, FLIGHT, NO_GOOD, false, false, GET_FOUR_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("TEST CARD", DIRE, FLIGHT, NO_GOOD, false, false, MOVE_SIX_ARMIES, NO_ABILITY));
    mainDeck->addCard(new Card("TEST CARD 2", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 3", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 4", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 5", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 6", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 7", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 8", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 9", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 10", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->addCard(new Card("TEST CARD 11", DIRE, FLIGHT, NO_GOOD, false, true, MOVE_THREE_ARMIES, GET_THREE_ARMIES));
    mainDeck->shuffle();
    //put 6 cards into the hand (Will auto-Draw on constrcutior).
    hand = new Hand(*mainDeck);

    //Test Players
    numberOfPlayers = 2;
    if (TEST_DATA_NUMBER_OF_BOTS>0) {
        players[0] = new Player(0, "Alice Bot", 0, 10, 3, 18, 0, 0, 0, new ModerateComputerStrategy); //create player1
    } else {
        players[0] = new Player(0, "Alice", 0, 10, 3, 18, 0, 0, 0, new HumanPlayerStrategy); //create player1
    }
    if (TEST_DATA_NUMBER_OF_BOTS>1) {
        players[1] = new Player(1, "Bob the bot", 0, 10, 3, 18, 0, 0, 0, new GreedyComputerStrategy()); //create player2
    } else {
        players[1] = new Player(1, "Bob", 0, 10, 3, 18, 0, 0, 0, new HumanPlayerStrategy()); //create player2
    }

    players[2] = nullptr;
    players[3] = nullptr;

    //NPC STUFF
    mainMap->setNPCArmies(0,3);
    mainMap->setNPCArmies(2,5);
    mainMap->setNPCArmies(5,1);
    mainMap->setArmies(0,10,3);
    mainMap->setArmies(1,10,5);
    mainMap->setArmies(1,1,1);
    mainMap->setArmies(0,1,1);
    mainMap->setArmies(0,4,2);

    //mainMap->buildCity(1,5);

    mainMap->buildCity(0,5);
    mainMap->buildCity(0,1);
    mainMap->buildCity(1,1);
    //mainMap->buildCity(0,10);


}
//Show the Entire BOARD.  Calls ASCII map draw and shows cards, hands, player info
void GameEngine::showMapAndHandAndPlayers() {


    //start by showing the stats view

    refreshStats=true;
    notify();
    refreshStats=false;

    using std::stringstream;

    //for IN-GAME-LISTING
    const int BOARD_HEIGHT = 50; //max board height
    string lines[BOARD_HEIGHT];
    const int WIDTH_ID = 2;
    const int WIDTH_NAME = 20;
    const int WIDTH_CONTINENT = 2;
    const int WIDTH_ARMIES = 2;
    const int WIDTH_CITIES = 2;
    const int WIDTH_PLAYER_INFO = 40;
    const int WIDTH_CARD_DESCRIPTION=40;
    const int MAX_CARDS = 13; //total  number of cards the player can have
    const int MIN_MAP_LINES = 22;

    int adj_land = 0;
    int adj_sea = 0;
    bool error = false;
    int counter = 0; //used to count the lines
    string s;
    string color;
    string mapLine;
    stringstream line;

    //call ascii map from heap, copy into local array, and delete heap
    vector<string>* map= mainMap->generateASCIIMap();
    for (int y=0;y<MAP_HEIGHT;y++) {
        lines[y]=(*map)[y];
    }
    map->clear();
    delete map;
    map=nullptr;

    //Show Hand (Draw Pile)
    lines[0] += ANSI_WHITE_UNDERLINE;
    lines[0] += " Draw Pile has " + to_string(mainDeck->deckSize()) +" cards left.";
    lines[0] += ANSI_COLOR_RESET;

    lines[1] += ANSI_WHITE_UNDERLINE;
    lines[1] += " # Cost Card";
    lines[1] += ANSI_COLOR_RESET;
    Card *temp;
    for (int i = 0; i < 6; i++) {
        temp=hand->getCard(i);
        line.str("");
        //build card info for display
        if (temp==nullptr || temp->isEmptyCard()) {
            line << ANSI_COLOR_WHITE << "- empty -";
        } else {
            line << getCardTitleAndTypeAndGoodInColor(temp,WIDTH_CARD_DESCRIPTION);
            //show action when in hand
            line << ANSI_COLOR_GREEN << " " << temp->getCardAbilityDescription();

        }
        line << ANSI_COLOR_RESET;
        //show card info + position + card cost
        lines[2 + i] += string{" "} + to_string(i) + "  " + ANSI_COLOR_YELLOW +  to_string(cardCost(i)) + "   " + line.str();

        line.str("");
    }

    //Player Info

    const int FIRST_PLAYER_LINE = 9;
    for (int i = 0; i < 2; i++) {
        color=(i==currentPlayer)?ANSI_ACTIVE_PLAYER:ANSI_WHITE_UNDERLINE;
        counter = FIRST_PLAYER_LINE;

        line << "  " << color << setw(WIDTH_PLAYER_INFO) <<  left <<  players[i]->getPlayerShortString() << ANSI_COLOR_RESET;
        lines[counter++] += line.str();
        line.str("");
        s=(to_string(players[i]->getCoins()) + string{" coins"} + " ");
        s+=to_string(players[i]->getUnplacedArmy()) + " Armies ";
        s+=to_string(players[i]->getUnplacedCity()) + " Cities";
        line << "  " << ANSI_COLOR_YELLOW << setw(WIDTH_PLAYER_INFO) << left << s << string{ANSI_COLOR_RESET};
        lines[counter++]+=line.str();
        line.str("");

        for (Card *c : *players[i]->ownedCards) {
            line << getCardTitleAndTypeAndGoodInColor(c,WIDTH_PLAYER_INFO) << ANSI_COLOR_RESET;
            lines[counter++] += "  " + line.str();
            line.str("");
        }
        for (int j = counter; j < (FIRST_PLAYER_LINE + MAX_CARDS); j++) {
            line << setw(WIDTH_PLAYER_INFO) << " ";
            lines[counter++] += "  " + line.str();
            line.str("");
        }
    }
    //now show them
    for (int i = 0; i <= (counter + 1); i++) {
        cout << lines[i] << endl;
    }
}
//helper function for buying / listing hand
int GameEngine::cardCost(int position) {
    switch (position) {
        case 0: //"Left"
            return 0;
        case 1:
        case 2:
            return 1;
        case 3:
        case 4:
            return 2;
        case 5:
            return 3;
        default:
            if (SHOW_DEBUG_GAMEENGINE) {
                cout << "ERROR:  GameEngine::cardCost called with invalid position " << position << endl;
            }
            return 0;
    }
}

//Forces player to buy a card from the hand.
Card* GameEngine::buyCardFromHand(Player *p) {
    int choice=-1;
    bool validChoice=false;
    Card* card;
    while (!validChoice) {
        cout << "Choose a card # to buy: ";
        cin >> choice;
        ignoreLine();
        if (cin.fail()) {
            cout << ANSI_COLOR_RED << "Invalid:" << ANSI_COLOR_RESET << " Unrecognized answer." << endl;
            cin.clear();
            ignoreLine();
            continue;
        }
        cin.clear();
        if (choice<0 || choice > 5 ) {
            cout << ANSI_COLOR_RED << "Invalid:" << ANSI_COLOR_RESET << " Choose a number between 0 and 5" << endl;
            continue;
        }
        //does the user have enough coins?
        if (cardCost(choice) > p->getCoins()) {
            cout << ANSI_COLOR_RED << "Invalid:" << ANSI_COLOR_RESET << " Not enough coins!" << endl;
            continue;
        }
        //looks valid.
        card=hand->exchange(choice);
        if (card== nullptr || card->isEmptyCard()) {
            cout << ANSI_COLOR_RED << "Invalid:" << ANSI_COLOR_RESET << " No card in that position" << endl;
            continue;
        }
        //p->ownedCards->push_back(card); //move the card from the hand into the player's owned card collection.
        p->AddCardToOwned(card); //triggers the Observer
        p->setCoins(p->getCoins()-cardCost(choice)); //pay for the card.
        validChoice=true; //break the loop


        cout << "You purchased " << ANSI_COLOR_PURPLE << card->title << ANSI_COLOR_RESET << " for " << ANSI_COLOR_YELLOW << cardCost(choice) << " coins." << ANSI_COLOR_RESET<< endl;
    }
    return card;
}

//When player draws a card, process only the stackable ACTIVE goods/effect of the card.
void GameEngine::processCardGood(GOODS goods, Player* player){
    switch(goods){
        case FLIGHT:    //for every wing, this decreases cost of moving over sea by 1
            player->setNumOfFlight(player->getNumOfFlight() + 1);
            break;
        case PLUS_1_ARMY: //everytime the player draws a card with setting armies as its main ability, allow to set one extra army that turn. STACKABLE
            player->setNumOfExtraPlacementPerTurn(player->getNumOfExtraPlacementPerTurn() + 1);
            break;
        case PLUS_1_MOVE: //everytime the player draws a card with moving armies as its main ability, allow to move one extra army that turn. STACKABLE
            player->setNumOfExtraMovePerTurn(player->getNumOfExtraMovePerTurn() + 1);
            break;
        case GAIN_TWO_COINS: //immediately gain 2 coins
            player->setCoins(player->getCoins() + 2);
            break;
        case IMMUNE_TO_ATTACK: //once a player holds a card with immunity, they cannot be attacked by another player for the rest of the game
            player->setImmunity(true);
            break;
        default:
            break;
    }
}

//Process action given by the card chosen by the player. Allows user to choose between OR actions. Allows user to skip turn.
void GameEngine::processPlayerActions(Card *card, Player *player) {
    //at this point, purchased card is in user ownedCards vector
    cout << ANSI_COLOR_GREEN << player->getPlayerShortString() << " processed action for card " << ANSI_COLOR_PURPLE << card->title << ANSI_COLOR_RESET << endl;
    //display card info
    cout << ANSI_COLOR_PURPLE << card->title << ANSI_COLOR_RESET << " allows you to: " << ANSI_COLOR_GREEN << card->getCardAbilityDescription() << ANSI_COLOR_RESET << " and has the goods: " << ANSI_COLOR_CYAN << card->getCardGoodDescription() << ANSI_COLOR_RESET << endl;
    //remind player of what they hold in inventory
    cout << "You have " << player->getCoins() << " coins, " << player->getUnplacedArmy() << " unplaced army(ies), and " << player->unplacedCity << " unplaced city(ies)." << endl;

    //extract card ability info into string format
    string ability1S = enum_to_string(card->ability1);
    string ability2S = enum_to_string(card->optional_ability);

    //Check if the cards offer goods (stackable abilities/top of card effect). If so, check if there is a second one.
    if (!enum_to_string(card->good).empty()){
        processCardGood(card->good, player);
        if (!enum_to_string(card->good2).empty())
            processCardGood(card->good2, player);
    }

    //user can choose to take the immediate action dictated by the card, or not
    cout << "Would you like to take the card actions? (Y/N)" << endl;
    string answer{};
    cin >> answer;

    //check if user answer is legal
    while (answer.compare("Y") != 0 && answer.compare("y") != 0 && answer.compare("n") != 0 && answer.compare("N") != 0){
        cout << "Please enter Y or N" << endl;
        cin >> answer;
    }

    //if user answers yes, check if card has OR or AND ability
    if (answer.compare("Y") == 0 || answer.compare("y") == 0){
        //if card has OR abilities, allow user to pick only one to execute
        if (card->abilityOr == true){
            int option;
            cout << "Would you like to perform: " << ability1S << " OR " << ability2S << "? (1/2)" << endl;
            cin >> option;
            while (cin.fail() || (option != 1 && option !=2)){
                cout << "Please enter 1 or 2" << endl;
                cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> option;

            }
            if (option == 1){
                cout << "You've chosen to " << ability1S << endl;
                processPossibleAction(card->ability1, player);
            }
            else if (option == 2){
                cout << "You've chosen to " << ability2S << endl;
                processPossibleAction(card->optional_ability, player);
            }
        }
        //if card has AND abilities, perform both in order of appearance
        else if (card->abilityAnd){
            cout << "Perform both actions: " << ability1S << " AND " << ability2S << endl;
            cout << "First: " << ability1S << endl;
            processPossibleAction(card->ability1, player);
            showMapAndHandAndPlayers();
            cout << "Now: " << ability2S << endl;
            processPossibleAction(card->optional_ability, player);
        }
        //if card has 1 ability, perform the action immediately
        else {
            cout << "Perform the action: " << ability1S << endl;
            processPossibleAction(card->ability1, player);
        }
    }
    //executed when user decides to not take card action
    else {
        cout << "Action(s) not taken. Skipping turn." << endl;
    }
}

//Helper method to set armies, takes the player and number of armies they can set as parameter
void GameEngine::getArmiesHelper(int numOfArmies, Player* player){
    bool zeroArmiesToPlace = false;
    int numOfArmiesToPlace{};
    int region{};
    int armies = numOfArmies;

    //check if user can set extra armies everytime they have a set army action card. If so, add it to the number of armies they are allowed to set for the turn
    if (player->getNumOfExtraPlacementPerTurn()!=0) {
        armies += player->getNumOfExtraPlacementPerTurn();
    }

    //while player can still place armies, loop
    while (!zeroArmiesToPlace){
        if (player->getPlayerStrategy()->isBot()==false) {
            showMapAndHandAndPlayers(); //display map before/after every move if Player is Human
        }

        do{
            cout << "Where would you like to set some armies ? (" << armies << " to set, from which " << player->getNumOfExtraPlacementPerTurn() << " is from your Goods) NOTE: You can only set armies if you have a city in the region:" << endl;
            cin >> region;
            while (cin.fail()){ //check for valid input
                cout << "ERROR -- You did not enter an integer, enter a valid integer:" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> region;
            }
        } while(!mainMap->hasCity(player->getId(), region) && mainMap->getStartZone() != region); //check if player has any city, or if the chosen region is the start zone

        //ask player how many armies to place, remind player how many they have left to place
        do {
            cout << "How many armies do you want to place in region " << region << "? (you have " << armies << " armies left to be placed):" << endl; //need a check for numbers given
            cin >> numOfArmiesToPlace;
            while (cin.fail()){ //check for valid input
                cout << "ERROR -- You did not enter an integer, enter a valid integer:" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> numOfArmiesToPlace;
            }
        } while(numOfArmiesToPlace>armies || numOfArmiesToPlace <= 0); //check if input > allowed number of moves, or if negative input

        //place armies in region
        player->PlaceNewArmies(region, numOfArmiesToPlace, mainMap);

        //decrement number of moves
        armies -= numOfArmiesToPlace;

        //check if any moves left, if yes, restart loop
        if (armies == 0)
            zeroArmiesToPlace = true;
    }
};

//Helper method to moves armies, takes number of moves, player and card ability.
void GameEngine::moveArmiesHelper(int numOfArmies, Player* player, ABILITIES ability){
    bool zeroArmiesToPlace = false;
    int numOfArmiesToPlace{};
    int startRegion{};
    int finalRegion{};
    int armies = numOfArmies; //this is the number of moves

    //check if user can have extra moves everytime they have a move army action card. If so, add it to the number of moves for the turn
    if (player->getNumOfExtraMovePerTurn()!=0){
        armies += player->getNumOfExtraMovePerTurn();
    }

    //while player can still move armies, loop
    while (!zeroArmiesToPlace) {
        if (player->getPlayerStrategy()->isBot()==false) {
            showMapAndHandAndPlayers(); //show map after every single move
        }

        do {
            cout << "Please enter the id of a legal start region: " << endl;
            cin >> startRegion;
            while (cin.fail()){ //check for legal start region
                cout << "ERROR -- You did not enter an integer, enter a valid integer" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> startRegion;
            }
        } while (mainMap->getArmies(player->getId(), startRegion) == 0); //check if start region entered has any army of the player

        //remind player how many armies they have in start region, and how many moves they have left. Also show if they have extra moves from card GOODS
        cout << "You have " << mainMap->getArmies(player->getId(), startRegion) << " armies in region " << startRegion << endl;
        cout << "You have " << armies << " (" << player->getNumOfExtraMovePerTurn() << " which is/are extra from your GOODS)" << " moves left" << endl;

        do{
            mainMap->showMapASCIIonScreen(startRegion);
        cout << "Please enter the id of a legal destination region (must be adjacent): " << endl;
        cin >> finalRegion;
            while (cin.fail()){ //check for valid input
                cout << "ERROR -- You did not enter an integer, enter a valid integer:" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> finalRegion;
            }
        } while (mainMap->checkPath(startRegion, finalRegion) == 2); //check if move is actually to adjacent territory

        //show type of connection and reminder of cost
        cout <<"You are going to move over " << enum_to_string(mainMap->checkPath(startRegion, finalRegion)) << ". (Cost = 1 move/land OR 3 moves/sea)" << endl;

        //move over land
        if (mainMap->checkPath(startRegion, finalRegion) == 0){
            cout << "Please enter the number of armies you'd like to move: " << endl;
            cin >> numOfArmiesToPlace;
            while (cin.fail() || numOfArmiesToPlace > mainMap->getArmies(player->getId(), startRegion) || numOfArmiesToPlace < 0) { //check valid input
                cout << "ERROR -- You did not enter a valid number. Try again:" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> numOfArmiesToPlace;
            }
            armies = player->moveOverLand(1, armies, startRegion, finalRegion, numOfArmiesToPlace, ability, mainMap); //perform the move, and set number of moves left
        }

        //move over water, check if player has enough moves to do so first
        else if (mainMap->checkPath(startRegion, finalRegion) == 1 && armies >= (3-player->getNumOfFlight())){
            cout << "Please enter the number of armies you'd like to move: " << endl;
            cin >> numOfArmiesToPlace;
            while (cin.fail() || numOfArmiesToPlace > mainMap->getArmies(player->getId(), startRegion) || numOfArmiesToPlace < 0) { //check for valid input
                cout << "ERROR -- You did not enter a valid number. Try again:" << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> numOfArmiesToPlace;
            }
            //check if player can perform action depending on number of armies entered
            if (armies >= numOfArmiesToPlace*(3-player->getNumOfFlight())){
                armies = player->moveOverWater(3, armies, startRegion, finalRegion, numOfArmiesToPlace, ability, mainMap);
            }
        }

        //if not enough moves, restart loop
        else if (armies < (3-player->getNumOfFlight())){
            cout << "Not enough to move over sea!" << endl;
        }

        //check if player has any moves left, if yes, loop
        if (armies == 0) {
            zeroArmiesToPlace = true;
        }
    }
};

//call helper methods defined above depending on abilities of picked card
void GameEngine::processPossibleAction(ABILITIES ability, Player *player){
    switch (ability){
        {   //all get army abilities are to set armies on territories. All call getArmiesHelper
            case GET_ONE_ARMY:
            {
                getArmiesHelper(1, player);
                break;
            }
            case GET_TWO_ARMIES:{
                getArmiesHelper(2, player);
                break;
            }
            case GET_THREE_ARMIES: {
                getArmiesHelper(3, player);
                break;
            }
            case GET_FOUR_ARMIES:
                getArmiesHelper(4, player);
                break;
            case GET_FIVE_ARMIES:
                getArmiesHelper(5, player);
                break;
            case MOVE_ONE_ARMY:{ //all move army abilities are to move armies from a start zone to a destination. All call moveArmiesHelper
                moveArmiesHelper(1, player, ability);
                break;
            }
            case MOVE_TWO_ARMIES:{
                moveArmiesHelper(2, player, ability);
                break;
            }
            case MOVE_THREE_ARMIES:{
                moveArmiesHelper(3, player, ability);
                break;
            }
            case MOVE_FOUR_ARMIES:{
                moveArmiesHelper(4, player, ability);
                break;
            }
            case MOVE_FIVE_ARMIES:{
                moveArmiesHelper(5, player, ability);
                break;
            }
            case MOVE_SIX_ARMIES:{
                moveArmiesHelper(6, player, ability);
                break;
            }
            case REMOVE_ONE_ENEMY_ARMY:{ //attack and destroy an army. Either NPC or player owned.
                int index{};
                int region{};

                //ask for who to attack
                cout << "Enter the index of the enemy you wish to attack (enter your own index if you wish to attack an NPC): " << endl;
                cin >> index;
                while (cin.fail() || index < 0 || index >= getNumberOfPlayers()){ //check for valid input
                    cout << "ERROR -- Enter a valid integer:" << endl;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cin >> index;
                }

                //attack npc
                if (index == currentPlayer){
                    cout << "Attacking NPC..." << endl;
                    do{
                        cout << "Enter the region id (you must at least have an army present on the entered region): " << endl;
                        cin >> region;
                        while (cin.fail()) {
                            cout << "ERROR -- Enter a valid integer:" << endl;
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            cin >> region;
                        }
                    } while(mainMap->getArmies(player->getId(), region) == 0);
                    player->DestroyArmy(region, nullptr, mainMap);
                }

                //if there are more than 2 players, check if target enemy is immune. Prompt to pick another player if target is immune
                else if (numberOfPlayers > 2) {
                    while (players[index]->getImmunity()) {
                        cout << "Pick another player, the chosen one is immune to attacks" << endl;
                        cin >> index;
                    }
                }

                //to check specifically if there are only 2 players. If the only other player is immune, skip this attack.
                else if (players[index]->getImmunity())
                    cout << "Skipping this move since the enemy is immune to attacks" << endl;
                else { //in any other case, prompt for a region where the attacker has at least an army on
                    do{
                        cout << "Enter the region id (you must at least have an army present on the entered region): " << endl;
                        cin >> region;
                        while (cin.fail()) { //check for valid input
                            cout << "ERROR -- Enter a valid integer:" << endl;
                            cin.clear();
                            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            cin >> region;
                        }
                    } while(mainMap->getArmies(player->getId(), region) == 0);
                    player->DestroyArmy(region, players[index], mainMap);
                }
                break;
            }
            case BUILD_NEW_CITY: { //build a city on a territory.
                int region{};
                do{
                    cout << "Enter the region id where you would like to build a city (you must have at least one army in the region): " << endl;
                    cin >> region;
                    while (cin.fail()) {
                        cout << "ERROR -- You did not enter an integer, enter a valid integer:" << endl;
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cin >> region;
                    }
                } while(mainMap->getArmies(player->getId(), region) == 0);
                player->BuildCity(region, mainMap);
                break;
            }
            default:
                break;
        }
    }
};
//helper function for drawing out "Board"
string GameEngine::getCardTitleAndTypeAndGoodInColor(Card *c, int width) const {
    //if width set>0, add spaces to behave like setw().
    //had to rewrite this to handle ansi codes
    //Note:  CAN go over
    string s="";
    //title
    int size=c->title.length() + c->getCardType().length() + c->getCardGoodDescription().length()+2;
    s+=ANSI_COLOR_PURPLE + c->title;
    s+=string{ANSI_COLOR_BLUE} + " " +c->getCardType();
    s+= string{ANSI_COLOR_CYAN} + " " + c->getCardGoodDescription();
    if (width>0 && size<width)
    {
        s+=string(width-size,' ');
    }    return s;
}

//Destructor
GameEngine::~GameEngine() {
    if (SHOW_MEMORY_GAMEENGINE) {
        cout << "MEMORY: GameEngine Destructor started" << endl;
    }
    if (hand!= nullptr) { //hand may never have been instantiated.
        delete hand;
        hand=nullptr;
    }
    delete mainDeck;
    mainDeck=nullptr;
    for (int i=0;i<4;i++) {
        if (players[i]!=nullptr) {
            delete players[i];
            players[i]=nullptr;
        }
    }
    delete mainMap;
    mainMap=nullptr;
    if (SHOW_MEMORY_GAMEENGINE) {
        cout << "MEMORY: GameEngine Destructor Finished" << endl;
    }


}
//Assignment operator (deep copy)
GameEngine &GameEngine::operator=(const GameEngine &source) {
    if (SHOW_DEBUG_GAMEENGINE) {
        cout << "DEBUG: Map Assignment Operator= started" << endl;
    }
    if (this == &source) {
        return *this;
    }
    //copy over shallow stuff
    currentPlayer=source.currentPlayer;
    startingPlayer=source.startingPlayer;
    numberOfPlayers=source.numberOfPlayers;
    roundNumber=source.roundNumber;
    //handle deep copy stuff
    //delete the heap items from this
    //Players
    for (int i=0;i<4;i++) {
        delete this->players[i];
        this->players[i]=nullptr;
        if (source.players[i]!=nullptr) { //possible for players to be null
            players[i]=new Player(*source.players[i]);
        }
    }
    //deck
    delete this->mainDeck;
    this->mainDeck=new Deck(*source.mainDeck);
    //hand
    delete this->hand;
    this->hand=nullptr;
    if (source.hand!= nullptr) { //possible for hand to be null
        hand=new Hand(*source.hand);
        hand->deck=mainDeck; //reattach hand to current deck
    }
    //map
    delete this->mainMap;
    this->mainMap=new Map(*source.mainMap);
    return *this;
}

void GameEngine::setCurrentPlayer(int p) {
    currentPlayer=p;
    currentCard=nullptr;
//    refreshPhase=true;
//    notify();
//    refreshPhase=false;
}

void GameEngine::setCurrentCard(Card *c) {
    currentCard=c;
    refreshPhaseCard=true;
    notify();
    refreshPhaseCard=false;
}

//Demo stuff
void part3Demo() {
    cout << "Starting Demo for Part 3..." << endl;
    //Create GameEngine with test data
    mainGame = new GameEngine();
    mainGame->createTestObjects(); // only for testing.  will be replaced by other parts

    cout << "DEMO:  Map and objects created. Starting 3 rounds:" << endl;

    int rounds=3;
    while (rounds>0) {
        rounds--;
        mainGame->mainMap->showMapASCIIonScreen();
        mainGame->takeTurn();
    }
    delete mainGame;
    mainGame=nullptr;
}
//Demo stuff
void part5Demo() {
    cout << "Part 5 demo is identical to part 4, so we will just call part 4..";
    part4Demo();
    cout << "End of 'Part 5' Demo" << endl;
}

//Demo stuff
void part4Demo() {
    cout << "Starting Demo for Part 4 and 5..." << endl;
    cout << "DEMO: " << endl;
    //Create GameEngine with test data
    mainGame = new GameEngine();
    mainGame->createTestObjects(); // only for testing.  will be replaced by other parts

    Deck *limitedDeck=new Deck();
    limitedDeck->addCard(new Card("DEMO Place Armies", ARCANE, FLIGHT, NO_GOOD, false, false, GET_FIVE_ARMIES, NO_ABILITY));
    limitedDeck->addCard(new Card("DEMO Move Armies Land", ARCANE, PLUS_1_MOVE, NO_GOOD, false, false, MOVE_SIX_ARMIES, NO_ABILITY));
    limitedDeck->addCard(new Card("DEMO Move Armies Sea", ARCANE, PLUS_1VP_PER_ARCANE, NO_GOOD, false, false, MOVE_SIX_ARMIES, NO_ABILITY));
    limitedDeck->addCard(new Card("DEMO Build City", ANCIENT, FLIGHT, NO_GOOD, false, false, BUILD_NEW_CITY, NO_ABILITY));
    limitedDeck->addCard(new Card("DEMO Destroy Army", ANCIENT, THREE_ELIXIRS, NO_GOOD, false, false, REMOVE_ONE_ENEMY_ARMY, NO_ABILITY));
    limitedDeck->addCard(new Card("DEMO AND action", ANCIENT, THREE_ELIXIRS, NO_GOOD, false, true, MOVE_FOUR_ARMIES, GET_FIVE_ARMIES));
    limitedDeck->addCard(new Card("DEMO OR action", ANCIENT, THREE_ELIXIRS, NO_GOOD, true, false, MOVE_FOUR_ARMIES, GET_FIVE_ARMIES));
    limitedDeck->addCard(new Card("DEMO Place Armies2", ANCIENT, THREE_ELIXIRS, NO_GOOD, false, false, GET_FOUR_ARMIES, NO_ABILITY));
    *mainGame->mainDeck=*limitedDeck; //call assignment operator to make mainDeck have a deep copy of limitedDeck
    delete limitedDeck;
    limitedDeck= nullptr;
    delete mainGame->hand;
    mainGame->hand = new Hand(*mainGame->mainDeck);

    cout << "DEMO:  Map and objects created. Starting 4 rounds:" << endl;

    int rounds=4;
    while (rounds>0) {
        rounds--;
        mainGame->mainMap->showMapASCIIonScreen();
        mainGame->takeTurn(); //part 4,5
    }
    delete mainGame;
    mainGame=nullptr;
    cout << "If number of coins of player reaches 0, they can only pick the first card for the rest of the game." << endl;
    cout << "End of Demo 4" << endl;

}

int GameEngine::getCurrentPlayer() {
    return currentPlayer;
}

//ALL BOT RELATED METHODS FOR STRATEGY PATTERN
void GameEngine::processBotPossibleAction(ABILITIES ability, Player *player){
    switch (ability){
        {   //all get army abilities are to set armies on territories. All call getArmiesHelper
            case GET_ONE_ARMY:
            {
                getBotArmiesHelper(1, player);
                break;
            }
            case GET_TWO_ARMIES:{
                getBotArmiesHelper(2, player);
                break;
            }
            case GET_THREE_ARMIES: {
                getBotArmiesHelper(3, player);
                break;
            }
            case GET_FOUR_ARMIES:
                getBotArmiesHelper(4, player);
            break;
            case GET_FIVE_ARMIES:
                getBotArmiesHelper(5, player);
            break;
            case MOVE_ONE_ARMY:{ //all move army abilities are to move armies from a start zone to a destination. All call moveArmiesHelper
                moveBotArmiesHelper(1, player, ability);
                break;
            }
            case MOVE_TWO_ARMIES:{
                moveBotArmiesHelper(2, player, ability);
                break;
            }
            case MOVE_THREE_ARMIES:{
                moveBotArmiesHelper(3, player, ability);
                break;
            }
            case MOVE_FOUR_ARMIES:{
                moveBotArmiesHelper(4, player, ability);
                break;
            }
            case MOVE_FIVE_ARMIES:{
                moveBotArmiesHelper(5, player, ability);
                break;
            }
            case MOVE_SIX_ARMIES:{
                moveBotArmiesHelper(6, player, ability);
                break;
            }
            case REMOVE_ONE_ENEMY_ARMY: { //attack and destroy an army. Note, bots cannot attack NPCs.
                bool stopper = false; //exit condition
                int randomPlayer = rand() % numberOfPlayers; //randomly generated enemy player
                int randomRegion = mainMap->getRandomValidTerritoryID(); //randomly generated region to attack

                if (numberOfPlayers >= 2) {
                    //while randomly generated enemy player has immunity or is the attacking player, generate another player
                    while (players[randomPlayer]->getImmunity() || players[randomPlayer]->getId() == currentPlayer) {
                        //if there are only 2 players, and the attacked is immune, skip this move
                        if (numberOfPlayers == 2 && players[randomPlayer]->getImmunity() && players[randomPlayer]->getId() != currentPlayer){
                            stopper = true;
                            break;
                        }
                        randomPlayer = rand() % numberOfPlayers;
                    }

                    //to avoid infinite loop if there are no two opposing players on the same territory on the entire map
                    if (!stopper){
                    map<int, int> m{};
                        //while the random region has no armies of both sides, or if one of the two (the attacker or enemy) doesn't have an army on the random region, generate another region
                        while (mainMap->getArmies(randomPlayer, randomRegion) == 0 || mainMap->getArmies(player->getId(), randomRegion) == 0) {
                           if (m.find(randomRegion) == m.end()) //if you don't find this random region in the hashmap, add it
                                m.insert({randomRegion, randomRegion});
                            randomRegion = mainMap->getRandomValidTerritoryID(); //generate new region
                            if (m.size() == mainMap->getTerritories()->size()){ //if hashmap contains all territories, exit loop since no attack can happen
                                stopper = true;
                                break;
                            }
                        }
                    }

                    //if found a valid region, perform the removal of the enemy army
                    if (!stopper){
                        player->DestroyArmy(randomRegion, players[randomPlayer], mainMap);
                    }
                    //else, skip attack
                    else if (stopper)
                       cout << "Skipping no one to attack" << endl;
                }
                break;
            }
            case BUILD_NEW_CITY: { //build a city on a territory.
                //check if user still has unplaced cities
                if (player->getUnplacedCity() != 0){
                    int randomRegion=mainMap->getRandomValidTerritoryID(); //generate random region
                    while(mainMap->getArmies(player->getId(), randomRegion) == 0){ //ensure region has player army on it
                        randomRegion = mainMap->getRandomValidTerritoryID();
                    };
                    player->BuildCity(randomRegion, mainMap); //build city
                }
                else
                    cout << player->getName() << " has 0 cities left to place, skipping this move." << endl; //skip move if no cities in hand left
                break;
            }
            default:
                break;
        }
    }
};

//helper method to set bot armies
void GameEngine::getBotArmiesHelper(int numOfArmies, Player* player){

    //randomize region
    int randomRegion = mainMap->getRandomValidTerritoryID();

    //check if user can set extra armies everytime they have a set army action card. If so, add it to the number of armies they are allowed to set for the turn
    if (player->getNumOfExtraPlacementPerTurn()!=0) {
        numOfArmies += player->getNumOfExtraPlacementPerTurn();
    }

    //while random region has no city of the player or is not start zone, generate new region
    while (!mainMap->hasCity(player->getId(), randomRegion) && mainMap->getStartZone() != randomRegion) {
        randomRegion = mainMap->getRandomValidTerritoryID();
    }

    //if no armies left in hand, skip move
    if (player->getUnplacedArmy() == 0){
        cout << "Player " << player->getName() << " has 0 armies left to set, skipping this move." << endl;
    }

    //if armies left in hand is less than the amount allowed to be set by the card, set the remaining armies in hand
    else if (player->getUnplacedArmy() < numOfArmies && player->getUnplacedArmy() != 0) {
        player->PlaceNewArmies(randomRegion, player->getUnplacedArmy(), mainMap);
    }

    //in other cases set all armies allowed by card in a single region
    else {
        player->PlaceNewArmies(randomRegion, numOfArmies, mainMap);
    }
};

//Helper method to moves armies, takes number of moves, player and card ability.
void GameEngine::moveBotArmiesHelper(int numOfArmies, Player* player, ABILITIES ability){

    if (mainGame->mainMap->howManyArmiesDoesPlayerHaveOnMap(player->getId()) == 0){
        cout << "No armies on the map. Skipping move." << endl;
        return;
    }

    bool zeroArmiesToPlace = false;
    //check if user can have extra moves everytime they have a move army action card. If so, add it to the number of moves for the turn
    if (player->getNumOfExtraMovePerTurn()!=0){
        numOfArmies += player->getNumOfExtraMovePerTurn();
    }

    while (!zeroArmiesToPlace) {
        int randomStartRegion = mainMap->getRandomValidTerritoryID(); //random start region
        int randomFinalRegion = mainMap->getRandomValidTerritoryID(); //random final region

        //generate random start region, while it has 0 army of the player
        while (mainMap->getArmies(player->getId(), randomStartRegion) == 0){
            randomStartRegion = mainMap->getRandomValidTerritoryID();
        }

        //generate random final region, while it is not connected to start region
        while (mainMap->checkPath(randomStartRegion, randomFinalRegion) == 2){
            randomFinalRegion = mainMap->getRandomValidTerritoryID();
        }

            //move over land, if connect by land
            if (mainMap->checkPath(randomStartRegion, randomFinalRegion) == 0){
                //output move info
                cout << "Moving over " << enum_to_string(mainMap->checkPath(randomStartRegion, randomFinalRegion)) << "... " << endl;
                numOfArmies = player->moveOverLand(1, numOfArmies, randomStartRegion, randomFinalRegion, 1, ability, mainMap); //perform the move, and set number of moves left
                if (!player->getPlayerStrategy()->isBot()) {
                    showMapAndHandAndPlayers(); //show if human
                }
            }

            //move over water, if connect by water and player has enough moves
            else if (mainMap->checkPath(randomStartRegion, randomFinalRegion) == 1 && numOfArmies >= (3-player->getNumOfFlight())){
                if (numOfArmies >= 1*(3-player->getNumOfFlight())){
                    //output move info
                    cout << "Moving over " << enum_to_string(mainMap->checkPath(randomStartRegion, randomFinalRegion)) << "... " << endl;
                    numOfArmies = player->moveOverWater(3, numOfArmies, randomStartRegion, randomFinalRegion, 1, ability, mainMap);
                    if (!player->getPlayerStrategy()->isBot()) {
                        showMapAndHandAndPlayers();
                    }
                }
            }

            //try another move if player cannot move oversea due to generated final region
            else if (numOfArmies < (3-player->getNumOfFlight())){
                //check for special case when all surrounding territories are sea and player does not have enough move.
                list<MapEdge *> *temp = mainMap->getTerritory(randomStartRegion)->getAdjacentTerritories();
                list<MapEdge *>::iterator it;
                bool stopper = false;
                for (it = temp->begin(); it != temp->end(); ++it){
                    if (enum_to_string(*((**it).pathType)) != "Sea"){
                        stopper = true;
                        break;
                    }
                }

                if (!stopper){
                    cout << "Player surrounded by sea, and does not have enough moves to cross over seas. Skipping move." << endl;
                    numOfArmies = 0;
                }
            }

            //check if player has any moves left, if yes, loop
            if (numOfArmies == 0) {
                zeroArmiesToPlace = true;
            }
    }
};

//check cost of cards
int GameEngine::botCardCost(int position) {
    switch (position) {
        case 0:
            return 0;
        case 1:
        case 2:
            return 1;
        case 3:
        case 4:
            return 2;
        case 5:
            return 3;
        default:
            if (SHOW_DEBUG_GAMEENGINE) {
                cout << "ERROR:  GameEngine::cardCost called with invalid position " << position << endl;
            }
            return 0;
    }
}
void GameEngine::tournamentStart() {
    MapLoader* mapLoader=new MapLoader();
    //Player* players[2];
    string playername;
    int n;
    int coins;
    bool samename=false; //catches identical name
    int strategy;
    string mode;
    this->setNumberOfPlayers(2);

    //print welcome banner
   GameLoader:: welcomeBanner();
//the player decides how many players will play

    cout << "Please select game Mode:" << endl;
    cout << "Enter 1 for Tournament Mode or 2 for Single Game Mode" << endl;

    cin >> n;
    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    while(cin.fail() || n<1 || n>2 ) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        cout << "Please enter a valid number(1 or 2):" << endl;            cin >> n;
    }


    if(n==1){
        //Tournament Mode
        tournamentMode=true;
        //uses maplaoder functions to request user map choice and to validate the map
        do{
            this->mainMap= mapLoader->readFile(mapLoader->fileInput());
            if( this->mainMap==nullptr)
                cout<< "The selected Map File is invalid. Please select a valid Map File from the options below"<<endl;

        }while(this->mainMap==nullptr);
        //set start zone
        this->mainMap->setStartZone();
        //set coins
        coins=12;
        cout<< "This is a 2-Player Tournament"<<endl;
        //each players name and details are intialized

        for(int i =0; i<4;i++){
            if(i<2)
            {
                do {
                    samename=false;
                    cout << "Please enter the name of Player " << i << endl;
                    cin >> playername;

                    //check if name is duplicate
                    for (int j = 0; j < i; j++) {
                        if (playername.compare(this->players[j]->getName()) == 0) {
                            samename = true;
                            cout<<"Your name matches with another players name. A different name is required."<<endl;
                            break;
                        }
                    }
                }while(samename);
                cout << "Select mode (1) Human (2) Greedy Bot (3) Moderate Bot:" << endl;
                cin>>strategy;
                cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                while(cin.fail() || strategy<1 || strategy>3 ) {
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    cout << "Please enter a valid number(1, 2 or 3):" << endl;
                    cin >> strategy;
                }

                this->players[i]=new Player(i, playername, 0, coins, 3, 18, 0,0,0);
                switch (strategy) {
                    case 1: {
                        this->players[i]->setPlayerStrategy(new HumanPlayerStrategy);
                        mode= "Human Mode";
                        break;
                    }
                    case 2: {
                        this->players[i]->setPlayerStrategy(new GreedyComputerStrategy);
                        mode= "Greedy Computer Mode";
                        break;
                    }
                    case 3: {
                        this->players[i]->setPlayerStrategy(new ModerateComputerStrategy);
                        mode= "Moderate Computer Mode";
                        break;
                    }
                    default: {
                        cout << "ERROR: invalid mode" << endl;
                    }

                }
                cout<< playername << " has been created with " << coins << " coins, 3 unplaced cities and 18 unplaced armies and is currently operating under " + mode << endl;

            }

            else
                this->players[i]= nullptr;
        }
        //deck of cards is created using function from cards which takes a deck and number of players as parameters to allocate the appropriate cards
        this->mainDeck=new Deck();
        GameLoader :: fillDeck(this->mainDeck, 2);
        this->hand=new Hand(*this->mainDeck); //create hand,  make link with deck, and draw first 6 cards

    }


    //clean up memory
    delete mapLoader;
    mapLoader= nullptr;


}

void GameEngine::showTournamentResults() {
    if (tournamentMode== false) {
        return;
    }
    string s;
    cout << ANSI_YELLOW_ON_BLACK;
    cout << "Tournament Results!" << endl;
    cout << setw(10) << left <<"Player" << setw(10)<< "Cards" << setw(18) << "Victory Points" << setw(10) << "Coins" << endl;
    for (int i=0;i<2;i++) {
        s=to_string(i);
        if (players[i]->getPlayerStrategy()->isBot()) {
            s+=" (bot)";
        }
        cout << setw(10) << s << setw(10) << players[i]->ownedCards->size();
        cout << setw(18) << players[i]->getVictoryPoints();
        cout << setw(10) << players[i]->getCoins();
        cout << endl;
    }

    cout << ANSI_COLOR_RESET;

}
