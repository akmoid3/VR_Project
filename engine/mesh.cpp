/**
* @file		mesh.cpp
* @ingroup Engine
* @brief		Implementation of the Mesh class
*
* @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
*/
#include "engine.h"

#define GLEW_STATIC
#include <GL/glew.h>

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
 * This method uses the most detailed level of detail (LOD) to load the mesh for rendering. It sets up vertex array objects (VAOs) and vertex buffer objects (VBOs) to store and render the mesh data. 
 * 
 */

// Non dovrebbe esistere da fare nel render
// Fare un metodo load lod, qui dentro inizializziamo i vbo e i vao e li popoliamo con i dati nel lod (che passimao come parametro), si chiama una sola volta
void LIB_API Mesh::loadLod() {
	std::vector<glm::vec3> vertex = lods[0].vertex();
	std::vector<glm::vec2> coordtext = lods[0].coordText();
	std::vector<glm::vec3> normal = lods[0].normals();
	std::vector<glm::uvec3> faces = lods[0].faces();

	// VAO SETUP
	// Generate a vertex array object and bind it:
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VERTEX VBO
	// Generate a vertex buffer and bind it:
	glGenBuffers(1, &vbo_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);

	// Copy the vertex data from system to video memory:
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), vertex.data(), GL_STATIC_DRAW);
	
	// Tell OpenGL that you want to use vertex arrays for the given attribute:
	//glEnableClientState(GL_VERTEX_ARRAY);

	//glVertexPointer(3, GL_FLOAT, 0, nullptr); // Tells OpenGL to use last bound vbo
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// TEXTURE COORDINATES VBO
	glGenBuffers(1, &vbo_textcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_textcoord);

	// Copy the vertex data from system to video memory:
	glBufferData(GL_ARRAY_BUFFER, coordtext.size() * sizeof(glm::vec2), coordtext.data(), GL_STATIC_DRAW);
	
	// Tell OpenGL that you want to use vertex arrays for the given attribute:
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);


	// NORMAL VECTOR VBO
	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);

	// Copy the vertex data from system to video memory:
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3),
		normal.data(), GL_STATIC_DRAW);

	// Tell OpenGL that you want to use vertex arrays for the given attribute:
	//glEnableClientState(GL_NORMAL_ARRAY);

	//glNormalPointer(GL_FLOAT, 0, nullptr);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);
	
	// FACE INDEX ARRAY VBO
	// Generate a vertex buffer and bind it:
	
	glGenBuffers(1, &vbo_face);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_face);

	// Copy the face index data from system to video memory:
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * 3 * sizeof(unsigned int), faces.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	//glEnableVertexAttribArray(0);
	
	
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
 * Then, it checks if the mesh has a shadow material. If it does, it renders the shadow material, otherwise it renders the mesh material. Finally, it renders the mesh.
 * If the object selection flag is set to true, the method sets the color of the object according to its ID for color picking. 
 * @param m The result of multiplying the inverse of the final camera matrix by the final position matrix in world coordinates of this object.
 * @param flag A flag for object selection, indicating whether to apply the color to the mesh. If the flag is set, it applies the ID color to the mesh. 
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
	//load((bool)flag); // Non va bene, implementare direttamente il render qui, glbind vertex array vao , gl draw elemnts 2 righe

	
	// Is the flag for the object selection is set to true, it applies the color of the ID when rendering the mesh
	unsigned char rgba[4];
	if (flag) {
		unsigned int idValue = id();
		memcpy(rgba, &idValue, sizeof(idValue)); // Copy the ID to the RGBA array
		glColor4ubv(rgba);

	}

	// Use the face index array vbo inside vao for rendering:
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, lods[0].faces().size() * 3, GL_UNSIGNED_INT, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, lods[0].vertex().size());
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
 * @brief Dispose of the vertex buffer objects (VBOs) and vertex array object (VAO) associated with the mesh.
 *
 * This method deletes the VBOs and VAO used by the mesh to store and render its vertex data. It releases the GPU memory occupied by these objects.
 * Call this method when the mesh is no longer needed to free up resources and prevent memory leaks.
 */
void LIB_API Mesh::disposeBuffers()
{
	glDeleteBuffers(1, &vbo_vertex);
	glDeleteBuffers(1, &vbo_textcoord);
	glDeleteBuffers(1, &vbo_normal);
	glDeleteBuffers(1, &vbo_face);
	glDeleteVertexArrays(1, &vao);
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
	: Node(name, matrix), m_material{ material }, m_isBlinking{ false }, previousMat{ nullptr }, m_shadowMaterial{ nullptr }, m_isCastingShadow{ true }, vao{ 0 }, vbo_face{0}, vbo_normal{ 0 }, vbo_textcoord{ 0 }, vbo_vertex{ 0 } {

}

/**
* @brief Destructor for the Mesh class.
*/
LIB_API Mesh::~Mesh() {
	disposeBuffers();
}
