/**
 * @file		directionalLight.cpp
 * @ingroup Engine
 * @brief	Implementation of the DirectionalLight class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#include "engine.h"
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

 ////////////////////////////////////
 // BODY OF CLASS DirectionalLight //
 ////////////////////////////////////

/**
* @brief Constructor for the DirectionalLight class.
* @details This constructor initializes the directional light with a given name, a transformation matrix relative to its parent node, and a default position.
* @param name The name of the light.
* @param matrix The initial transformation matrix of the light relative to its parent node.
*/
LIB_API DirectionalLight::DirectionalLight(const std::string& name, const glm::mat4& matrix) :
	Light(name, matrix), m_position{ glm::vec4{0.0f,0.0f,1.0f,0.0f} }
{

}

/**
 * @brief Destructor for the DirectionalLight class.
 */
LIB_API DirectionalLight::~DirectionalLight()
{
}

/**
* @brief Set the position of the light.
* @details This method sets the position of the light. The w component of the position is set to 0.0f, indicating that this is a directional light.
* The light position is set to object coordinates and is modified by the current transformation matrix as with any other object
* @param x The x-coordinate of the light's position.
* @param y The y-coordinate of the light's position.
* @param z The z-coordinate of the light's position.
*/
void LIB_API DirectionalLight::position(float x, float y, float z)
{
	m_position = glm::vec4(x, y, z, 0.0f);

}

/**
* @brief Render the light.
* @details This method first calls the render method of the Light class. Then it sets the position of the light for rendering.
* @param m The result of multiplying the inverse of the final camera matrix by the final position matrix in world coordinates of this object.
* @param flag This parameter is ignored in this implementation. Default is nullptr.
*/
void LIB_API DirectionalLight::render(const glm::mat4& mat, void* flag)
{
	Engine::getProgramDirect();
	Light::render(mat);
	Shader::getCurrentProgram()->setVec3(Shader::getCurrentProgram()->getParamLocation("lightPosition"), glm::vec3(mat[3]));

}
