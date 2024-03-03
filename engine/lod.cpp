/**
 * @file		lod.cpp
 * @ingroup Engine
 * @brief	Implementation of the Lod class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#include "engine.h"

 ///////////////////////////
 // BODY OF CLASS Lod     //
 ///////////////////////////

 /**
 * @brief Constructor for the Lod class.
 * @details This constructor initializes the level of detail with an empty list of vertex data and faces.
 */
LIB_API Lod::Lod()
{
}

/**
 * @brief Add VertexData object to the level of detail.
 * @param vertex The VertexData object to be added.
 */
/*
void LIB_API Lod::addVertexData(VertexData vertex)
{
	m_vertex.push_back(vertex);
}
*/

/**
 * @brief Add a face to the level of detail.
 * 
 * A face is represented by a vector of three components corresponding to its vertices.
 * @param face The face to be added.
 */
void LIB_API Lod::addFaces(glm::uvec3 face)
{
	m_faces.push_back(face);
}

/**
 * @brief Add a normal vector to the level of detail.
 *
 * A normal is represented by a vector of three components.
 * @param normal The normal vector to be added.
 */
void LIB_API Lod::addNormal(glm::vec3 normal)
{
	m_normal.push_back(normal);
}

/**
 * @brief Add a vertex to the level of detail.
 *
 * A vertex is represented by a vector of three components.
 * @param vertex The vertex to be added.
 */
void LIB_API Lod::addVertex(glm::vec3 vertex)
{
	m_vertex.push_back(vertex);
}

/**
 * @brief Add a text coordinate to the level of detail.
 *
 * A text coordinate is represented by a vector of three components.
 * @param coordText The texture coordinate to be added.
 */
void LIB_API Lod::addCoordText(glm::vec2 coordText)
{
	m_coordText.push_back(coordText);
}

/**
 * @brief Add a face to the level of detail.
 *
 * A face is represented by a vector of three components corresponding to its vertices.
 * @param face The face to be added.
 */
void LIB_API Lod::normals(std::vector<glm::vec3> normal)
{
	m_normal = normal;
}

/**
* @brief Set the vertex vector.
* @param vertex A vector of vertices.
*/
void Lod::vertex(std::vector<glm::vec3> vertex)
{
	m_vertex = vertex;
}


/**
* @brief Set the texture coordinate vector.
* @param coordText A vector of texture coordinates.
*/
void LIB_API Lod::coordText(std::vector<glm::vec2> coordText)
{
	m_coordText = coordText;
}

/**
 * @brief Get the list of faces in the level of detail.
 * @return The list of faces.
 */
std::vector<glm::uvec3> LIB_API Lod::faces() const
{
	return m_faces;
}

/**
 * @brief Get the list of normal vectors in the level of detail.
 * @return The list of normal vectors
 */
std::vector<glm::vec3> LIB_API Lod::normals() const
{
	return m_normal;
}

/**
 * @brief Get the list of vertex vectors in the level of detail.
 * @return The list of vertex vectors
 */
std::vector<glm::vec3> LIB_API Lod::vertex() const
{
	return m_vertex;
}

/**
 * @brief Get the list of texture coordinates in the level of detail.
 * @return The list of texture coordinates
 */
std::vector<glm::vec2> LIB_API Lod::coordText() const
{
	return m_coordText;
}


/**
 * @brief Get the list of vertex data in the level of detail.
 * @return The list of vertex data.
 */
 /*
std::vector<VertexData> LIB_API Lod::vertex() const
{
	return m_vertex;
}
*/