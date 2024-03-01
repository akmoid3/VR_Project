/**
 * @file vehicle.cpp
 * @ingroup Client
 * @brief Implementation of the Vehicle class.
 */
#include "vehicle.h"

/**
* @brief Constructs a new Vehicle object.
*
* @param symbol The symbol representing the vehicle.
* @param length The length of the vehicle.
* @param row The row position of the vehicle.
* @param col The column position of the vehicle.
* @param isHorizontal Indicates if the vehicle is placed horizontally.
*/
Vehicle::Vehicle(std::string symbol, int length, int row, int col, bool isHorizontal)
    : symbol(symbol), length(length), row(row), col(col), isHorizontal(isHorizontal) {};