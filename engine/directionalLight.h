/**
 * @file		directionalLight.h
 * @ingroup Engine
 * @brief	Declaration of the DirectionalLight class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "light.h"
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
* @brief Represents a directional light source in a 3D scene.
* @details This class extends the Light class. It provides methods for setting the position of the light and rendering the light.
*/
class LIB_API DirectionalLight : public Light
{
	//////////
public:	  //
	//////////	   
	
	DirectionalLight(const std::string& name, const glm::mat4& matrix);
	~DirectionalLight();

	void position(float x, float y, float z) override;
	

	// Ereditato tramite Node
	void render(const glm::mat4&, void*) override;


	///////////
protected:	//
	///////////	 

	// Internal vars:
	glm::vec4 m_position; ///< The position of the light. The w component is 0.0f, indicating that this is a directional light.
};
