cmake_minimum_required(VERSION 3.17)
project(C345_Ass2)

set(CMAKE_CXX_STANDARD 17)

add_executable(C345_Ass2 PhaseView.cpp main.cpp Map.cpp Map.h Cards.cpp Cards.h CardsDriver.cpp CardsDriver.h MapDriver.cpp MapDriver.h Player.cpp Player.h PlayerDriver.h PlayerDriver.cpp BiddingFacility.cpp BiddingFacility.h BiddingFacilityDriver.cpp BiddingFacilityDriver.h MapLoader.cpp MapLoader.h MapLoaderDriver.cpp MapLoaderDriver.h GameEngine.cpp GameEngine.h GameEnd.cpp GameEnd.h GameLoaderDriver.h GameLoaderDriver.cpp GameLoader.h GameLoader.cpp GameStartup.cpp StatisticsView.cpp StatisticsView.h GameObservers.cpp GameObservers.h Observer.cpp Observer.h Subject.cpp Subject.h PlayerStrategies.h PlayerStrategies.cpp)