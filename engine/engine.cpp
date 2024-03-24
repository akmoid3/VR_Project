/**
 * @file		engine.cpp
 * @ingroup Engine
 * @brief	Implementation of the 3D Graphics Engine .dll/.so dynamic library
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */



 //////////////
 // #INCLUDE //
 //////////////

	// Library main include:
#include "engine.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

// C/C++:
#include <iostream>
#include <string>
#include <memory>

// Glew (include it before GL.h):
#define GLEW_STATIC
#include <GL/glew.h>


#define FREEGLUT_STATIC
#include <GL/freeglut.h>

#define FREEIMAGE_LIB // Static lib only
#include <FreeImage.h>


////////////
// STATIC //
////////////

static glm::mat4 c_inverse; ///< Inverse of the camera final matrix.
Texture* Engine::whitePixel = nullptr;
/////////////
// GLOBALS //
/////////////
int windowId;
Ovoreader ovoreader;
Camera* ortho = new OrthographicCamera("camera", glm::mat4{ 1.0f }, -1.0f, 1.0f);
bool objectSelection{ false };
int mouseX = 0;
int mouseY = 0;
Camera* currentCamera = nullptr;

// FPS:
unsigned int fps = 0;
unsigned int frames = 0;

// Shaders:
Shader* vs = nullptr;
Shader* fs = nullptr;
Shader* fsOmni = nullptr;
Shader* fsSpot = nullptr;
Shader* fsDir = nullptr;
Shader* progOmni = nullptr; 
Shader* progDir = nullptr; 
Shader* progSpot = nullptr; 
int projLoc = -1; // -1 means 'not assigned', as 0 is a valid location, per sapere dove trovare le variabili
int mvLoc = -1;

Shader* dirShader = nullptr;
Shader* spotShader = nullptr;



/////////////
// SHADERS //
/////////////

////////////////////////////
const char* vertShader = R"(
   #version 440 core

   // Uniforms:
   uniform mat4 projection;
   uniform mat4 modelview;
   uniform mat3 normalMatrix;
   

   // Attributes:
   layout(location = 0) in vec3 in_Position;
   layout(location = 1) in vec3 in_Normal;
   layout(location = 2) in vec2 in_TexCoord;

   // Varying:
   out vec4 fragPosition;
   out vec3 normal;   
   out vec2 texCoord;

   void main(void)
   {
	  texCoord = in_TexCoord;
      fragPosition = modelview * vec4(in_Position, 1.0f);
      gl_Position = projection * fragPosition;      
      normal = normalMatrix * in_Normal;
   }
)";

////////////////////////////
const char* fragShaderOmni = R"(
   #version 440 core

   in vec4 fragPosition;
   in vec3 normal;   
   in vec2 texCoord; 

   out vec4 fragOutput;

   // Material properties:
   uniform vec3 matEmission;
   uniform vec3 matAmbient;
   uniform vec3 matDiffuse;
   uniform vec3 matSpecular;
   uniform float matShininess;

   // Light properties:
   uniform vec3 lightPosition; 
   uniform vec3 lightAmbient; 
   uniform vec3 lightDiffuse; 
   uniform vec3 lightSpecular;

   // Texture mapping:
   layout(binding = 0) uniform sampler2D texSampler;

   void main(void)
   {      
	  // Texture element:
      vec4 texel = texture(texSampler, texCoord);

      // Ambient term:
      vec3 fragColor = matEmission + matAmbient * lightAmbient;

      // Diffuse term:
      vec3 _normal = normalize(normal);
      vec3 lightDirection = normalize(lightPosition - fragPosition.xyz);      
      float nDotL = dot(lightDirection, _normal);   
      if (nDotL > 0.0f)
      {
         fragColor += matDiffuse * nDotL * lightDiffuse;
      
         // Specular term:
         vec3 halfVector = normalize(lightDirection + normalize(-fragPosition.xyz));                     
         float nDotHV = dot(_normal, halfVector);         
         fragColor += matSpecular * pow(nDotHV, matShininess) * lightSpecular;
      } 
      // Final color:
      fragOutput = texel * vec4(fragColor, 1.0f);
   }
)";

////////////////////////////
const char* fragShaderDir = R"(
   #version 440 core

// Varying variables from the vertex shader:
in vec4 fragPosition;
in vec3 normal;
in vec2 texCoord; 

