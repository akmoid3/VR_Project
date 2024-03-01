/**
 * @file		lod.h
 * @ingroup Engine
 * @brief	Declaration of the Lod class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
// C/C++:
#include <string>
#include <vector>
#include "vertexdata.h"

/////////////
// CLASSES //
/////////////

/**
 * @brief Represents a level of detail (LOD) of a Mesh.
 * @details This class extends the Object class. It provides methods for adding vertex data and faces to the level of detail, and getting the list of vertex data and faces.
 * Each vertex is saved in the vertices' vector. For each face, three unsigned int indices reference the vertices stored in the VertexData vector of this class.
 */
class LIB_API Lod {

public:

	Lod();
	void addVertexData(VertexData);
	void addFaces(glm::uvec3);
	std::vector<glm::uvec3> faces() const;
	std::vector<VertexData> vertex() const; // std::vector<glm vec3 > per vertici, normale ,textcoord, uv. interleaved array(avanzato), singolo vbo con prima coord vert, poi normale e uv

protected:
	std::vector<VertexData> m_vertex; ///< The list of VertexData objects in the level of detail.
	std::vector<glm::uvec3> m_faces; ///< The list of faces in the level of detail.
};