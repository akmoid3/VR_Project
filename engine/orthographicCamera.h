/**
 * @file		orthographicCamera.h
 * @ingroup Engine
 * @brief	Declaration of the OrthographicCamera class
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
 * 
 * @brief Represents an orthographic camera in a 3D scene.
 *
 * It provides functionality for creating and manipulating an orthographic camera.
 */
class LIB_API OrthographicCamera : public Camera
{
	//////////
public:    //
	//////////	   

	OrthographicCamera(const std::string& name, const glm::mat4& matrix, float near, float far);
	~OrthographicCamera();
	void update(int width, int height)override;

	// Ereditato tramite Node
	void render(const glm::mat4&, void*) override;


	///////////
protected:	//
	///////////	 
	
	// Internal vars:	
	float m_left; ///< The left clipping plane coordinate, initialized to 0.0f.
	float m_bottom; ///< The bottom clipping plane coordinate, initialized to 0.0f.
};
