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
void LIB_API Lod::addVertexData(VertexData vertex)
{
	m_vertex.push_back(vertex);
}

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
 * @brief Get the list of faces in the level of detail.
 * @return The list of faces.
 */
std::vector<glm::uvec3> LIB_API Lod::faces() const
{
	return m_faces;
}

/**
 * @brief Get the list of vertex data in the level of detail.
 * @return The list of vertex data.
 */
std::vector<VertexData> LIB_API Lod::vertex() const
{
	return m_vertex;
}