out vec4 fragOutput;

// Material properties:
uniform vec3 matEmission;
uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

// Light properties:
uniform vec3 lightPosition; 
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

// Texture mapping:
layout(binding = 0) uniform sampler2D texSampler;

void main(void)
{
	// Texture element:
      vec4 texel = texture(texSampler, texCoord);

    // Emission and ambient:
    vec3 fragColor = matEmission + matAmbient * lightAmbient;

    // Diffuse term:
    vec3 _normal = normalize(normal);
	 vec3 lightDirection = normalize(lightPosition); 
    float nDotL = dot(_normal, lightDirection);

    if (nDotL > 0.0) {
        fragColor += matDiffuse * nDotL * lightDiffuse;

       // Specular term:
			vec3 halfVector = normalize(lightDirection + normalize(-fragPosition.xyz));                     
         float nDotHV = dot(_normal, halfVector);         
         fragColor += matSpecular * pow(nDotHV, matShininess) * lightSpecular;
    }

    fragOutput = texel * vec4(fragColor, 1.0);
}
)";

////////////////////////////
const char* fragShaderSpot = R"(
   #version 440 core

   in vec4 fragPosition;
   in vec3 normal;   
   in vec2 texCoord; 

   out vec4 fragOutput;

   // Material properties:
   uniform vec3 matEmission;
   uniform vec3 matAmbient;
   uniform vec3 matDiffuse;
   uniform vec3 matSpecular;
   uniform float matShininess;

   // Light properties:
   uniform vec3 lightPosition; 
   uniform vec3 lightAmbient; 
   uniform vec3 lightDiffuse; 
   uniform vec3 lightSpecular;
   uniform vec3 lightSpotDirection;
   uniform float lightCutOff;

   // Texture mapping:
   layout(binding = 0) uniform sampler2D texSampler;

   void main(void)
   {      

	  // Texture element:
      vec4 texel = texture(texSampler, texCoord);

      // Ambient term:
      vec3 fragColor = matEmission + matAmbient * lightAmbient;

      // Diffuse term:

      vec3 _normal = normalize(normal);
      vec3 lightDirection = normalize(lightPosition - fragPosition.xyz);      
      float nDotL = dot(lightDirection, _normal);   
      if (nDotL > 0.0f)
      {
         fragColor += matDiffuse * nDotL * lightDiffuse;
      
         // Specular term:
         vec3 halfVector = normalize(lightDirection + normalize(-fragPosition.xyz));                     
         float nDotHV = dot(_normal, halfVector);         
         fragColor += matSpecular * pow(nDotHV, matShininess) * lightSpecular;

			if(dot(-lightDirection, lightSpotDirection) < lightCutOff) fragColor = vec3(0.0f);

      } 
      
      // Final color:
      fragOutput = texel * vec4(fragColor, 1.0f);
   }
)";

////////////////////////////
/*
const char* fragShaderSpot = R"(
   #version 440 core

   in vec4 fragPosition;
   in vec3 normal;   
   
   out vec4 fragOutput;

   // Material properties:
   uniform vec3 matEmission;
   uniform vec3 matAmbient;
   uniform vec3 matDiffuse;
   uniform vec3 matSpecular;
   uniform float matShininess;

   // Light properties:
   uniform vec3 lightPosition; 
   uniform vec3 lightAmbient; 
   uniform vec3 lightDiffuse; 
   uniform vec3 lightSpecular;
   uniform vec3 lightDir;
   uniform float lightCutOff;

   void main(void)
   {      
      // Ambient term:
      vec3 fragColor = matEmission + matAmbient * lightAmbient;

      // Diffuse term:
      vec3 _normal = normalize(normal);
      vec3 lightDirection = normalize(lightPosition - fragPosition.xyz);      
      float nDotL = dot(lightDirection, _normal);   
	
		// Spotlight effect:
		float cosAngle = dot(-lightDirection, lightDir);
      if (cosAngle > cos(lightCutOff) && nDotL > 0.0f)
      {
          fragColor += matDiffuse * nDotL * lightDiffuse;
          
          //Specular term:
          vec3 viewDir = normalize(-fragPosition.xyz);
          vec3 halfVector = normalize(lightDirection + viewDir);
          float nDotHV = dot(_normal, halfVector);
          fragColor += matSpecular * pow(max(0.0, nDotHV), matShininess) * lightSpecular;
      }      

		// Final color:
      fragOutput = vec4(fragColor, 1.0f);
	}
)";*/

