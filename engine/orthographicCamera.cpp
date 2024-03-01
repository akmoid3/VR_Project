/**
 * @file		orthographicCamera.cpp
 * @ingroup Engine
 * @brief	Implementation of the OrthographicCamera class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#include "engine.h"

//////////////////////////////////////
// BODY OF CLASS OrthographicCamera //
//////////////////////////////////////

/**
* @brief Constructor for OrthographicCamera.
* @param name The name of the camera.
* @param matrix The initial transformation matrix of the camera.
* @param near The near clipping plane distance.
* @param far The far clipping plane distance.
*/
LIB_API OrthographicCamera::OrthographicCamera(const std::string& name, const glm::mat4& matrix,
	float near, float far) :
	Camera(name, matrix, near, far), m_left{ 0.0f }, m_bottom{ 0.0f } {

}

/**
* @brief Destructor for OrthographicCamera.
*/
LIB_API OrthographicCamera::~OrthographicCamera() {}

/**
* @brief Updates the orthographic projection matrix based on the viewport dimensions.
* @param width The width of the viewport.
* @param height The height of the viewport.
*/
void OrthographicCamera::update(int width, int height)
{
	m_projectionMatrix = glm::ortho(m_left, (float)width, m_bottom, (float)height, m_nearPlane, m_farPlane);
}

/**
* @brief Empty method. 
* 
* It is included to satisfy the virtual function declaration in the base class Object.
*/
void LIB_API OrthographicCamera::render(const glm::mat4&, void*)
{
}
