/**
* @file	   node.cpp
* @ingroup Engine
* @brief		Implementation of the Node class
*
* @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
*/
#include "engine.h"
#include <iostream>
#include <algorithm>


#define FREEGLUT_STATIC
#include <GL/freeglut.h>

//////////////////////////
// BODY OF CLASS Node   //
//////////////////////////

/**
* @brief Get the final transformation matrix of the node in world coordinates.
* @return The final transformation matrix of the node.
*/
glm::mat4 LIB_API Node::getFinalMatrix() const
{
	if (m_parent != nullptr)
	{
		return  m_matrix * m_parent->getFinalMatrix(); 
	}
	else
	{
		return m_matrix;
	}
}

/**
* @brief Get the parent of the node.
* @return A pointer to the parent node.
*/
Node LIB_API * Node::parent() const
{
	return m_parent;
}

/**
* @brief Set the parent of the node.
* @param parent A pointer to the new parent node.
*/
void LIB_API Node::parent(Node* parent)
{
	m_parent = parent;
}

/**
* @brief Get the children of the node.
* @return A vector of pointers to the child nodes.
*/
std::vector<Node*> LIB_API Node::children() const
{
	return m_children;
}

/**
* @brief Add a child to the node.
* @param child A pointer to the child node to be added.
*/
void LIB_API Node::addChild(Node* child)
{
	m_children.push_back(child);
	child->parent(this);
}

/**
* @brief Get the transformation matrix of the node relative to its parent node in the hierarchy.
* @return The transformation matrix of the node.
*/
glm::mat4 LIB_API Node::matrix() const
{
	return m_matrix;
}

/**
* @brief Set the transformation matrix of the node relative to its parent node in the hierarchy.
* @param matrix The new transformation matrix of the node.
*/
void LIB_API Node::matrix(glm::mat4 matrix)
{
	m_matrix = matrix;
}

/**
* @brief Recursively pass the nodes in the hierarchy to a list for rendering, starting from the invoking node. 
*
* This method recursively parses all the child nodes linked to the invoking node. 
* During each recursion, it invokes the pass() method of the child nodes. The child node relative tranformation matrix is multiplied by the parent node’s final transformation matrix in world coordinates. 
* @param list A shared pointer to the list.
* @param previousLevelMatrix The final transformation matrix of the previous level. Default is an identity matrix.
*/
void LIB_API Node::pass(std::shared_ptr<List> list, glm::mat4 previousLevelMatrix)
{
	// Calculate final matrix
	glm::mat4 finalMatrix = previousLevelMatrix * m_matrix;

	// Add to list
	list->add(this, finalMatrix);

	//render(c_inverse * finalMatrix);

	// Call recursive pass on child
	for (const auto& child : m_children)
	{
		child->pass(list, finalMatrix);
	}
}

/**
* @brief Render the node.
* @param matrix The result of multiplying the inverse of the final camera matrix by the final position matrix in world coordinates of this object. Default is an identity matrix.
* @param flag This parameter is ignored in this implementation. Default is nullptr.
*/
void LIB_API Node::render(const glm::mat4& matrix, void*)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(matrix));
}

/**
 * @brief Node destructor.
 *
 * This destructor is responsible for deleting dynamically allocated child nodes
 * stored in the m_children vector. It iterates through the vector, deletes each child
 * node, and prints a message indicating the deletion.
 */
LIB_API Node::~Node()
{
	for (Node* node : m_children) {
		std::cout << "Deleted " << node->name() << std::endl;
		delete node;
	}
}

/**
 * @brief Removes a child node from the m_children vector.
 *
 * @param nodeToRemove A pointer to the Node object to be removed.
 *
 * This function searches for the specified child node in the m_children vector.
 * If found, it removes the node from the vector. If not found, it prints an error message.
 */
void LIB_API Node::removeChild(Node* nodeToRemove) {
	auto it = std::find_if(m_children.begin(), m_children.end(),
		[nodeToRemove](const Node* node) {
			return node == nodeToRemove;
		});

	// Check if the item was found before removing
	if (it != m_children.end()) {
		// Erase the item from the vector
		m_children.erase(it);
	}
	else {
		std::cout << "Node not found in the vector." << std::endl;
	}
}

/**
* @brief Constructor for the Node class.
* @param name The name of the node.
* @param matrix The initial transformation matrix of the node relative to its parent.
*/
LIB_API Node::Node(const std::string& name, const glm::mat4& matrix)
	: Object(name), m_matrix{ matrix }, m_parent{ nullptr }
{
}
