/**
 * @file		perspectiveCamera.cpp
 * @ingroup Engine
 * @brief	Implementation of the PerspectiveCamera class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#include "engine.h"
#include <iostream>

 //////////////////////////////////////
 // BODY OF CLASS PerspectiveCamera  //
 //////////////////////////////////////

/**
* Constructor for the PerspectiveCamera class.
*
* @param name The name of the camera.
* @param matrix The initial transformation matrix.
* @param near The near clipping plane.
* @param far The far clipping plane.
* @param angle The field of view angle.
*/
LIB_API PerspectiveCamera::PerspectiveCamera(const std::string& name, const glm::mat4& matrix, float near,
	float far, float angle) :
	Camera(name, matrix, near, far), m_angle{ angle } {

}

/**
* Destructor for PerspectiveCamera.
*/
LIB_API PerspectiveCamera::~PerspectiveCamera() {}


/**
* Updates the perspective projection matrix based on the viewport dimensions.
* @param width The width of the viewport.
* @param height The height of the viewport.
*/
void LIB_API PerspectiveCamera::update(int width, int height)
{

	m_projectionMatrix = glm::perspective(glm::radians(m_angle), (float)width / (float)height, m_nearPlane, m_farPlane);
}

/**
* Empty method. It is included to satisfy the virtual function declaration in the base class Object.
*/
void LIB_API PerspectiveCamera::render(const glm::mat4&, void*)
{

}
