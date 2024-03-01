/**
 * @file		rushhourgame.cpp
 * @ingroup Client
 * @brief	Implementation of the RushHourGame class.
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "rushHourGame.h"
#include <string>
#include <fstream>
#include <chrono>

Mesh* selectedCar = nullptr;
Vehicle* selectedVehicle = nullptr;

/**
 * @brief Loads a logic level from a file.
 *
 * @param filename The name of the file to load.
 * @return int Status of the file loading operation: 0 if success, 1 if failure.
 */
int RushHourGame::loadLogicLevelFromFile(const std::string& filename) {
	board.initBoard();
	vehicles.clear();
	// Read level from file
	std::ifstream inputFile;
	inputFile.open(filename.c_str());

	if (!inputFile.is_open()) {
		std::cerr << "Unable to open the file: " << filename << std::endl;
		return 1;
	}

	std::string symbol;
	int length, row, col;
	bool isHorizontal;

	while (inputFile >> symbol >> length >> row >> col >> isHorizontal) {
		vehicles.emplace_back(symbol, length, row, col, isHorizontal);
	}

	inputFile.close();


	// Place vehicles on logic board
	for (Vehicle& vehicle : vehicles) {
		board.addVehicle(vehicle);
	}

	// Reinitialize variables
	selectedCar = nullptr;
	selectedVehicle = nullptr;

	return 0;
}

/**
 * @brief Checks if a mesh matches a vehicle.
 *
 * @param nextCar The mesh to check.
 * @return true if the mesh matches a vehicle, false otherwise.
 */
bool RushHourGame::matchVehicle(Mesh* nextCar) {
	if (nextCar) {

		// Check if selected object is a valid vehicle
		selectedVehicle = &findVehicle(nextCar->name());
		if ((*selectedVehicle).symbol.compare("-") == 0) {
			selectedVehicle = nullptr;
			std::cout << "Invalid CAR. Please try again." << std::endl;
			return false;
		}

		if (selectedCar != nextCar) {
			// Make the correct object blink
			nextCar->startBlink();
			if (selectedCar != nullptr)
				selectedCar->stopBlink();
			selectedCar = nextCar;
		}
	}
	return true;
}

/**
 * @brief Move a car in the specified direction.
 *
 * @param direction The direction to move the car ('L', 'R', 'U', or 'D').
 */
void RushHourGame::play(char direction) {

	// Make a move if conditions are valid
	if (!isGameWon()) {

		if (selectedVehicle) {
			if (!isDirectionValid(direction) || !board.moveVehicle(*selectedVehicle, direction)) {
				std::cout << "Invalid move. Please try again." << std::endl;
				return;
			}

			moveMesh(selectedCar, direction);

			board.display(); // Uncomment for debugging
			std::cout << "You MOVED " << std::endl;
		}
		else {
			std::cout << "No valid vehicle selected." << std::endl;
		}

	}
	else {
		std::cout << "Congratulations! You've won the game." << std::endl;
	}

}

/**
 * @brief Checks if the game is won.
 *
 * @return true if the game is won, false otherwise.
 */
bool RushHourGame::isGameWon() {
	return vehicles[0].col + vehicles[0].length == board.boardSize(); // Red car reached the rightmost column
}


/**
 * @brief Checks if a direction is valid.
 *
 * @param direction The direction to check ('L', 'R', 'U', or 'D').
 * @return true if the direction is valid, false otherwise.
 */
bool RushHourGame::isDirectionValid(char direction)
{
	// Check right direction
	if ((selectedVehicle->isHorizontal && (direction != 'L' && direction != 'R')) || (!selectedVehicle->isHorizontal && (direction != 'U' && direction != 'D')))
		return false;
	return true;
}

/**
 * @brief Finds a vehicle by its symbol.
 *
 * @param symbol The symbol of the vehicle to find.
 * @return Vehicle& A reference to the found vehicle.
 */
Vehicle& RushHourGame::findVehicle(std::string symbol) {
	// Search vehicle
	for (Vehicle& vehicle : vehicles) {
		if (symbol.compare(vehicle.symbol) == 0) {
			return vehicle;
		}
	}

	// Return an empty vehicle if not found
	static Vehicle vehicle("-", 0, 0, 0, 0);
	return vehicle;
}

/**
 * @brief Moves a mesh in the specified direction.
 *
 * @param car The mesh to move.
 * @param direction The direction to move the mesh ('L', 'R', 'U', or 'D').
 */
void RushHourGame::moveMesh(Mesh* car, char direction) {
	float x = 0.0f;
	float z = 0.0f;
	float unit = 42.66f;
	switch (direction)
	{
	case 'U':
		z -= unit;
		break;

	case 'D':
		z += unit;
		break;

	case 'L':
		x -= unit;
		break;

	case 'R':
		x += unit;
		break;

	default:
		break;
	}

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
	car->matrix(translation * selectedCar->matrix());
}
