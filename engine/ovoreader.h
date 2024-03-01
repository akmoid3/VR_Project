/**
 * @file	   ovoreader.h
 * @ingroup Engine
 * @brief	Declaration of the Ovoreader class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "node.h"
#include "material.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

// C/C++:
#include <string>
#include <vector>
#include <utility>

/////////////
// CLASSES //
/////////////

/**
 * @brief Decoder for the OverVision object (OVO) 3D file format.
 *
 * It provides methods for loading scene from a file in the OVO format, getting the textures, and recursively loading data.
 */
class LIB_API Ovoreader
{


	//////////
public:	  //
	//////////	   

	Node* load(std::string fileName);
	std::vector<std::pair<std::string, Texture*>> textures();
	void deleteTexturesAndMaterials();

	///////////
protected:	//
	///////////	 

	Node* recursiveLoad(const char* data, unsigned int& position);

	// Internal vars:
	std::vector<std::pair<std::string, Material*>> m_materials; ///< A vector of pairs, where each pair contains a string representing the name and a pointer to a Material object.
	std::vector<std::pair<std::string, Texture*>> m_textures; ///< A vector of pairs, where each pair contains a string representing a name and a pointer to a Texture object.

};
