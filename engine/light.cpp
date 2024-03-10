/**
 * @file		light.cpp
 * @ingroup Engine
 * @brief	Implementation of the Light abstract class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#include "engine.h"
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <iostream>
#include "light.h"

////////////
// STATIC //
////////////

/**
 * @brief Static variable to keep track of the number of Light objects created.
 */
unsigned int Light::cnt = 0;

//////////////////////////
// BODY OF CLASS Light //
//////////////////////////

/**
 * @brief Render the light.
 * @details This method first calls the render method of the Node class. Then it enables the light and sets the ambient, diffuse, and specular properties of the light for rendering.
 * @param m The result of multiplying the inverse of the final camera matrix by the final position matrix in world coordinates of this object.
 * @param flag This parameter is ignored in this implementation. Default is nullptr.
 */
void LIB_API Light::render(const glm::mat4& m, void*)
{
	Node::render(m);
	glEnable(m_lightNumber);
	//glLightfv(m_lightNumber, GL_AMBIENT, glm::value_ptr(m_ambient));
	//glLightfv(m_lightNumber, GL_DIFFUSE, glm::value_ptr(m_diffuse));
	//glLightfv(m_lightNumber, GL_SPECULAR, glm::value_ptr(m_specular));
}

/**
 * @brief Constructor for the Light class.
 * @details This constructor initializes the light with a given name, a transformation matrix relative to it's parent node, a light number of 0, and default ambient, diffuse, and specular properties.
 * @param name The name of the light.
 * @param matrix The initial transformation matrix of the light relative to it's parent node.
 */
//valori di default sono quelli della GL_LIGHT0
LIB_API Light::Light(const std::string& name, const glm::mat4& matrix) :
	Node(name, matrix), m_lightNumber{ 0 }, m_ambient{ glm::vec4{0.0f,0.0f,0.0f,1.f} }, m_diffuse{ glm::vec4{1.0f,1.0f,1.0f,1.0f} }, m_specular{ glm::vec4{1.0f,1.0f,1.0f,1.0f} }
{
	
}

/**
 * @brief Destructor for the Light class.
 */
LIB_API Light::~Light()
{
}

// Settings:

/**
 * @brief Set the ambient property of the light.
 * @param r The red component of the ambient property.
 * @param g The green component of the ambient property.
 * @param b The blue component of the ambient property.
 * @param a The alpha component of the ambient property.
 */
void LIB_API Light::ambient(float r, float g, float b, float a)
{
	m_ambient = glm::vec4{ r, g, b, a };
}

/**
* @brief Set the specular property of the light.
* @param r The red component of the specular property.
* @param g The green component of the specular property.
* @param b The blue component of the specular property.
* @param a The alpha component of the specular property.
*/
void LIB_API Light::specular(float r, float g, float b, float a)
{
	m_specular = glm::vec4{ r, g, b, a };
}

/**
* @brief Set the diffuse property of the light.
* @param r The red component of the diffuse property.
* @param g The green component of the diffuse property.
* @param b The blue component of the diffuse property.
* @param a The alpha component of the diffuse property.
*/
void LIB_API Light::diffuse(float r, float g, float b, float a)
{
	m_diffuse = glm::vec4{ r, g, b, a };

}

/**
* @brief Set the identification light number.
* @param lightNumber The light number.
*/
void LIB_API Light::lightNumber(unsigned int lightNumber)
{
	m_lightNumber = lightNumber;
}


