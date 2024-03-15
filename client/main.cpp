/**
 * @file		 main.cpp
 *
 * @brief	 Client application for playing the Rush Hour game that uses the 3D Graphics Engine dynamic library
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 *
 * @mainpage Project Documentation
 * This work was carried out and presented as a partial requirement for the certification of the Computer Graphics course at the Scuola Universitaria Professionale della Svizzera Italiana (SUPSI),
 * Dipartimento Tecnologie Innovative, Lugano, Switzerland.
 *
 * It consists of two distinct components: the 3D Graphics Engine, which is a dynamic library, and the playable Rush Hour game, which is a client application that utilizes the 3D graphics engine.
 */

 /**
  * @defgroup Client Client
  * @brief Client application for playing the Rush Hour game that uses the 3D Graphics Engine dynamic library
  */



  //////////////
  // #INCLUDE //
  //////////////

	 // Library header:
#include "engine.h"

#include "rushHourGame.h"




// C/C++:
#include <iostream>
#include <memory>

/////////////
// GLOBALS //
/////////////
Camera* selectedCamera = nullptr;
Camera* staticCamera1 = nullptr;
Camera* staticCamera2 = nullptr;
Camera* dynamicCamera = nullptr;
glm::mat4 defaultDynamicCameraMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, 100.0f));
Mesh* car = nullptr;
bool newCarSelected = false;
bool isGameStarted = false;
int level = 1;
// Singleton instance
Engine& engine = Engine::instance();
RushHourGame game;
Node* scene = nullptr;
Node* oldScene = nullptr;


///////////////
// CALLBACKS //
///////////////

/**
 * @brief This callback is invoked each time a standard keyboard key is pressed.
 *
 * @param key key pressed id
 * @param mouseX mouse X coordinate
 * @param mouseY mouse Y coordinate
 */
void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	if (selectedCamera->name().compare("cameraDinamica") == 0) {
		std::cout << "[std key pressed]" << std::endl;
		float x = 0.0f, y = 0.0f, z = 0.0f;
		float value = 3.5f;
		float angleX = 0.0f;
		float angleY = 0.0f;
		float angleValue = 2.0f;


		switch (key)
		{
		case 'w':
			z -= value;
			break;

		case 's':
			z += value;
			break;

		case 'a':
			x -= value;
			break;

		case 'd':
			x += value;
			break;

		case 'q':
			y += value;
			break;

		case 'e':
			y -= value;
			break;

		case 't':
			angleX -= angleValue;
			break;

		case 'y':
			angleX += angleValue;
			break;

		case 'g':
			angleY -= angleValue;
			break;

		case 'h':
			angleY += angleValue;
			break;

		case 'r':
			selectedCamera->matrix(defaultDynamicCameraMatrix);
			break;

		default:
			break;
		}
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		selectedCamera->matrix( rotation * selectedCamera->matrix() * translation);
		// prendere la matrice attuale, 
	}
}

void reset() {
	car = nullptr;
	newCarSelected = false;
	isGameStarted = false;

	oldScene->removeChild(staticCamera1);
	oldScene->removeChild(staticCamera2);
	oldScene->removeChild(dynamicCamera);

	scene->addChild(staticCamera1);
	scene->addChild(dynamicCamera);
	scene->addChild(staticCamera2);
	selectedCamera = staticCamera1;

	Node* node = engine.findNode(scene, "Plane001");
	if (node) {
		Mesh* m = dynamic_cast<Mesh*>(node);
		if (m != 0)
			m->isCastingShadow(false);
	}
}


