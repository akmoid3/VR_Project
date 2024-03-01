/**
 * @file		vertexData.h
 * @ingroup Engine
 * @brief	Declaration of the VertexData class
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

/////////////
// CLASSES //
/////////////

/**
 * @brief Represents the data of a vertex in a 3D mesh.
 * @details This class provides methods for setting and getting the position, normal vector, and texture coordinates of the vertex.
 */
class LIB_API VertexData {

public:
	void posVertex(glm::vec3 posVertex);
	void normal(glm::vec3 normal);
	void coordText(glm::vec2 coordText);
	glm::vec3 posVertex() const;
	glm::vec3 normal() const;
	glm::vec2 coordText() const;
	void printData();

private:
	glm::vec3 m_posVertex; ///< The position of the vertex.
	glm::vec3 m_normal; ///< The normal vector associated to the vertex.
	glm::vec2 m_coordText; ///< The texture coordinates of the vertex.

};