/**
 * @file		spotLight.h
 * @ingroup Engine
 * @brief	Declaration of the SpotLight class
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
 * @brief Represents a spotlight in a 3D scene.
 * @details This class extends the Light class. It provides methods for setting the position, direction, attenuation constants, and cutoff angle of the light, and rendering the light.
 */
class LIB_API SpotLight : public Light
{
	//////////
public: //
	//////////	   

	SpotLight(const std::string& name, const glm::mat4& matrix);
	~SpotLight();

	void position(float x, float y, float z) override;
	void cutoff(float cutoff);
	void direction(float dx, float dy, float dz);
	void attenuationConstant(float a);
	void attenuationLinear(float a);
	void attenuationQuadratic(float a);


	// Ereditato tramite Node
	void render(const glm::mat4&, void*) override;


	///////////
protected:	//
	///////////	 

	// Internal vars:
	glm::vec4 m_position;			///< The position of the light. The w component is 1.0f, indicating that this is a spotlight.
	float m_attenuationConstant;	///< The constant attenuation of the light. Default is 1.
	float m_attenuationLinear;		///< The linear attenuation of the light. Default is 0.
	float m_attenuationQuadratic;	///< The quadratic attenuation of the light. Default is 0.
	float m_cutoff; 				///< The cutoff angle of the light. Any angle between 0 and 90 degrees.
	glm::vec3 m_direction; ///< The direction of the light.
};
