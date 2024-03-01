/**
 * @file		perspectiveCamera.h
 * @ingroup Engine
 * @brief	Declaration of the PerspectiveCamera class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "camera.h"
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
 * @brief  Represents a perspective camera in a 3D scene.
 * 
 * It provides functionality for creating and manipulating a perspective camera.
 */
class LIB_API PerspectiveCamera : public Camera
{
	//////////
public: //
	//////////	   

	PerspectiveCamera(const std::string& name, const glm::mat4& matrix,float near,float far,float angle);
	~PerspectiveCamera();
	void update(int width, int height)override;


	// Ereditato tramite Node
	void render(const glm::mat4&, void*) override;


	///////////
protected:	//
	///////////	 

	// Internal vars:
	float m_angle; ///< The field of view angle.

};
