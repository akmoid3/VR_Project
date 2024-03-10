/**
 * @file		omniLight.cpp
 * @ingroup Engine
 * @brief	Implementation of the OmniLight class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#include "engine.h"
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

 ////////////////////////////////////
 // BODY OF CLASS OmniLight        //
 ////////////////////////////////////

/**
* @brief Constructor for the OmniLight class.
* @details This constructor initializes the omnidirectional light with a given name, a transformation matrix relative to its parent node, default attenuation constants, the special cutoff value of 180.0f, and a default position.
* @param name The name of the light.
* @param matrix The initial transformation matrix of the light relative to its parent node.
*/
LIB_API OmniLight::OmniLight(const std::string& name, const glm::mat4& matrix) :
	Light(name, matrix),m_attenuationConstant{1.0f},m_attenuationLinear{0.0f},m_attenuationQuadratic{0.0f},m_cutoff{180.0f},m_position{glm::vec4{0.0f,0.0f,0.0f,1.0f}}
{
	
}

/**
 * @brief Destructor for the OmniLight class.
 */
LIB_API OmniLight::~OmniLight()
{
}

/**
* @brief Set the position of the light.
* @details This method sets the position of the light. The w component of the position is set to 1.0f, indicating that this is an omnidirectional light. 
* The light position is set to object coordinates and is modified by the current transformation matrix as with any other object
* @param x The x-coordinate of the light's position.
* @param y The y-coordinate of the light's position.
* @param z The z-coordinate of the light's position.
*/
void LIB_API OmniLight::position(float x, float y, float z)
{
	m_position = glm::vec4(x, y, z, 1.0f);
}

/**
* @brief Set the constant attenuation of the light.
* @param a The constant attenuation of the light.
*/
void LIB_API OmniLight::attenuationConstant(float a)
{
	m_attenuationConstant = a;
	
}

/**
* @brief Set the linear attenuation of the light.
* @param a The linear attenuation of the light.
*/
void LIB_API OmniLight::attenuationLinear(float a)
{
	m_attenuationLinear = a;
}

/**
* @brief Set the quadratic attenuation of the light.
* @param a The quadratic attenuation of the light.
*/
void LIB_API OmniLight::attenuationQuadratic(float a)
{
	m_attenuationQuadratic = a;
}

/**
* @brief Set the cutoff value of the light.
* @param a The cutoff value of the light. Should be set to the special value of 180.f, indicating that this is an omnidirectional light.
*/
void LIB_API OmniLight::cutoff(float a)
{
	m_cutoff = a;
}

/**
* @brief Render the light.
* @details This method first calls the render method of the Light class. Then it sets the position and attenuation properties of the light for rendering, and sets the cutoff value.
* @param mat The result of multiplying the inverse of the final camera matrix by the final position matrix in world coordinates of this object.
* @param flag This parameter is ignored in this implementation. Default is nullptr.
*/
void LIB_API OmniLight::render(const glm::mat4& mat, void* flag)
{
	Light::render(mat);
	//glLightfv(m_lightNumber, GL_POSITION, glm::value_ptr(m_position));
	//glLightf(m_lightNumber, GL_CONSTANT_ATTENUATION, m_attenuationConstant); // default 1
	//glLightf(m_lightNumber, GL_LINEAR_ATTENUATION, m_attenuationLinear); // default 0
	//glLightf(m_lightNumber, GL_QUADRATIC_ATTENUATION, m_attenuationQuadratic); // default 0
	//glLightfv(m_lightNumber, GL_SPOT_CUTOFF, &m_cutoff);
}
