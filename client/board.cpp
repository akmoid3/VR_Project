/**
 * @file		board.cpp
 * @ingroup Client
 * @brief	Implementation of the Board class.
 */
#include "board.h"
#include <iostream>
#include <vector>
#include "vehicle.h"

using namespace std;

/**
 * @brief Constructs a new Board object and initializes the game board.
 */
Board::Board() {
	initBoard();
}

/**
 * @brief Displays the current state of the game board on the output console.
 */
void Board::display() {
	for (int i = 0; i < m_boardSize; ++i) {
		for (int j = 0; j < m_boardSize; ++j) {
			cout << m_grid[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

/**
 * @brief Checks if a move is valid.
 *
 * @param vehicle The vehicle to move.
 * @param newRow The new row position of the vehicle.
 * @param newCol The new column position of the vehicle.
 * @param direction The direction to move the vehicle ('L', 'R', 'U', or 'D').
 * @return true if the move is valid, false otherwise.
 */
bool Board::isValidMove(const Vehicle& vehicle, int newRow, int newCol, char direction) {
	
	int toCheckX = vehicle.row;
	int toCheckY = vehicle.col;

	if (direction == 'L') {
		toCheckY -= 1;
	}
	else if (direction == 'R') {
		toCheckY += vehicle.length;
	}
	else if (direction == 'U') {
		toCheckX -= 1;
	}
	else if (direction == 'D') {
		toCheckX += vehicle.length;
	}
	else {
		return false;
	}

	// Check for out of bounds
	if (toCheckX < 0 || toCheckX >= m_boardSize || toCheckY < 0 || toCheckY >= m_boardSize)
		return false;

	// Check for collision with another vehicle
	if (m_grid[toCheckX][toCheckY].compare(".") != 0 )
		return false;

	return true;
}

/**
 * @brief Moves a Vehicle in the specified direction if the move is valid.
 *
 * @param vehicle The Vehicle to move.
 * @param direction The direction to move the Vehicle ('L', 'R', 'U', or 'D').
 * @return true if the Vehicle was moved successfully, false otherwise.
 */
bool Board::moveVehicle(Vehicle& vehicle, char direction) {
	int toClearX = vehicle.row;
	int toClearY = vehicle.col;
	int toFillX = vehicle.row;
	int toFillY = vehicle.col;
	int newRow = vehicle.row;
	int newCol = vehicle.col;

	if (direction == 'L') {
		toClearY += vehicle.length - 1;
		toFillY -= 1;
		newCol -= 1;
	}
	else if (direction == 'R') {
		toFillY += vehicle.length;
		newCol += 1;
	}
	else if (direction == 'U') {
		toClearX += vehicle.length - 1;
		toFillX -= 1;	
		newRow -= 1;
	}
	else if (direction == 'D') {
		toFillX += vehicle.length;
		newRow += 1;
	}
	else {
		return false;
	}

	// Check if move is valid
	if (!isValidMove(vehicle, newRow, newCol, direction))
		return false;

	// Update board
	m_grid[toClearX][toClearY] = '.';
	m_grid[toFillX][toFillY] = vehicle.symbol;

	// Update vehicle position
	vehicle.row = newRow;
	vehicle.col = newCol;

	return true;
}

/**
 * @brief Adds a Vehicle to the game board.
 *
 * @param vehicle The Vehicle to add.
 */
void Board::addVehicle(Vehicle& vehicle) {
	if (vehicle.isHorizontal) {
		for (int i = 0; i < vehicle.length; i++) {
			m_grid[vehicle.row][vehicle.col + i] = vehicle.symbol;
		}
	}
	else {
		for (int i = 0; i < vehicle.length; i++) {
			m_grid[vehicle.row + i][vehicle.col] = vehicle.symbol;
		}
	}
}

/**
 * @brief Returns the size of the game board.
 *
 * @return The size of the game board.
 */
const int Board::boardSize()
{
	return m_boardSize;
}

/**
 * @brief Initializes the game board.
 */
void Board::initBoard() {
	for (int i = 0; i < m_boardSize; ++i) {
		for (int j = 0; j < m_boardSize; ++j) {
			m_grid[i][j] = '.';
		}
	}
}