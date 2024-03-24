/**
 * @file		texture.cpp
 * @ingroup Engine
 * @brief	Implementation of the Texture class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */

#define GLEW_STATIC
#include <GL/glew.h>
#include "engine.h"
#include <iostream>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#define FREEIMAGE_LIB 
#include <FreeImage.h>
 // Glew (include it before GL.h):


 // Quick define for the extension:
#define GL_TEXTURE_MAX_ANISOTROPY_EXT        0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT    0x84FF

 ///////////////////////////
 // BODY OF CLASS Material//
 ///////////////////////////

/**
* @brief Constructor for the Texture class.
* @details This constructor initializes the texture with a given name and a default special texture ID of 0.
* @param name The name of the texture.
*/
LIB_API Texture::Texture(const std::string& name) : Object(name), m_texId{ 0 }
{
}

/**
 * @brief Load the texture.
 * @details This method creates and binds a texture, sets the texture settings, checks if anisotropic filtering is supported and applies it if so, loads an image from file, checks if the image is RGB or RGBA, loads the image into OpenGL, flips the image vertically if necessary, and releases the bitmap at the end.
 */
void LIB_API Texture::load()
{

	// Create and bind texture: 
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);

	// Align to 1 byte:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	
	// Change texture settings:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	int anisotropicLevel = 1;
	
	/*if (strstr((const char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic"))
	{
		std::cout << "   Anisotropic filtering supported" << std::endl;
		glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropicLevel);
		std::cout << "   Anisotropic filtering max. level: " << anisotropicLevel << std::endl;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicLevel);
	}*/

	// Load an image from file:
	std::string path = "../ovo/";
	std::string texturePath = path + name();
	std::cout << texturePath << std::endl;
	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(texturePath.c_str(), 0),
		texturePath.c_str());

	// Check if image is RGB or RGBA
	int intFormat = GL_RGB;
	GLenum extFormat = GL_BGR;
	if (FreeImage_GetBPP(bitmap) == 32)
	{
		intFormat = GL_RGBA;
		extFormat = GL_BGRA;
	}

	// FreeImage supports them but they will appear upside-down:  use the FreeImage_FlipVertical() method to fix that.
	FreeImage_FlipVertical(bitmap);
	// Load image into OpenGL: 
	// glTexImage2D(GL_TEXTURE_2D, 0, component, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap),0, format, GL_UNSIGNED_BYTE, (void*) FreeImage_GetBits(bitmap));
	//gluBuild2DMipmaps(GL_TEXTURE_2D, component, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), format, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));
	glGenerateMipmap(GL_TEXTURE_2D);
	// Textures exported via the over3ds plugin are saved as DDS files
	

	// Release bitmap
	FreeImage_Unload(bitmap);
}

/**
 * @brief Delete the texture.
 * @details This method releases unused resources by deleting the texture stored in this object.
 */
void LIB_API Texture::deleteTexture()
{
	// Release unused resources: 
	glDeleteTextures(1, &m_texId);
}

/**
 * @brief Get the texture special ID. 
 * 
 * A value equal to 0 represents an invalid ID, indicating that the texture has not been loaded yet or that the loading process was unsuccessful.
 * @return The texture ID.
 */
unsigned int LIB_API Texture::texId()
{
	return m_texId;
}

/**
 * @brief Render the texture.
 * @details This method binds the texture and enables 2D texturing.
 * @param matrix This parameter is ignored in this implementation. 
 * @param arg This parameter is ignored in this implementation. 
 */
void LIB_API Texture::render(const glm::mat4&, void*)
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
}


