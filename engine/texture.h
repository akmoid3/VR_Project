/**
 * @file		texture.h
 * @ingroup Engine
 * @brief	Declaration of the Texture class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "object.h"
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
 * @brief	Represents a texture.
 * @details This class extends the Object class and represents a texture. It provides methods for loading the texture, deleting the texture, getting the texture ID, and rendering the texture.
 */
class LIB_API Texture : public Object
{
//////////
public: //
//////////	 
	
	Texture(const std::string& name);

	void load();
	void deleteTexture();
	unsigned int texId();

	// Ereditato tramite Object
	void render(const glm::mat4&, void*) override;


////////////
protected://
////////////	 


	// Internal vars:
	unsigned int m_texId; ///< Special texture ID.

};
