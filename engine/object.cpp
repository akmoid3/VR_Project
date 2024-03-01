/**
 * @file		object.cpp
 * @ingroup Engine
 * @brief	Implementation of the Object abstract class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#include "engine.h"


////////////
// STATIC //
////////////

unsigned int Object::m_global_id = 1; // 0 rappresenta un ID non valido, ID validi partono da 1

///////////////////////////
// BODY OF CLASS Object  //
///////////////////////////

/**
* @brief Set the name of the object.
*
* @param name The name to set.
*/
void LIB_API Object::name(std::string name)
{
	m_name = name;
}

/**
* @brief Get the name of the object.
*
* @return The name of the object.
*/
std::string LIB_API Object::name() const
{
	return m_name;
}

/**
* @brief Get the unique ID of the object.
*
* @return The unique ID of the object.
*/
unsigned int LIB_API Object::id() const
{
	return m_id;
}

/**
* @brief Constructor for the Object class.
*
* @param name The name of the object.
*/
LIB_API Object::Object(const std::string& name) : m_name{ name }, m_id{m_global_id++}
{

}

/**
* @brief Destructor for the Object class.
*/
LIB_API Object::~Object()
{

}
