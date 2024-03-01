/**
* @file		mesh.cpp
* @ingroup Engine
* @brief		Implementation of the Mesh class
*
* @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
*/
#include "engine.h"
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <ctime>

////////////
// STATIC //
////////////

Material Mesh::defaultMaterial("default"); //viene creata quando parte la dll e distrutta alla chiusura

//////////////////////////
// BODY OF CLASS Mesh   //
//////////////////////////

/**
* @brief Add a level of detail to the mesh.
* @param lod The level of detail to be added.
*/
void LIB_API Mesh::addLod(Lod lod)
{
	lods.push_back(lod);
}

/**
 * @brief Make the mesh blink. 
 * 
 * This method is used by the render() method of this class. It calculates the emission intensity based on the elapsed time since the start of the program. It scales a sine wave from [-1, 1] to [0, 1] and multiplies it by a blink speed to convert the elapsed time to seconds for the sin function. 
 * The calculated intensity is then used to set the emission property of the mesh's material, making the mesh appear to blink.
 */
void LIB_API Mesh::blink() 
{
	// glutGet(GLUT_ELAPSED_TIME) retrieves the elapsed time in milliseconds since the start of the GLUT program. 
	// Scales sine wave from[-1, 1] to[0, 1]: addition of 0.5 shifts it up, and multiplication by 0.5 ensures result in[0, 1].
	// Multiplying by blink_speed converts elapsed time to seconds for the sin function.
	float blink_speed = 0.005; // Adjust this value to control the blinking speed. 
	float intensity = 0.5 + 0.5 * sin(glutGet(GLUT_ELAPSED_TIME) * blink_speed);

	// Set the material properties
	m_material->emission(intensity, intensity, intensity, 1.0f);
}

/**
 * @brief Load the mesh for rendering.
 *
 * This method uses the most detailed level of detail (LOD) to load the mesh for rendering. It iterates over each face in the LOD, retrieves the vertex data for each vertex in the face, and uses this data to render the face. 
 * If the object selection flag is set to true, the method sets the color of the object according to its ID for color picking. 
 * @param flag A flag indicating whether to apply the color to the mesh. If the flag is set, it applies the color to the mesh. 
 */
void LIB_API Mesh::load(bool flag) {
	
	// Set the color of the object according to its ID for color picking.
	unsigned char rgba[4];
	if(flag){
		unsigned int idValue = id();
		memcpy(rgba, &idValue, sizeof(idValue)); // Copy the ID to the RGBA array
		
	}
	// Use only the most detailed level of detail (LOD). 
	// Render the three vertices of the face.
	for (const auto& face : lods[0].faces()) {

		VertexData vertex0 = lods[0].vertex()[face[0]];
		VertexData vertex1 = lods[0].vertex()[face[1]];
		VertexData vertex2 = lods[0].vertex()[face[2]];

		glBegin(GL_TRIANGLES);
		if (flag) {
			glColor4ubv(rgba);
		}

		glNormal3f(vertex0.normal().x, vertex0.normal().y, vertex0.normal().z);
		glTexCoord2f(vertex0.coordText().s, vertex0.coordText().t);
		glVertex3f(vertex0.posVertex().x, vertex0.posVertex().y, vertex0.posVertex().z);

		glNormal3f(vertex1.normal().x, vertex1.normal().y, vertex1.normal().z);
		glTexCoord2f(vertex1.coordText().s, vertex1.coordText().t);
		glVertex3f(vertex1.posVertex().x, vertex1.posVertex().y, vertex1.posVertex().z);

		glNormal3f(vertex2.normal().x, vertex2.normal().y, vertex2.normal().z);
		glTexCoord2f(vertex2.coordText().s, vertex2.coordText().t);
		glVertex3f(vertex2.posVertex().x, vertex2.posVertex().y, vertex2.posVertex().z);
		glEnd();
	}
}

/**
* @brief Set the material of the mesh.
* @param material The new material of the mesh.
*/
void LIB_API Mesh::setMaterial(Material* material)
{
	m_material = material;
}

/**
* @brief Set the shadow material of the mesh.
* @param material The material to be used for rendering the shadow of the mesh.
*/
void LIB_API Mesh::shadowMaterial(Material * material)
{
	m_shadowMaterial = material;
}

/**
 * @brief Render the mesh. 
 * 
 * This method first calls the render method of the Node class. If the mesh has a material and the object selection flag is false, it checks if the mesh is blinking. If it is, it calls the blink method. 
 * Then, it checks if the mesh has a shadow material. If it does, it renders the shadow material, otherwise it renders the mesh material. Finally, it loads the mesh for rendering.
 * @param m The result of multiplying the inverse of the final camera matrix by the final position matrix in world coordinates of this object.
 * @param flag A flag for object selection.
 */
void LIB_API Mesh::render(const glm::mat4& m, void* flag)
{
	Node::render(m, flag);
	if (m_material && (bool)flag == false) {
		if (m_isBlinking) {
			blink();
		}

		// If it's a shadow, render the shadow material, else render the Mesh material
		if (m_shadowMaterial) {
			m_shadowMaterial->render();
		}
		else {
			m_material->render();
		}

	}
	load((bool)flag);
}

/**
* @brief Start blinking the mesh. 
* 
* This method saves the original material, changes the material to a clone of the original material which is used for blinking and sets the blinking flag to ture.
*/
void LIB_API Mesh::startBlink()
{
	saveMaterial();
	m_material = m_material->clone();
	m_isBlinking = true;
}

/**
* @brief Stop blinking the mesh. 
* This method deletes the blinking material, resets the original material and sets the blinking flag to false.
*/
void LIB_API Mesh::stopBlink()
{
	if (m_isBlinking) {
		// Free memory
		delete m_material;
		m_material = nullptr;

		resetMaterial();
		m_isBlinking = false;
	}
}


/**
* @brief Save the original material of the mesh.
*/
void LIB_API Mesh::saveMaterial()
{
	previousMat = m_material;
}

/**
* @brief Check if the mesh is blinking.
* @return True if the mesh is blinking, false otherwise.
*/
bool LIB_API Mesh::isBlinking() const
{
	return m_isBlinking;
}

/**
* @brief Check if the mesh is casting a shadow.
* @return True if the mesh is casting a shadow, false otherwise.
*/
bool LIB_API Mesh::isCastingShadow()
{
	return m_isCastingShadow;
}

/**
* @brief Set whether the mesh should be casting a shadow.
* @param flag True if the mesh should cast a shadow, false otherwise.
*/
void LIB_API Mesh::isCastingShadow(bool flag)
{
	m_isCastingShadow = flag;
}

/**
* @brief Reset the original material of the mesh.
*/
void LIB_API Mesh::resetMaterial() {
	m_material = previousMat;
	previousMat = nullptr;
}

/**
* @brief Constructor for the Mesh class.
* @param name The name of the mesh.
* @param matrix The initial transformation matrix of the mesh relative to its parent node.
* @param material The material of the mesh. Defaults to the default material.
*/
LIB_API Mesh::Mesh(const std::string& name, const glm::mat4& matrix, Material* material)
	: Node(name, matrix), m_material{ material }, m_isBlinking{ false }, previousMat{ nullptr }, m_shadowMaterial{ nullptr }, m_isCastingShadow{ true } {

}

/**
* @brief Destructor for the Mesh class.
*/
LIB_API Mesh::~Mesh() {
	
}