//////////////
// DLL MAIN //
//////////////

#ifdef _WINDOWS
#include <Windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * DLL entry point. Avoid to rely on it for easier code portability (Linux doesn't use this method).
 * @param instDLL handle
 * @param reason reason
 * @param _reserved reserved
 * @return true on success, false on failure
 */
int APIENTRY DllMain(HANDLE instDLL, DWORD reason, LPVOID _reserved)
{
	// Check use:
	switch (reason)
	{
		///////////////////////////
	case DLL_PROCESS_ATTACH: //
		break;


		///////////////////////////
	case DLL_PROCESS_DETACH: //
		break;
	}

	// Done:
	return true;
}
#endif



///////////////////////////
// BODY OF CLASS Engine  //
///////////////////////////

/**
 * @brief Returns the singleton instance of the Engine.
 * @return Engine& Singleton instance of the Engine
 */
LIB_API Engine& Engine::instance() {
	static Engine instance;
	return instance;
}

// GETTER/SETTER

/**
 * @brief Returns the height of the viewport.
 * @return int Height of the viewport
 */
int LIB_API Engine::height() const
{
	return m_height;
}

/**
 * @brief Sets the height of the viewport.
 * @param height Height of the viewport
 */
void LIB_API Engine::height(int height)
{
	m_height = height;
}

/**
 * @brief Returns the width of the viewport.
 * @return int Width of the viewport
 */
int LIB_API Engine::width() const
{
	return m_width;
}

/**
 * @brief Sets the width of the viewport.
 * @param width Width of the viewport
 */
void LIB_API Engine::width(int width)
{
	m_width = width;
}

/**
 * @brief Returns the initialization flag.
 * @return bool Returns true if the engine has been initialized, false otherwise.
 */
bool LIB_API Engine::initFlag() const
{
	return m_initFlag;
}

/**
 * @brief Sets the initialization flag.
 * @param initFlag Boolean value representing the initialization state of the engine. Set to true if the engine has been initialized, false otherwise.
 */
void LIB_API Engine::initFlag(bool initFlag)
{
	m_initFlag = initFlag;
}

/**
 * @brief Returns the running state of the engine.
 * @return bool Returns true if the engine is currently running, false otherwise.
 */
bool LIB_API Engine::isRunning() const
{
	return m_isRunningFlag;
}

/**
 * @brief Sets the running flag.
 * @param isRunningFlag Boolean value representing the running state of the engine. Set to true if the engine is running, false otherwise.
 */
void LIB_API Engine::isRunning(bool isRunningFlag)
{
	m_isRunningFlag = isRunningFlag;
}

/**
 * @brief Returns the currently selected object.
 * @details This function should be called after `Engine::activateMouseObjectSelection()`.
 * @return Mesh* Pointer to the currently selected Mesh.
 */
Mesh LIB_API* Engine::getSelectedObject()
{
	return currentSelectedObject;
}

/**
 * @brief Returns the List.
 * @return std::shared_ptr<List> Shared pointer to the List
 */
std::shared_ptr<List> LIB_API Engine::list() const {
	return m_list;
}

/**
 * @brief Sets the List.
 * @param list Shared pointer to the List
 */
void LIB_API Engine::list(std::shared_ptr<List> list) {
	m_list = list;
	m_list->setProgram(progOmni);
}

///////////////
// CALLBACKS //
///////////////

/**
 * @brief This callback is invoked each time the window gets resized (and once also when created).
 *
 * This function is registered as a callback in the Engine::init() method.
 * @param width new window width
 * @param height new window height
 */
static void reshapeCallback(int width, int height) {
	Engine::instance().m_width = width;
	Engine::instance().m_height = height;

	glViewport(0, 0, width, height);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	  
/**
 * Debug message callback for OpenGL. See https://www.opengl.org/wiki/Debug_Output
 */
void __stdcall debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	std::cout << "OpenGL says: \"" << std::string(message) << "\"" << std::endl;
}

/**
 * @brief This callback is invoked when the mainloop is left and before the context is released.
 *
 * This function is registered as a callback in the Engine::init() method.
 */
