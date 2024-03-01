/**
 * @file		omniLight.h
 * @ingroup Engine
 * @brief	Declaration of the OmniLight class
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
 * @brief Represents an omnidirectional light source in a 3D scene.
 * @details This class extends the Light class. It provides methods for setting the position of the light, setting the attenuation constants, setting the cutoff value, and rendering the light.
 */
class LIB_API OmniLight : public Light
{
	//////////
public: //
	//////////	   

	OmniLight(const std::string& name, const glm::mat4& matrix);
	~OmniLight();

	void position(float x, float y, float z) override;
	void attenuationConstant(float a);
	void attenuationLinear(float a);
	void attenuationQuadratic(float a);
	void cutoff(float a);

	// Ereditato tramite Node
	void render(const glm::mat4&, void*) override;


	///////////
protected:	//
	///////////	 
	//   
	// const/dest


	// Internal vars:
	glm::vec4 m_position;		///< The position of the light. The w component is 1.0f, indicating that this is an omnidirectional light.
	float m_attenuationConstant; ///< The constant attenuation of the light. Default is 1.
	float m_attenuationLinear;	///< The linear attenuation of the light. Default is 0.
	float m_attenuationQuadratic; ///< The quadratic attenuation of the light. Default is 0.
	float m_cutoff; ///< The cutoff value of the light. It is set to the special value of 180.f, indicating that this is an omnidirectional light.
};