/**
* This callback is invoked each time a special keyboard key is pressed.
* @param key key pressed id
* @param mouseX mouse X coordinate
* @param mouseY mouse Y coordinate
*/
void specialCallback(int key, int mouseX, int mouseY)
{
	std::cout << "[key pressed]" << std::endl;

	// Change box rotation:
	const float speed = 1.0f;
	switch (key)
	{
	case GLUT_KEY_F1:
		selectedCamera = staticCamera1;
		break;

	case GLUT_KEY_F2:
		selectedCamera = staticCamera2;
		break;

	case GLUT_KEY_F3:
		selectedCamera = dynamicCamera;
		break;

	case GLUT_KEY_F4:
		oldScene = scene;
		scene = engine.load("../ovo/Livello1.ovo");
		game.loadLogicLevelFromFile("../logicLevels/level1.txt");
		reset();
		level = 1;
		delete oldScene;
		oldScene = nullptr;
		break;

	case GLUT_KEY_F5:
		oldScene = scene;
		scene = engine.load("../ovo/Livello2.ovo");
		game.loadLogicLevelFromFile("../logicLevels/level2.txt");
		reset();
		level = 2;
		delete oldScene;
		oldScene = nullptr;
		break;

	case GLUT_KEY_F6:
		oldScene = scene;
		scene = engine.load("../ovo/Livello3.ovo");
		game.loadLogicLevelFromFile("../logicLevels/level3.txt");
		reset();
		level = 3;
		delete oldScene;
		oldScene = nullptr;
		break;

	case GLUT_KEY_F8:
		engine.isRunning(false);
		break;

	case GLUT_KEY_UP:
		std::cout << "up" << std::endl;
		game.play('U');
		break;

	case GLUT_KEY_DOWN:
		std::cout << "down" << std::endl;
		game.play('D');
		break;

	case GLUT_KEY_LEFT:
		std::cout << "left" << std::endl;
		game.play('L');
		break;

	case GLUT_KEY_RIGHT:
		std::cout << "right" << std::endl;
		game.play('R');
		break;
	}
}


/**
 * This callback is invoked each time a mouse button is pressed or released.
 * @param button mouse button id (e.g., GLUT_LEFT_BUTTON)
 * @param state state of the mouse button (GLUT_UP or GLUT_DOWN)
 * @param x mouse X coordinate at the time of the event
 * @param y mouse Y coordinate at the time of the event
 */
void mouseClicks(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		engine.activateMouseObjectSelection(x, y);
		newCarSelected = true;
	}
}


/////////////
// METHODS //
/////////////  

/**
 * @brief Displays the game menu and status information on the screen.
 * @param engine The game Engine used for rendering text.
 */
void menu(Engine& engine) {
	engine.showFps(1.0f, engine.height() - 15.0f);
	engine.addText("Choose or reset a level : ", 1.0f, engine.height() - 30.0f);
	engine.addText("[F4] LEVEL 1 ", 1.0f, engine.height() - 50.0f);
	engine.addText("[F5] LEVEL 2 ", 1.0f, engine.height() - 70.0f);
	engine.addText("[F6] LEVEL 3 ", 1.0f, engine.height() - 90.0f);
	engine.addText("[F8] EXIT ", 1.0f, engine.height() - 110.0f);
	if (selectedCamera->name().compare("cameraStatica1") == 0)
	{
		engine.addText("Selected camera: Static Camera 1", 2.0f, 60.0f);
		engine.addText("[F2] Static Camera 2", 2.0f, 40.0f);
		engine.addText("[F3] Dynamic Camera ", 2.0f, 20.0f);
	}
	else if (selectedCamera->name().compare("cameraStatica2") == 0) {
		engine.addText("Selected camera: Static Camera 2", 2.0f, 60.0f);
		engine.addText("[F1] Static Camera 1", 2.0f, 40.0f);
		engine.addText("[F3] Dynamic Camera ", 2.0f, 20.0f);
	}
	else if (selectedCamera->name().compare("cameraDinamica") == 0) {
		engine.addText("Selected camera: Dynamic Camera", 2.0f, 60.0f);
		engine.addText("[F1] Static Camera 1", 2.0f, 40.0f);
		engine.addText("[F2] Static Camera 2", 2.0f, 20.0f);
		engine.addText("[w][a][s][d] movement", engine.width() - 200.0f, 20.0f);
		engine.addText("[t][y][g][h] rotation", engine.width() - 200.0f, 40.0f);
		engine.addText("[q][e] move up and down", engine.width() - 200.0f, 60.0f);
		engine.addText("[r] reset position", engine.width() - 200.0f, 80.0f);
	}

	if (isGameStarted == false) {
		engine.addText("To win the game, move the red car to the exit ", engine.width() / 2.0f - 170.0f, engine.height() - 50.0f);
		engine.addText("Click on a car to start playing", engine.width() / 2.0f - 110.0f, engine.height() - 100.0f);
	}
	else {
		if (game.isGameWon()) {
			engine.addText("YOU WON!", engine.width() / 2.0f - 20.0f, engine.height() - 50.0f);
		}
		else {
			engine.addText("MOVEMENT : [KEY UP] [KEY DOWN] [KEY LEFT] [KEY RIGHT]", engine.width() / 2.0f - 200.0f, engine.height() - 50.0f);
		}

	}

	std::string text = "SELECTED LEVEL: " + std::to_string(level);
	engine.addText(text.c_str(), engine.width() / 2.0f - 70.0f, 20.0f);

}

