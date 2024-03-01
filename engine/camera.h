/**
 * @file		camera.h
 * @ingroup Engine
 * @brief	Declaration of the Camera abstract class.
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
* @brief Abstract class that represents a camera in a 3D scene.
*
* This class is meant to be extended by concrete camera implementations, such as
* perspective or orthographic cameras, to define specific camera behaviors.
*/
class LIB_API Camera : public Node
{
	//////////
public:    //
	//////////	   

	Camera(const std::string& name, const glm::mat4& matrix, float near, float far);
	~Camera();

	glm::mat4 projectionMatrix() const;
	glm::mat4 getCameraInverse();

	// Ereditato tramite Node
	void render(const glm::mat4&, void*) override;

	/**
	 * @brief Virtual method. Update the camera based on the viewport dimensions.
	 *
	 * @param width The width of the viewport.
	 * @param height The height of the viewport.
	 */
	virtual void update(int width, int height) = 0;

	///////////
protected:	//
	///////////	 

	// Internal vars:
	glm::mat4 m_projectionMatrix; ///< The camera's projection matrix, initialized in the constructor with a default value equivalent to an identity matrix.
	float m_nearPlane; ///< The near clipping plane.
	float m_farPlane; ///< The far clipping plane.

};
