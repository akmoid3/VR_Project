/**
 * @file		camera.cpp
 * @ingroup Engine
 * @brief	Implementation of the Camera abstract class.
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#include "engine.h"

 //////////////////////////
 // BODY OF CLASS Camera //
 //////////////////////////

/**
* @brief Constructor for the Camera class.
*
* @param name The name of the camera.
* @param matrix The initial transformation matrix.
* @param near The near clipping plane.
* @param far The far clipping plane.
*/
LIB_API Camera::Camera(const std::string& name, const glm::mat4& matrix, float near, float far) :
	Node(name, matrix), m_nearPlane{ near }, m_farPlane{ far }, m_projectionMatrix{ glm::mat4{1} } {

}

/**
 * @brief Destructor for the Camera class.
 */
LIB_API Camera::~Camera() {}

/**
 * @brief Get the camera's projection matrix.
 *
 * @return The camera's projection matrix.
 */
glm::mat4 LIB_API Camera::projectionMatrix() const
{
	return m_projectionMatrix;
}

/**
 * @brief Get the inverse of the camera's final transformation matrix in world coordinates.
 *
 * @return The inverse of the camera's final transformation matrix.
 */
glm::mat4 LIB_API Camera::getCameraInverse()
{
	return inverse(getFinalMatrix());
}

/**
 * @brief Render the camera (implementation specific to derived classes).
 */
void LIB_API Camera::render(const glm::mat4&, void*)
{
	//funzione vuota (o visualizzare una pallina per debug)
}

