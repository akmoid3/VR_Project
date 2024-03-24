/**
 * @file		spotLight.cpp
 * @ingroup Engine
 * @brief	Implementation of the SpotLight class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#include "engine.h"
#define FREEGLUT_STATIC
#include <GL/freeglut.h>


 ////////////////////////////
 // BODY OF CLASS SpotLight//
 ////////////////////////////

/**
 * @brief Constructor for the SpotLight class.
 * @details This constructor initializes the spotlight with a given name, a transformation matrix relative to its parent node, default position, direction, attenuation constants, and a default cutoff angle.
 * @param name The name of the light.
 * @param matrix The initial transformation matrix of the light relative to its parent node.
 */
LIB_API SpotLight::SpotLight(const std::string& name, const glm::mat4& matrix) :
	Light(name, matrix),m_position{glm::vec4{0.0f,0.0f,0.0f,1.0f}},m_attenuationConstant{1.0f},m_attenuationLinear{0.0f},m_attenuationQuadratic{0.0f},
	m_direction{glm::vec3{0.0f,1.0f,0.0f}},m_cutoff{45.0f}
{

}

/**
 * @brief Destructor for the SpotLight class.
 */
LIB_API SpotLight::~SpotLight()
{
}

/**
 * @brief Set the position of the light.
 * @details This method sets the position of the light. The w component of the position is set to 1.0f, indicating that this is a spotlight.
 * The light position is set to object coordinates and is modified by the current transformation matrix as with any other object
 * @param x The x-coordinate of the light's position.
 * @param y The y-coordinate of the light's position.
 * @param z The z-coordinate of the light's position.
 */
void LIB_API SpotLight::position(float x, float y, float z)
{
	m_position = glm::vec4(x, y, z, 1.0f);
}

/**
 * @brief Set the linear attenuation of the light.
 * @param a The linear attenuation of the light.
 */
void LIB_API SpotLight::attenuationLinear(float a)
{
	m_attenuationLinear = a;
}

/**
 * @brief Set the quadratic attenuation of the light.
 * @param a The quadratic attenuation of the light.
 */
void LIB_API SpotLight::attenuationQuadratic(float a)
{
	m_attenuationQuadratic = a;
}

/**
 * @brief Set the cutoff angle of the light.
 * @param cutoff The cutoff angle of the light. Any angle between 0 and 90 degrees. 
 */
void LIB_API SpotLight::cutoff(float cutoff)
{
	m_cutoff = cutoff;
}

/**
 * @brief Set the direction of the light.
 * @param dx The x-component of the light's direction.
 * @param dy The y-component of the light's direction.
 * @param dz The z-component of the light's direction.
 */
void LIB_API SpotLight::direction(float dx, float dy, float dz)
{
	m_direction = glm::vec3(dx, dy, dz);
	
}

/**
 * @brief Set the constant attenuation of the light.
 * @param a The constant attenuation of the light.
 */
void LIB_API SpotLight::attenuationConstant(float a)
{
	m_attenuationConstant = a;
}

/**
 * @brief Render the light.
 * @details This method first calls the render method of the Light class. Then it sets the position, direction, and attenuation properties of the light for rendering, and sets the cutoff angle.
 * @param mat The result of multiplying the inverse of the final camera matrix by the final position matrix in world coordinates of this object.
 * @param flag This parameter is ignored in this implementation. Default is nullptr.
 */
void LIB_API SpotLight::render(const glm::mat4& mat, void* flag)
{
	Engine::getProgramSpot();
	Light::render(mat);
	Shader::getCurrentProgram()->setVec3(Shader::getCurrentProgram()->getParamLocation("lightPosition"), glm::vec3(mat[3])); // Si passa mat[3] che corrisponde alla posizione della luce in eye coordinates. La variabile m_position non viene settata dall'ovoreader, perciò resta il valore di default.
	Shader::getCurrentProgram()->setFloat(Shader::getCurrentProgram()->getParamLocation("lightCutOff"), glm::cos(glm::radians(m_cutoff)));
	Shader::getCurrentProgram()->setVec3(Shader::getCurrentProgram()->getParamLocation("lightSpotDirection"), glm::normalize(glm::mat3(mat) * m_direction));
	//glLightfv(m_lightNumber, GL_SPOT_DIRECTION, glm::value_ptr(m_direction));
	//glLightfv(m_lightNumber, GL_POSITION, glm::value_ptr(m_position));
	//glLightf(m_lightNumber, GL_CONSTANT_ATTENUATION, m_attenuationConstant); 
	//glLightf(m_lightNumber, GL_LINEAR_ATTENUATION, m_attenuationLinear); // default 0
	//glLightf(m_lightNumber, GL_QUADRATIC_ATTENUATION, m_attenuationQuadratic); // default 0
	//glLightfv(m_lightNumber, GL_SPOT_CUTOFF, &m_cutoff);

}
