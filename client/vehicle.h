/**
 * @file		vehicle.h
 * @ingroup Client
 * @brief	Declaration of the Vehicle class.
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#pragma once
#include <string>

 /**
  * @brief Represents a logical vehicle in the Rush Hour game.
  */
class Vehicle {
public:
    std::string symbol; ///< The symbol representing the vehicle.
    int length; ///< The length of the vehicle.
    int row, col; ///< The position of the vehicle.
    bool isHorizontal; ///< Indicates if the vehicle is placed horizontally.
    
    Vehicle(std::string symbol, int length, int row, int col, bool isHorizontal);
};