//////////
// MAIN //
//////////

/**
 * Application entry point.
 * @param argc number of command-line arguments passed
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char* argv[])
{
	// Credits:
	std::cout << "RushHour, Gruppo 5 (C) SUPSI" << std::endl;
	std::cout << std::endl;

	// Load game logic level from file
	game.loadLogicLevelFromFile("../logicLevels/level1.txt");

	// Init and use the lib:
	engine.init("RushHour", 640, 480);
	engine.setKeyboardCallback(keyboardCallback);
	engine.setMouseCallback(mouseClicks);
	engine.setSpecialCallback(specialCallback);
	engine.setBackgroundColor(0.5f, 0.7f, 1.0f);

	// Define initial transformations
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -40.0f));
	glm::mat4 translation2 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, -40.0f));
	glm::mat4 translation3 = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -10.0f, -40.0f));
	glm::mat4 rotation1 = glm::rotate(glm::mat4(1.0f), glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 transformationCamera1 = (glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 300.0f, 400.0f))) * rotation1;
	glm::mat4 rotation2 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 transformationCamera2 = (glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 400.0f, 0.0f))) * rotation2;


	staticCamera1 = new PerspectiveCamera("cameraStatica1", transformationCamera1, 1.0f, 2000.0f, 45.0f);
	staticCamera2 = new PerspectiveCamera("cameraStatica2", transformationCamera2, 1.0f, 2000.0f, 45.0f);
	dynamicCamera = new PerspectiveCamera("cameraDinamica", defaultDynamicCameraMatrix, 1.0f, 2000.0f, 45.0f);
	auto list = std::make_shared<List>();



	engine.isRunning(true);
	engine.list(list);

	scene = engine.load("../ovo/Livello1.ovo");


	// Decide which node should cast shadows
	Node* node = engine.findNode(scene, "Plane001");
	if (node) {
		Mesh* m = dynamic_cast<Mesh*>(node);
		if (m != 0)
			m->isCastingShadow(false);
	}

	scene->addChild(staticCamera1);
	scene->addChild(dynamicCamera);
	scene->addChild(staticCamera2);
	selectedCamera = staticCamera1;

	while (engine.isRunning())
	{
		engine.update();
		engine.clearWindow();
		// Render stuff…
		engine.begin3D(selectedCamera);
		scene->pass(list);


		engine.end3D();
		engine.begin2D();
		menu(engine);
		engine.end2D();
		engine.swapBuffers();

		if (newCarSelected && !game.isGameWon()) {
			car = engine.getSelectedObject();
			if (car != nullptr && game.matchVehicle(car)) {
				if (!isGameStarted)
					isGameStarted = true;
			}

			newCarSelected = false;
		}

	}
	engine.free();

	// Free memory
	selectedCamera = nullptr;

	// Delete the whole scene including the cameras
	delete scene;

	// Done:
	std::cout << "\n[application terminated]" << std::endl;
	return 0;
}



