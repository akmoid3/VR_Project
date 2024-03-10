/**
 * @file		engine.h
 * @brief	3D Graphics Engine .dll/.so dynamic library
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

 /**
  * @defgroup Engine Engine
  * @brief This group contains all the classes and functions for the 3D Graphics Engine dynamic library.
  */

#pragma once
#include <string>

#pragma warning (disable : 4251)
 /////////////
 // VERSION //
 /////////////

	 // Generic info:
#define LIB_NAME      "Engine 1.0"  ///< Library credits
#define LIB_VERSION   10            ///< Library version (divide by 10)

#ifdef _WINDOWS 	
	// Export API:
	// Specifies i/o linkage (VC++ spec):
#ifdef ENGINE_EXPORTS
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif      	
#else // Under Linux
#define LIB_API  // Dummy declaration
#endif

#ifdef _DEBUG
#define ENABLE_DEBUG glDebugMessageCallback((GLDEBUGPROC)debugCallback, nullptr); glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS)
#else
#define ENABLE_DEBUG 
#endif


//Include after ifdef _WINDOWS
#include "node.h"
#include "mesh.h"
#include "camera.h"
#include "object.h"
#include "light.h"
#include "directionalLight.h"
#include "spotLight.h"
#include "omniLight.h"
#include "material.h"
#include "orthographicCamera.h"
#include "perspectiveCamera.h"
#include "list.h"
#include "ovoreader.h"
#include "lod.h"
#include "texture.h"
#include "keysDefinitions.h"

#include <memory>


/////////////
// CLASSES //
/////////////

/**
 * @brief Main component of the 3D Graphics Engine API.
 *
 * The Engine class is a singleton that is responsible for initializing and interacting with the OpenGL context and the various engine components. It is the primary class for rendering.
 * The Engine class uses a List for the objects to be rendered. It provides methods for setting up the rendering context, loading assets, managing the render list, handling input, and controlling the render loop. 
 * It also provides access to the currently selected object in the scene, when the mouse object selection method is called.
 * The Engine class also provides methods for setting the background color, clearing the window, swapping buffers, and other tasks necessary for rendering a frame.
 */
class LIB_API Engine
{
	//////////
public:	  //
	//////////	  

	static Engine& instance(void);
	bool init(const std::string& titolo, unsigned int width, unsigned int height);
	void setBackgroundColor(float r, float g, float b);
	void clearWindow();
	void setKeyboardCallback(void (*callback)(unsigned char, int, int));
	void setSpecialCallback(void (*callback)(int, int, int));
	void setMouseCallback(void (*callback)(int, int, int, int));
	void update();
	void swapBuffers();
	bool free();
	void begin3D(Camera* camera);
	void end3D();

	Node* load(std::string);
	void begin2D();
	void end2D();
	void addText(const char text[64], float x, float y);
	std::shared_ptr<List> list() const;
	void list(std::shared_ptr<List>);
	void activateMouseObjectSelection(int mouseX, int mouseY);
	void showFps(float x, float y);
	Node* findNode(Node* node, const std::string& name);

	// Getter/Setter
	int height() const;
	void height(int);
	int width() const;
	void width(int);
	bool initFlag() const;
	void initFlag(bool);
	bool isRunning() const;
	void isRunning(bool);
	Mesh* getSelectedObject();

	// Those are initialized here and are immediately overwritten by the reshape_callback at the application startup
	int m_width{ 0 }; ///< The width of the viewport. Updated by the reshape_callback
	int m_height{ 0 }; ///< The height of the viewport. Updated by the reshape_callback


	///////////
protected:	//
	///////////	

	// Const/dest 
	Engine() {};

	// Methods
	void selectObject();

	///////////
private:	   //
	///////////	   

	// Internal vars:
	static Engine& m_instance; ///< Singleton instance of the Engine.

	bool m_initFlag{ false }; ///< Flag indicating whether the Engine has been initialized.
	bool m_isRunningFlag{ false }; ///< Flag indicating whether the Engine is running.
	std::shared_ptr<List> m_list{ nullptr }; ///< Shared pointer to the List.
	Mesh* currentSelectedObject{ nullptr }; ///< Pointer to the currently selected Mesh, saved after calling void activateMouseObjectSelection(int mouseX, int mouseY);.
};
