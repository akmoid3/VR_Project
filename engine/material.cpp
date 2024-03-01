/**
 * @file		material.cpp
 * @ingroup Engine
 * @brief	Implementation of the Material class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#include "engine.h"
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

 ///////////////////////////
 // BODY OF CLASS Material//
 ///////////////////////////

 /**
  * @brief Constructor for the Material class.
  * @details This constructor initializes the material with a given name and default properties.
  *
  * The default properties are as follows:
  * - Ambient color: glm::vec3{0.5f}, Alpha: 1.0f
  * - Diffuse color: glm::vec3{0.5f}, Alpha: 1.0f
  * - Emission color: glm::vec3{0.5f}, Alpha: 1.0f
  * - Specular color: glm::vec3{0.5f}, Alpha: 1.0f
  * - Shininess: 50.0f
  * - Texture: nullptr (no texture)
  *
  * @param name The name of the material.
  */
LIB_API Material::Material(const std::string& name) : Object(name), m_ambient{ glm::vec3{ 0.5f }, 1.0f },
m_diffuse{ glm::vec3{ 0.5f }, 1.0f }, m_emission{ glm::vec3{ 0.5f }, 1.0f }, m_specular{ glm::vec3{ 0.5f }, 1.0f }, m_shininess{ 50.0f }, m_texture{ nullptr }
{

}

/**
* @brief Destructor for the Material class.
*/
LIB_API Material::~Material()
{
}

/**
 * @brief Set the emission property of the material.
 * @param r The red component of the emission property.
 * @param g The green component of the emission property.
 * @param b The blue component of the emission property.
 * @param a The alpha component of the emission property.
 */
void LIB_API Material::emission(float r, float g, float b, float a)
{
	m_emission = glm::vec4(r, g, b, a);
}

/**
 * @brief Get the emission property of the material.
 * @return The emission property of the material.
 */
glm::vec4 Material::emission()
{
	return m_emission;
}

/**
 * @brief Set the ambient property of the material.
 * @param r The red component of the ambient property.
 * @param g The green component of the ambient property.
 * @param b The blue component of the ambient property.
 * @param a The alpha component of the ambient property.
 */
void LIB_API Material::ambient(float r, float g, float b, float a)
{
	m_ambient = glm::vec4(r, g, b, a);
}

/**
 * @brief Set the diffuse property of the material.
 * @param r The red component of the diffuse property.
 * @param g The green component of the diffuse property.
 * @param b The blue component of the diffuse property.
 * @param a The alpha component of the diffuse property.
 */
void LIB_API Material::diffuse(float r, float g, float b, float a)
{
	m_diffuse = glm::vec4(r, g, b, a);
}

/**
 * @brief Set the specular property of the material.
 * @param r The red component of the specular property.
 * @param g The green component of the specular property.
 * @param b The blue component of the specular property.
 * @param a The alpha component of the specular property.
 */
void LIB_API Material::specular(float r, float g, float b, float a)
{
	m_specular = glm::vec4(r, g, b, a);

}

/**
 * @brief Set the shininess property of the material.
 * 
 * @param s The shininess property of the material. Value ranges from 0.0 to 128.f.
 */
void LIB_API Material::shininess(float s)
{
	//Max value 128.f
	m_shininess = s;
}

/**
 * @brief Render the material.
 * @details This method sets the material properties for rendering and then renders the texture if it exists.
 * @param matrix This parameter is ignored in this implementation. Default is nullptr..
 * @param arg This parameter is ignored in this implementation. Default is nullptr.
 */
void LIB_API Material::render(const glm::mat4& m, void* arg)
{

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(m_emission));
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(m_ambient));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(m_diffuse));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(m_specular));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);

	if (m_texture != nullptr) {
		m_texture->render(m, arg);
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}
}

/**
 * @brief Set the texture of the material.
 * @param tex The new texture of the material.
 */
void LIB_API Material::setTexture(Texture* tex)
{
	m_texture = tex;
}

/**
 * @brief Clone the material.
 * @details This method creates a new material with the same properties as the current material.
 * @return A pointer to the cloned material.
 */
Material LIB_API* Material::clone() const
{
	Material* new_material = new Material(m_name);
	new_material->m_ambient = m_ambient;
	new_material->m_diffuse = m_diffuse;
	new_material->m_emission = m_emission;
	new_material->m_specular = m_specular;
	new_material->m_shininess = m_shininess;
	new_material->m_texture = m_texture;
	return new_material;
}



