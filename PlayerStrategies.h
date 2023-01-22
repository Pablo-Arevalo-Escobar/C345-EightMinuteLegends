//
// Created by Kevin Li Zhong Hao on 2021-04-12.
//

#ifndef C345_ASS2_PLAYERSTRATEGIES_H
#define C345_ASS2_PLAYERSTRATEGIES_H

#include "Cards.h"

//abstract class PlayerStrategy
class PlayerStrategy {
    public:
        int botType = 0; //1 for human 2 for greed 3 for moderate
        PlayerStrategy(): bot{false}{}; //default constrcutor
        virtual void execute() = 0; //pure virtual method to be inherited by child classes
        void processBotMove(string typeOfComputer, string targetAbility1, string targetAbility2); //to process bot moves only
        virtual ~PlayerStrategy() = default; //destructor
        bool isBot() const {return bot;} //is this strategy a bot

  protected:
    bool bot{false};
};

void ass3Part3Demo();
void ass3Part2Demo();

//concrete human strategy
class HumanPlayerStrategy: public PlayerStrategy{
    public:
        HumanPlayerStrategy() {this->botType = 1;}
        void execute() override;
        ~HumanPlayerStrategy() override = default;
};

//concrete greedy bot strategy
class GreedyComputerStrategy: public PlayerStrategy{
    public:
        GreedyComputerStrategy() {bot=true; this->botType = 2;}
        void execute() override;
        ~GreedyComputerStrategy() override = default;
};

//concrete moderate bot strategy
class ModerateComputerStrategy: public PlayerStrategy{
    public:
        ModerateComputerStrategy() {bot=true; this->botType = 3;}
        void execute() override;
        ~ModerateComputerStrategy() override = default;
};

#endif //C345_ASS2_PLAYERSTRATEGIES_H
