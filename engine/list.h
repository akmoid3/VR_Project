/**
 * @file		list.h
 * @ingroup Engine
 * @brief	Declaration of the List class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "object.h"
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
#include <list>

/////////////
// CLASSES //
/////////////

/**
 * @brief Represents a list of objects used to rendered in a 3D scene.
 * @details This class extends the Object class. It provides methods for rendering the objects in a specific order (first the lights then the meshes), adding objects to the list, getting the list of objects, and resetting the list.
 */
class LIB_API List : public Object
{
	//////////
public:	  //
	//////////	   
	void render(const glm::mat4 & = glm::mat4{ 1 }, void* = nullptr) override;
	void add(Object* object, glm::mat4 finalMatrix);
	std::list<std::pair<Object*, glm::mat4>> getList();
	void resetList();

	List();
	~List();

	List(const List& other);
	List& operator=(const List& other);


	///////////
protected:	//
	///////////	 


	// Internal vars:
	std::list<std::pair<Object*, glm::mat4>> m_list; ///< The list of objects.
	unsigned int cnt; ///< A counter for the number of lights added to the list.
	static unsigned int max_lights; ///< The maximum number of lights allowed in the scene, set to 8.
	Material* m_shadowMaterial; ///< The material to be used for casting the object's shadow.
};
