/**
 * @file		mesh.h
 * @ingroup Engine
 * @brief	Declaration of the Mesh class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */
#pragma once


 // Library include:
#include "node.h"
#include "material.h"
#include "vertexdata.h"
#include "lod.h"
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
 * @brief Represents a single 3D object.
 *
 * This class extends the Node class and stores a single 3D model with a certain material and level of detail (LOD).
 * It provides methods for managing the material of the mesh, loading the mesh, adding LODs, and rendering the mesh.
 * It also includes functionality for blinking the mesh and casting shadows.
 */
class LIB_API Mesh : public Node
{
//////////
public: //
//////////	   
	static Material defaultMaterial; ///< Default material for all Mesh objects.

	Mesh(const std::string& name, const glm::mat4& matrix, Material* material = &defaultMaterial);
	~Mesh();

	void addLod(Lod);
	void loadLod();
	void setMaterial(Material*);
	void shadowMaterial(Material*);
	void startBlink();
	void stopBlink();
	void resetMaterial();
	void saveMaterial();
	bool isBlinking() const;
	bool isCastingShadow();
	void isCastingShadow(bool);
	void disposeBuffers();

	// Ereditato tramite Node
	void render(const glm::mat4&, void*) override;

	///////////
protected:	//
	///////////	 

	// Methods:
	void blink();

	// Internal vars:
	Material* m_material; ///< The material of the mesh.
	std::vector<Lod> lods; ///< A vector containing the levels of detail of the mesh.
	bool m_isBlinking; ///< A flag indicating whether the mesh is blinking, initialized to false.
	Material* previousMat; ///< Used for storing the original material of the mesh, when the mesh is set to blink, initialized to nullptr.
	Material* m_shadowMaterial; ///< The material used for rendering the shadow of the mesh, initialized to nullptr.
	bool m_isCastingShadow; ///< A flag indicating whether the mesh should be casting a shadow, initialized to true.

	// VBO
	unsigned int vao;
	unsigned int vbo_vertex;
	unsigned int vbo_normal;
	unsigned int vbo_textcoord;
	unsigned int vbo_face;

};
