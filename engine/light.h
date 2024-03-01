/**
 * @file		light.h
 * @ingroup Engine
 * @brief	Declaration of the Light abstract class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "node.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
// C/C++:
#include <string>
#include <vector>


/////////////
// CLASSES //
/////////////

/**
 * @brief Abstract class that represents a light source in a 3D scene.
 * @details This class extends the Node class. It provides methods for setting the ambient, diffuse, and specular properties of the light, setting the position of the light, keeping track of the number of lights in the scene, and rendering the light.
 */
class LIB_API Light : public Node
{
	//////////
public:    //
	//////////	   

	Light(const std::string& name, const glm::mat4& matrix);
	~Light();


	void ambient(float r, float g, float b, float a);
	void diffuse(float r, float g, float b, float a);
	void specular(float r, float g, float b, float a);
	void lightNumber(unsigned int lightNumber);

	/**
	* @brief Virtual method. Set the position of the light. 
	* The light position is set to object coordinates and is modified by the current transformation matrix as with any other object
	* @param x The x-coordinate of the light's position.
	* @param y The y-coordinate of the light's position.
	* @param z The z-coordinate of the light's position.
	*/
	virtual void position(float x, float y, float z) = 0;

	// Ereditato tramite Node
	void render(const glm::mat4&, void* = nullptr) override;


	///////////
protected:	//
	///////////	  

	// Internal vars:
	unsigned int m_lightNumber; ///< The number used to identify this light object.
	static unsigned int cnt; ///< A counter for the number of lights in the scene.
	glm::vec4 m_ambient; ///< The ambient property of the light.
	glm::vec4 m_diffuse; ///< The diffuse property of the light.
	glm::vec4 m_specular; ///< The specular property of the light.
};
