/**
 * @file		list.cpp
 * @ingroup Engine
 * @brief	Implementation of the List class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#include "engine.h"
#include <utility>
#include <iostream>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

////////////
// STATIC //
////////////

unsigned int List::max_lights = 8;

//////////////////////////
// BODY OF CLASS List //
//////////////////////////

/**
 * @brief Reset the list.
 * @details This method disables all the lights, clears the list, and resets the counter.
 */
void LIB_API List::resetList() {

	// Spegne tutte le luci
	for (unsigned int i = 0; i < cnt; i++)
	{
		glDisable(GL_LIGHT0 + i);
	}
	m_list.clear();
	cnt = 0;
}

/**
 * @brief Render the objects in the list.
 * @details This method renders the objects in the list. In the first pass, it renders the objects normally. In the second pass, it renders the shadows of the objects that cast shadows.
 * @param c_inverse  The inverse of the final camera matrix in world coordinates of this object.
 * @param flag A flag indicating whether object selection is enabled. If the flag is set to true, it signals that objects need to be rendered to allow object selection.

 */
void LIB_API List::render(const glm::mat4& c_inverse, void* flag)
{
	// First pass: Render objects normally
	for (auto& pair : m_list) {
		Object* obj = pair.first;
		glm::mat4 mat = pair.second;
		obj->render(c_inverse * mat, flag);
	}

	// Second pass: Render the shadows
	for (auto& pair : m_list) {
		Object* obj = pair.first;
		glm::mat4 mat = pair.second;

		Mesh* m = dynamic_cast<Mesh*>(obj);
		if (m != 0 && m->isCastingShadow()) {

			// Apply the scaling matrix to create the shadow effect
			glm::mat4 shadowMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.01f, 1.0f));
			mat = shadowMat * mat;

			// Render the object with the scaling matrix applied and the shadow material
			m->shadowMaterial(m_shadowMaterial);
			m->render(c_inverse * mat, (void*)false);
			m->shadowMaterial(nullptr);
		}

	}
}

/**
 * @brief Add an object to the render list.
 * @details This method adds an object to be rendered in a 3D scene to the list. 
 * If the object is a light and the number of lights is less than the maximum number of lights, the light is added to the front of the list and the light number is set equal to the counter of lights. 
 * If the object is a mesh, it is added to the back of the list. This ensures that the lights will be rendered before the meshes.
 * @param object The object to be added.
 * @param finalMatrix  The final position matrix in world coordinates of this object.

 */
void LIB_API List::add(Object* object, glm::mat4 finalMatrix)
{

	// Controlla che le luci da renderizzare non siano più di 8
	Light* l = dynamic_cast<Light*>(object);
	if (l != 0) {
		if (cnt < max_lights) {
			m_list.push_front(std::make_pair(l, finalMatrix));
			l->lightNumber(GL_LIGHT0 + (cnt++));
		}
		else
		{
			std::cout << "Light ignored " << l->name() << " :" << "Can't render more than 8 lights" << std::endl;
		}

	}

	Mesh* m = dynamic_cast<Mesh*>(object);
	if (m != 0) {
		m_list.push_back(std::make_pair(m, finalMatrix));
	}
}

/**
 * @brief Get the list of objects.
 * @return The list of objects.
 */
std::list<std::pair<Object*, glm::mat4>> List::getList()
{
	return m_list;
}

/**
 * @brief Constructor for the List class.
 * @details This constructor initializes the list with a default name of "list", 
 * a counter for the number of the lights added to the scene of 0, a maximum number of lights of 8, 
 * and a default shadow material.
 */
LIB_API List::List() : Object("list"), cnt{ 0 }, m_shadowMaterial{new Material("Shadow")}
{
	m_shadowMaterial->emission(0.0f, 0.0f, 0.0f, 1.0f);
	m_shadowMaterial->specular(0.0f, 0.0f, 0.0f, 1.0f);
	m_shadowMaterial->diffuse(0.0f, 0.0f, 0.0f, 1.0f);
	m_shadowMaterial->ambient(0.0f, 0.0f, 0.0f, 1.0f);
}

/**
 * @brief Destructor for the List class.
 */
LIB_API List::~List()
{
	delete m_shadowMaterial;
}

/**
 * @brief Copy constructor for the List class.
 * @param other The List object to be copied.
 */
LIB_API List::List(const List& other) : Object(other), m_list(other.m_list), cnt(other.cnt) {
	m_shadowMaterial = new Material(*other.m_shadowMaterial);
}

/**
 * @brief Copy assignment operator for the List class.
 * @param other The List object to be copied.
 * @return A reference to the copied List object.
 */
List LIB_API & List::operator=(const List& other) {
	if (this != &other) {
		Object::operator=(other);
		m_list = other.m_list;
		cnt = other.cnt;
		delete m_shadowMaterial;
		m_shadowMaterial = new Material(*other.m_shadowMaterial);
	}
	return *this;
}
