/**
 * @file	   node.h
 * @ingroup Engine
 * @brief	Declaration of the Node class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "object.h"
#include "list.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

// C/C++:
#include <string>
#include <vector>
#include <memory>

/////////////
// CLASSES //
/////////////

/**
 * @brief Represents a node in a scene graph. 
 * 
 * This class extends the Object class with the functions to locate the object in the 3D space through a matrix and in a hierarchy through a hierarchical structure. It also implements a function to quickly get a given node’s final matrix in world coordinates.
 */
class LIB_API Node : public Object
{
//////////
public: //
//////////	   
	glm::mat4 getFinalMatrix() const;
	Node* parent() const;
	void parent(Node*);
	std::vector<Node*> children()const;
	void addChild(Node*);
	glm::mat4 matrix() const;
	void matrix(glm::mat4);
	void pass(std::shared_ptr<List> list, glm::mat4 matrixLivelloPrima = glm::mat4{ 1 });
	void render(const glm::mat4 & = glm::mat4{ 1 }, void* = nullptr) override;
	void removeChild(Node* nodeToRemove);
	void setProgram(Shader* program);

	Node(const std::string& name, const glm::mat4& matrix);
	~Node();
	//distruttore cancellare le istanze del vettore, fare delete dell'operatore di copia e costruttore di copia

////////////
protected://
////////////	 

	// Internal vars:
	glm::mat4 m_matrix; ///< The transformation matrix of the node relative to its parent node.
	Node* m_parent; ///< A pointer to the parent node, initialized to nullptr.
	std::vector<Node*> m_children; ///< A vector of pointers to the child nodes.
	Shader* m_program;
};