void LIB_API closeCallback()
{
	std::cout << "[close func invoked]" << std::endl;

	// Release OpenGL resources while the context is still available:
	for (auto& tex : ovoreader.textures()) {
		tex.second->deleteTexture();
	}
	delete progOmni;
	delete fsOmni;
	delete vs;
}


/**
 * @brief This callback is invoked once each second.
 *
 * This function is registered as a callback in the Engine::init() method. It is used for calculating the FPS.
 * @param value passepartout value
 */
void LIB_API timerCallback(int value)
{
	// Update values:
	fps = frames;
	frames = 0;

	// Register the next update:
	glutTimerFunc(1000, timerCallback, 0);
	printf("%d \n", fps);
}

/**
 * @brief This is the main rendering routine automatically invoked by FreeGLUT.
 * In this implementation, it is intentionally left empty.
 *
 * This function is registered as a callback in the Engine::init() method.
 */
static void displayCallback() {
	//deve restare vuota
}


// SETTERS FOR THE CALLBACKS

/**
 * @brief Sets the keyboard callback.
 * @details This function allows for passing or defining a custom keyboard callback.
 * The provided callback function will be called when keyboard events occur.
 * @param callback Function pointer to the keyboard callback
 */
void LIB_API Engine::setKeyboardCallback(void (*callback)(unsigned char, int, int)) {
	glutKeyboardFunc(callback);
}

/**
 * @brief Sets the special callback.
 * @details This function allows for passing or defining a custom special callback.
 * The provided callback function will be called when special events occur.
 * @param callback Function pointer to the special callback
 */
void LIB_API Engine::setSpecialCallback(void(*callback)(int, int, int))
{
	glutSpecialFunc(callback);
}

/**
 * @brief Sets the mouse callback.
 * @details This function allows for passing or defining a custom mouse callback.
 * The provided callback function will be called when mouse events occur.
 * @param callback Function pointer to the mouse callback
 */
void LIB_API Engine::setMouseCallback(void(*callback)(int, int, int, int))
{
	glutMouseFunc(callback);
}

/////////////
// METHODS //
/////////////  

/**
 * @brief Activates mouse object selection.
 * @details This function activates the selection of objects in the scene through a mouse click on the window. It triggers the `Engine::selectObject()` method within the `Engine::end3D()` method.
 * @param _mouseX The x-coordinate of the mouse cursor.
 * @param _mouseY The y-coordinate of the mouse cursor.
 */
void LIB_API Engine::activateMouseObjectSelection(int _mouseX, int _mouseY) {
	mouseX = _mouseX;
	mouseY = _mouseY;
	objectSelection = true;
}

/**
 * @brief Finds a Node in a scene.
 * @details This function performs a recursive search through a tree hierarchy of nodes (a scene), starting from the provided root node. It looks for a node with the specified name and returns a pointer to it if found. If no node with the given name is found, it returns nullptr.
 * @param node Pointer to the root Node from where the search should start.
 * @param name Name of the Node to find.
 * @return Node* Pointer to the found Node. If no Node with the given name is found, returns nullptr.
 */
Node LIB_API* Engine::findNode(Node* node, const std::string& name)
{
	if (node->name().compare(name) == 0)
		return node;

	for (auto& child : node->children()) {
		Node* result = findNode(child, name);
		if (result != nullptr) {
			// Node found 
			return result;
		}
	}

	// Node not found 
	return nullptr;
}


/**
 * @brief Initialization method for the Engine.
 * @details This method should be called before any other function. It initializes the OpenGL context, sets up the window and viewport, and enables various OpenGL features.
 * @param titolo The title of the window.
 * @param width The width of the window.
 * @param height The height of the window.
 * @return bool Returns true on success, false on error.
 */
