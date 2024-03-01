/**
 * @file		rushhourgame.h
 * @ingroup Client
 * @brief	Declaration of the RushHourGame class.
 */

#pragma once

#include <vector>
#include "board.h"
#include "vehicle.h"
#include "engine.h"

 /**
  * @brief Represents the Rush Hour game logic.
  */
class RushHourGame {
public:

    int loadLogicLevelFromFile(const std::string& filename);

    bool matchVehicle(Mesh * car);

    void play(char direction);

    bool isGameWon();

    bool isDirectionValid(char direction);

    Vehicle& findVehicle(std::string symbol);

    void moveMesh(Mesh* car, char direction);

protected: 
    Board board; ///< The game board.
    std::vector<Vehicle> vehicles; ///< The vehicles in the game.

};
