/**
 * @file		vertexData.cpp
 * @ingroup Engine
 * @brief	Implementation of the VertexData class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#include "engine.h"
#include <iostream>

//////////////////////////////
// BODY OF CLASS VertexData //
//////////////////////////////

/**
 * @brief Set the position of the vertex.
 * @param posVertex The position of the vertex.
 */
void LIB_API VertexData::posVertex(glm::vec3 posVertex)
{
	m_posVertex = posVertex;
}

/**
 * @brief Set the normal vector associated to the vertex.
 * @param normal The normal of the vertex.
 */
void LIB_API VertexData::normal(glm::vec3 normal)
{
	m_normal = normal;
}

/**
 * @brief Set the texture coordinates of the vertex.
 * @param coordText The texture coordinates of the vertex.
 */
void LIB_API VertexData::coordText(glm::vec2 coordText)
{
	m_coordText = coordText;
}

/**
 * @brief Get the position of the vertex.
 * @return The position of the vertex.
 */
glm::vec3 LIB_API VertexData::posVertex() const
{
	return m_posVertex;
}

/**
 * @brief Get the normal of the vertex.
 * @return The normal of the vertex.
 */
glm::vec3 LIB_API VertexData::normal() const
{
	return m_normal;
}

/**
 * @brief Get the texture coordinates of the vertex.
 * @return The texture coordinates of the vertex.
 */
glm::vec2 LIB_API VertexData::coordText() const
{
	return m_coordText;
}

/**
 * @brief Print the vertex data.
 * @details This method prints the position, normal, and texture coordinates of the vertex.
 */
void LIB_API VertexData::printData()
{
	std::cout << "Position Vertex: (" << m_posVertex.x << ", " << m_posVertex.y << ", " << m_posVertex.z << ")" << std::endl;
	std::cout << "Normal: (" << m_normal.x << ", " << m_normal.y << ", " << m_normal.z << ")" << std::endl;
	std::cout << "Texture Coordinate: (" << m_coordText.x << ", " << m_coordText.y << ")" << std::endl;
}
