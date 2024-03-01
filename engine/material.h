/**
 * @file		material.h
 * @ingroup Engine
 * @brief	Declaration of the Material class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "object.h" 
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

// C/C++:
#include <string>
#include <vector>

/////////////
// CLASSES //
/////////////

/**
 * @brief Represents a material.
 * 
 * This class extends the Object class and contains all the parameters to define a material. 
 * It includes a texture and methods for setting these properties, rendering the material, and cloning the material.
 */
class LIB_API Material : public Object
{
	//////////
public: //
	//////////	

	Material(const std::string& name);
	~Material();

	void emission(float r, float g, float b, float a);
	glm::vec4 emission();
	void ambient(float r, float g, float b, float a);
	void diffuse(float r, float g, float b, float a);
	void specular(float r, float g, float b, float a);
	void shininess(float s);
	void render(const glm::mat4 & = glm::mat4{ 1 }, void* = nullptr) override;
	void setTexture(Texture* tex);
	Material* clone() const;

	///////////
protected:	//
	///////////	 

	// Internal vars:
	glm::vec4 m_emission; ///< The emission property of the material.
	glm::vec4 m_ambient; ///< The ambient property of the material.
	glm::vec4 m_diffuse; ///< The diffuse property of the material.
	glm::vec4 m_specular; ///< The specular property of the material.
	float m_shininess; ///< The shininess property of the material.
	Texture* m_texture; ///< The texture of the material.
	
};