bool LIB_API Engine::init(const std::string& titolo, unsigned int width, unsigned int height)
{
	// Prevent double init:
	if (m_initFlag)
	{
		std::cout << "ERROR: class already initialized" << std::endl;
		return false;
	}

	// Init context:
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Init GLUT and set some context flags:
	glutInitContextVersion(4, 4);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_DEBUG); // <-- Debug flag required by the OpenGL debug callback  

	glutInitWindowPosition(100, 100);

	// FreeGLUT can parse command-line params, in case:
	int argc = 0;
	char* argv[] = { nullptr };
	glutInit(&argc, argv);

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Set window size
	glutInitWindowSize(width, height);

	// Create the window with a specific title:   
	windowId = glutCreateWindow(titolo.c_str());

	// Init all available OpenGL extensions:
	// Init Glew (*after* the context creation):   
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "[ERROR] glewok" << glewGetErrorString(error) << std::endl;
		return -1;
	}
	else
		if (GLEW_VERSION_4_4)
			std::cout << "Driver supports OpenGL 4.4\n" << std::endl;
		else
		{
			std::cout << "[ERROR] OpenGL 4.4 not supported\n" << std::endl;
			return -1;
		}
	// Register OpenGL debug callback:

	ENABLE_DEBUG;

	// Log context properties:
	std::cout << "OpenGL properties:" << std::endl;
	std::cout << "   Vendor . . . :  " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "   Driver . . . :  " << glGetString(GL_RENDERER) << std::endl;

	int oglVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &oglVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &oglVersion[1]);
	std::cout << "   Version  . . :  " << glGetString(GL_VERSION) << " [" << oglVersion[0] << "." << oglVersion[1] << "]" << std::endl;

	int oglContextProfile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &oglContextProfile);
	if (oglContextProfile & GL_CONTEXT_CORE_PROFILE_BIT)
		std::cout << "                :  " << "Core profile" << std::endl;
	if (oglContextProfile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		std::cout << "                :  " << "Compatibility profile" << std::endl;

	int oglContextFlags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &oglContextFlags);
	if (oglContextFlags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
		std::cout << "                :  " << "Forward compatible" << std::endl;
	if (oglContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
		std::cout << "                :  " << "Debug flag" << std::endl;
	if (oglContextFlags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT)
		std::cout << "                :  " << "Robust access flag" << std::endl;
	if (oglContextFlags & GL_CONTEXT_FLAG_NO_ERROR_BIT)
		std::cout << "                :  " << "No error flag" << std::endl;

	std::cout << "   GLSL . . . . :  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << std::endl;
	// Register callbacks
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(displayCallback);
	glutCloseFunc(closeCallback);
	glutTimerFunc(1000, timerCallback, 0);

	// Tell OpenGL that you want to use vertex arrays for the given attributes:
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Enable zeta buffer
	glEnable(GL_DEPTH_TEST);

	// In case there is some scaling
	glEnable(GL_NORMALIZE);

	// Activate local view position
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

	// Activate face culling
	glEnable(GL_CULL_FACE);

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Enable for the multipass rendering
	glDepthFunc(GL_LEQUAL);

	// Init FreeImage:
	FreeImage_Initialise(); // Static lib only


	// Call this after creating your OpenGL context

	// Check OpenGL version:
	std::cout << "OpenGL context" << std::endl;
	std::cout << "   version  . . : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "   vendor . . . : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "   renderer . . : " << glGetString(GL_RENDERER) << std::endl;


	// White pixel texture
	whitePixel = new Texture("pave.dds");
	whitePixel->load();
	
	// Compile vertex shader:
	vs = new Shader("vertex_shader");
	vs->loadFromMemory(Shader::TYPE_VERTEX, vertShader);

	// Compile fragment shader:
	fsOmni = new Shader("fragment_shader_omni");
	fsOmni->loadFromMemory(Shader::TYPE_FRAGMENT, fragShaderOmni);

	// Compile fragment shader:
	fsSpot = new Shader("fragment_shader_spot");
	fsSpot->loadFromMemory(Shader::TYPE_FRAGMENT, fragShaderSpot);

	// Compile fragment shader:
	fsDir = new Shader("fragment_shader_dir");
	fsDir->loadFromMemory(Shader::TYPE_FRAGMENT, fragShaderDir);

	// Setup shader program:
	// bisognerebbe fare qualche controllo

	// Setup shader program omni
	progOmni = new Shader("program_omni");
	progOmni->build(vs, fsOmni);
	Shader::setCurrentProgram(progOmni);
	progOmni->bind(0, "in_Position");
	progOmni->bind(1, "in_Normal");
	progOmni->bind(2, "in_TexCoord");
	
	// Setup shader program dir
	progDir = new Shader("program_dir");
	progDir->build(vs, fsDir);
	Shader::setCurrentProgram(progDir);
	progDir->bind(0, "in_Position");
	progDir->bind(1, "in_Normal");
	progDir->bind(2, "in_TexCoord");

	// Setup shader program spot
	progSpot = new Shader("program_spot");
	progSpot->build(vs, fsSpot);
	Shader::setCurrentProgram(progSpot);
	progSpot->bind(0, "in_Position");
	progSpot->bind(1, "in_Normal");
	progSpot->bind(2, "in_TexCoord");
	
	// Get shader variable locations:
	//projLoc = progOmni->getParamLocation("projection");
	mvLoc = progOmni->getParamLocation("modelview");

	

	// Done:
	m_initFlag = true;
	return true;
}

/**
 * @brief Sets the background color for the window.
 * @param r The red component of the color.
 * @param g The green component of the color.
 * @param b The blue component of the color.
 */
void LIB_API Engine::setBackgroundColor(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f); // RGBA components
}

