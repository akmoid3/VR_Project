/**
 * @file		object.h
 * @ingroup Engine
 * @brief	Definition of the Object abstract class
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

/////////////
// CLASSES //
/////////////

/**
 * @brief Base abstract class used by all the derived classes.
 *
 * This class is responsible for keeping track of the existing objects, forcing some required API (virtual) methods
 * and providing a unique ID to each object.
 */
class LIB_API Object
{
	
//////////
public: //
//////////	   
	Object(const std::string& m_name);
	virtual ~Object();

	void name(std::string);
	std::string name() const;
	unsigned int id() const;

	/**
	* @brief Virtual method. Render the object (implementation specific to derived classes).
	*/
	virtual void render(const glm::mat4&, void* = nullptr) = 0;


////////////
protected://
////////////	 

	// Internal vars:
	unsigned int m_id; ///< The unique ID of the object.
	std::string m_name; ///< The name of the object.
	static unsigned int m_global_id; ///< Global counter for generating unique IDs.0 represents an invalid ID, valid IDs start from 1.
};
