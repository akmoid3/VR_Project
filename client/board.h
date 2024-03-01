/**
 * @file		board.h
 * @ingroup Client
 * @brief	Declaration of the Board class.
 */
#pragma once

#include <vector>
#include "vehicle.h"

/**
* @brief Represents the game board in the Rush Hour game.
*/
class Board {
public:

	Board();

	void display();

	bool moveVehicle(Vehicle& vehicle, char direction);

	void addVehicle(Vehicle& vehicle);

	const int boardSize();

	void initBoard();

protected:
	// Methods
	bool isValidMove(const Vehicle& vehicle, int newRow, int newCol, char direction);

	// Var
	static const int m_boardSize{ 6 }; ///< The size of the game board.
	std::string m_grid[m_boardSize][m_boardSize]; ///< The game board grid.

};