/**
 * @brief Updates the Engine by entering the main FreeGLUT processing loop.
 */
void LIB_API Engine::update() {
	// Enter the main FreeGLUT processing loop:     
	glutMainLoopEvent();
}

/**
 * @brief Clears the window by clearing the screen buffer.
 */
void LIB_API Engine::clearWindow() {

	// Clear the screen:         
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/**
 * @brief Swaps the buffers, updates the framerate, and forces a rendering refresh.
 */
void LIB_API Engine::swapBuffers() {
	// Framerate
	frames++;

	// Swap this context's buffer:     
	glutSwapBuffers();

	// Force rendering refresh
	glutPostWindowRedisplay(windowId);
}


/**
 * @brief Deinitialization method.
 * @details This method releases resources, deletes textures and materials objects and deinitializes the Engine. It should be called when you're done using the Engine.
 * @return true on success, false on error
 */
bool LIB_API Engine::free()
{
	// Release FreeImage
	FreeImage_DeInitialise(); // Static lib only


	// Delete objects
	delete ortho;

	ovoreader.deleteTexturesAndMaterials();

	// Really necessary?
	if (!m_initFlag)
	{
		std::cout << "ERROR: class not initialized" << std::endl;
		return false;
	}

	// Done:
	m_initFlag = false;
	return true;
}

/**
 * @brief Begins 3D rendering with a specified camera.
 * @param camera The camera to use for rendering.
 * @details This function sets up the necessary OpenGL state for 3D rendering.
 * It updates the camera with the current window dimensions, loads the camera's projection matrix into the OpenGL state, and calculates the inverse of the camera's final transformation matrix.
 */
void LIB_API Engine::begin3D(Camera* camera) {

	//glMatrixMode(GL_PROJECTION);
	camera->update(m_width, m_height);
	currentCamera = camera;
	//shader->setMatrix(projLoc, camera->projectionMatrix());
	//glLoadMatrixf(glm::value_ptr(camera->projectionMatrix()));
	c_inverse = inverse(camera->getFinalMatrix());

}


/**
 * @brief Selects an object in the scene using color picking.
 * @details This function is activated when `Engine::activateMouseObjectSelection()` is called and is used within the `Engine::end3D()` method.
 * It uses color picking to select objects in the window. The function first clears the back buffer and disables lighting and texture mapping.
 * It then reads the pixel at the mouse click coordinates and reconstructs the ID of the selected object. After finding the matching object in the list, it can be retrieved using Engine::getSelectedObject().
 * Finally, it clears the back buffer again and enables lighting and texture mapping for default rendering.
 */
void LIB_API Engine::selectObject() {

	// Clear the back buffer 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Disable lighting and texture mapping
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	m_list->render(c_inverse, (void*)objectSelection);

	// Useful for debugging
	// glutSwapBuffers();
	// Sleep(5000);

	// Read pixel at mouse click coordinates
	unsigned char pixel[4];
	glReadPixels(mouseX, Engine::instance().m_height - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	// Forcing alpha channel to 0 to avoid reading issues
	pixel[3] = 0;
	std::cout << "mouseX: " << mouseX << std::endl;
	std::cout << "mouseY: " << Engine::instance().m_height - mouseY << std::endl;


	// Reconstruct the 32-bit unsigned int ID
	unsigned int idReconstructed;
	memcpy(&idReconstructed, pixel, sizeof(idReconstructed));
	std::cout << "Reconstructed ID in engine: " << idReconstructed << std::endl;


	// Search list for match
	std::string objectName;
	for (const auto& pair : m_list->getList()) {
		Object* obj = pair.first;
		unsigned int id = obj->id();
		if (id == idReconstructed) {
			Mesh* mesh = dynamic_cast<Mesh*>(obj);
			if (mesh == 0) {
				break;
			}
			currentSelectedObject = mesh;
			std::cout << "Selected object name: " << mesh->name() << std::endl;
		}
	}


	// Clear the back buffer again before default rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable lighting and texture mapping for default rendering
	//glEnable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);

}

/**
 * @brief Ends 3D rendering.
 *
 * This function handles object selection and default rendering. It is the function that finally renders the objects on the screen.
 */
void LIB_API Engine::end3D()
{
	/*
	// Object selection
	if (objectSelection) {
		selectObject();
		objectSelection = false;
	}


		// Default rendering
	m_list->render(c_inverse, (void*)objectSelection);
	m_list->resetList();
	*/


	for (unsigned int l = 0; l < m_list->getNrOfLights(); l++) {
	//for (unsigned int l = 1; l <2; l++) {
		// Enable addictive blending from light 1 on:
		if (l == 1)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}

		// Render one light at time
		std::pair<Object*, glm::mat4> element = m_list->getElem(l);
		Object* obj = element.first;
		glm::mat4 mat = element.second;
		obj->render(c_inverse * mat, nullptr); // FLAGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG????????????????????????????????????????????????????????????????????????!
		Shader::getCurrentProgram()->setMatrix(Shader::getCurrentProgram()->getParamLocation("projection"), currentCamera->projectionMatrix());
		
		// Render meshes
		m_list->renderMeshes(c_inverse, nullptr);
	}

	// Disable blending, in case we used it:
	if (m_list->getNrOfLights() > 1)
		glDisable(GL_BLEND);

	m_list->resetList();
}

void LIB_API Engine::getProgramSpot()
{	
	Shader::setCurrentProgram(progSpot);
}

void LIB_API Engine::getProgramOmni()
{
	Shader::setCurrentProgram(progOmni);
}

void LIB_API Engine::getProgramDirect()
{
	Shader::setCurrentProgram(progDir);
}

Texture LIB_API * Engine::getWhitePixel()
{
	return whitePixel;
}

/**
 * @brief Loads a scene from an OVO file.
 *
 * @param fileName The name of the OVO file to load.
 * @return Node* A pointer to the loaded node.
 */
Node LIB_API* Engine::load(std::string fileName)
{
	return ovoreader.load(fileName);
}

/**
 * @brief Begins 2D rendering, used for printing text on the screen.
 *
 * This function sets the orthographic projection and disables lighting.
 */
void LIB_API Engine::begin2D()
{
	// Set orthographic projection:
	//glMatrixMode(GL_PROJECTION);
	ortho->update(m_width, m_height);
	//progOmni->setMatrix(projLoc, ortho->projectionMatrix());
	//glLoadMatrixf(glm::value_ptr(ortho->projectionMatrix()));
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
	//progOmni->setMatrix(mvLoc, glm::mat4(1.0f));

	// Disable lighting before rendering white 2D text:
	//glDisable(GL_LIGHTING);
	//glDisable(GL_TEXTURE_2D);
	//glColor3f(1.0f, 1.0f, 1.0f);
}

/**
 * @brief Ends 2D rendering, after text has been printed on screen.
 *
 * This function re-enables lighting.
 */
void LIB_API Engine::end2D()
{
	// Redo ligting:
	//glEnable(GL_LIGHTING);
}

/**
 * @brief Adds text to the screen.
 *
 * This function should be called between Engine::begin2D() and Engine::end2D().
 *
 * @param text The text to add.
 * @param x The x-coordinate for the text.
 * @param y The y-coordinate for the text.
 */
void LIB_API Engine::addText(const char text[64], float x, float y)
{
	//glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*)text);
}

/**
 * @brief Shows the frames per second (FPS) on the screen.
 *
 * This function should be called between Engine::begin2D() and Engine::end2D().
 * @param x The x-coordinate for the FPS text on screen.
 * @param y The y-coordinate for the FPS text on screen.
 */
void LIB_API Engine::showFps(float x, float y) {
	char text[64];
	sprintf(text, "FPS: %d", fps);
	addText(text, x, y);
}






