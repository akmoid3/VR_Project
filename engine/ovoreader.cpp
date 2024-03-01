/**
 * @file	   ovoreader.cpp
 * @ingroup Engine
 * @brief	Implementation of the Ovoreader class
 *
 * @author	Gruppo05 (C) SUPSI [mariagrazia.corino@student.supsi.ch, kristian.boldini@student.supsi.ch, ahmed.elaidy@student.supsi.ch]
 */



 //////////////
 // #INCLUDE //
 //////////////
#include "engine.h"
 // GLM:
#define GLM_FORCE_CTOR_INIT         // Constructors no longer init to identity since 0.9.9

#include <glm/glm.hpp>
#include <glm/gtc/packing.hpp>

// C/C++:
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits.h>

using namespace std;

/////////////
// #DEFINE //
/////////////

// General:
#define APP_NAME "OVO Reader v0.8.1r"

// Macro for printing an OvMatrix4 to console:   
#define MAT2STR(m) std::cout << "   Matrix  . . . :  \t" << m[0][0] << "\t" << m[1][0] << "\t" << m[2][0] << "\t" << m[3][0] << std::endl \
                           << "                    \t" << m[0][1] << "\t" << m[1][1] << "\t" << m[2][1] << "\t" << m[3][1] << std::endl \
                           << "                    \t" << m[0][2] << "\t" << m[1][2] << "\t" << m[2][2] << "\t" << m[3][2] << std::endl \
                           << "                    \t" << m[0][3] << "\t" << m[1][3] << "\t" << m[2][3] << "\t" << m[3][3] << std::endl

/**
 * @cond
 */
// Stripped-down redefinition of OvObject (just for the chunk IDs):
class LIB_API OvObject {
public:
	enum class Type : int  ///< Type of entities
	{
		// Foundation types:
		OBJECT = 0,
		NODE,
		OBJECT2D,
		OBJECT3D,
		LIST,

		// Derived classes:
		BUFFER,
		SHADER,
		TEXTURE,
		FILTER,
		MATERIAL,
		FBO,
		QUAD,
		BOX,
		SKYBOX,
		FONT,
		CAMERA,
		LIGHT,
		BONE,
		MESH,       // Keep them...
		SKINNED, // ...consecutive
		INSTANCED,
		PIPELINE,
		EMITTER,

		// Animation type
		ANIM,

		// Physics related:
		PHYSICS,

		// Terminator:
		LAST,
	};
};

// Stripped-down redefinition of OvMesh (just for the subtypes):
class LIB_API OvMesh {
public:
	enum class Subtype : int ///< Kind of mesh
	{
		// Foundation types:
		DEFAULT = 0,
		NORMALMAPPED,
		TESSELLATED,

		// Terminator:
		LAST,
	};
};

// Stripped-down redefinition of OvLight (just for the subtypes):
class LIB_API OvLight {
public:
	enum class Subtype : int ///< Kind of light
	{
		// Foundation types:
		OMNI = 0,
		DIRECTIONAL,
		SPOT,

		// Terminator:
		LAST,
	};
};

/**
 * @endcond
 */

/////////////////////////////
// BODY OF CLASS OvoReader //
/////////////////////////////


 // Recursive loading function:
/**
* @brief Recursively loads data.
*
* Used by the load() method of this class.
* 
* @param data The data to load.
* @param position The position in the data to start loading from.
* @return A pointer to the root node of the scene.
*/
Node LIB_API* Ovoreader::recursiveLoad(const char* data, unsigned int& position) {
	bool verbose = false;
	// Extract node information:
	unsigned int nrOfChildren = 0;
	Node* thisNode = nullptr;
	unsigned int chunkId = 0;
	unsigned int chunkSize = 0;

	memcpy(&chunkId, data + position, sizeof(unsigned int));
	position += sizeof(unsigned int);
	memcpy(&chunkSize, data + position, sizeof(unsigned int));
	position += sizeof(unsigned int);
	std::cout << "Recursive" << chunkId << " " << chunkSize << " " << position << std::endl;



	// Parse chunk information according to its type:
	switch ((OvObject::Type)chunkId) {
		///////////////////////////////
		//
		/////////////////////////////
	case OvObject::Type::NODE: //
	{
		std::cout << "node]" << endl;

		// Node name:
		char nodeName[FILENAME_MAX];
		strcpy(nodeName, data + position);
		std::cout << "   Name  . . . . :  " << nodeName << endl;
		position += (unsigned int)strlen(nodeName) + 1;

		// Node matrix:
		glm::mat4 matrix;
		memcpy(&matrix, data + position, sizeof(glm::mat4));
		position += sizeof(glm::mat4);

		// Nr. of children nodes:
		unsigned int children;
		memcpy(&children, data + position, sizeof(unsigned int));
		std::cout << "   Nr. children  :  " << children << endl;
		position += sizeof(unsigned int);

		// Optional target node, [none] if not used:
		char targetName[FILENAME_MAX];
		strcpy(targetName, data + position);
		std::cout << "   Target node . :  " << targetName << endl;
		position += (unsigned int)strlen(targetName) + 1;

		nrOfChildren = children;
		thisNode = new Node(nodeName, matrix);
	}
	break;

	////////////////////////////////
	case OvObject::Type::MESH:  //
	case OvObject::Type::SKINNED: {
		// Both standard and skinned meshes are handled through this case:
		bool isSkinned = false;
		if ((OvObject::Type)chunkId == OvObject::Type::SKINNED) {
			isSkinned = true;
			std::cout << "skinned mesh]" << endl;
		}
		else
			std::cout << "mesh]" << endl;

		// Mesh name:
		char meshName[FILENAME_MAX];
		strcpy(meshName, data + position);
		position += (unsigned int)strlen(meshName) + 1;
		std::cout << "   Name  . . . . :  " << meshName << endl;

		// Mesh matrix:
		glm::mat4 matrix;
		memcpy(&matrix, data + position, sizeof(glm::mat4));
		position += sizeof(glm::mat4);

		// Mesh nr. of children nodes:
		unsigned int children;
		memcpy(&children, data + position, sizeof(unsigned int));
		std::cout << "   Nr. children  :  " << children << endl;
		position += sizeof(unsigned int);

		// Optional target node, or [none] if not used:
		char targetName[FILENAME_MAX];
		strcpy(targetName, data + position);
		std::cout << "   Target node . :  " << targetName << endl;
		position += (unsigned int)strlen(targetName) + 1;

		// Mesh subtype (see OvMesh SUBTYPE enum):
		unsigned char subtype;
		memcpy(&subtype, data + position, sizeof(unsigned char));
		char subtypeName[FILENAME_MAX];
		switch ((OvMesh::Subtype)subtype) {
		case OvMesh::Subtype::DEFAULT:
			strcpy(subtypeName, "standard");
			break;
		case OvMesh::Subtype::NORMALMAPPED:
			strcpy(subtypeName, "normal-mapped");
			break;
		case OvMesh::Subtype::TESSELLATED:
			strcpy(subtypeName, "tessellated");
			break;
		default:
			strcpy(subtypeName, "UNDEFINED");
		}
		std::cout << "   Subtype . . . :  " << (int)subtype << " (" << subtypeName << ")" << endl;
		position += sizeof(unsigned char);

		// Material name, or [none] if not used:
		char materialName[FILENAME_MAX];
		strcpy(materialName, data + position);
		std::cout << "   Material  . . :  " << materialName << endl;
		position += (unsigned int)strlen(materialName) + 1;

		// Mesh bounding sphere radius:
		float radius;
		memcpy(&radius, data + position, sizeof(float));
		std::cout << "   Radius  . . . :  " << radius << endl;
		position += sizeof(float);

		// Mesh bounding box minimum corner:
		glm::vec3 bBoxMin;
		memcpy(&bBoxMin, data + position, sizeof(glm::vec3));
		std::cout << "   BBox minimum  :  " << bBoxMin.x << ", " << bBoxMin.y << ", " << bBoxMin.z << endl;
		position += sizeof(glm::vec3);

		// Mesh bounding box maximum corner:
		glm::vec3 bBoxMax;
		memcpy(&bBoxMax, data + position, sizeof(glm::vec3));
		std::cout << "   BBox maximum  :  " << bBoxMax.x << ", " << bBoxMax.y << ", " << bBoxMax.z << endl;
		position += sizeof(glm::vec3);


		// MESH CREATION
		nrOfChildren = children;

		Material* material = nullptr;
		for (auto& pair : m_materials) {
			if (pair.first.compare(materialName) == 0) {
				material = pair.second;
			}
		}

		thisNode = new Mesh(meshName, matrix, material);
		Mesh* meshNode = dynamic_cast<Mesh*>(thisNode);

		// Optional physics properties:
		unsigned char hasPhysics;
		memcpy(&hasPhysics, data + position, sizeof(unsigned char));
		std::cout << "   Physics . . . :  " << (int)hasPhysics << endl;
		position += sizeof(unsigned char);
		if (hasPhysics) {
			/**
			 * Mesh physics properties.
			 */
			struct PhysProps {
				// Pay attention to 16 byte alignement (use padding):
				unsigned char type;
				unsigned char contCollisionDetection;
				unsigned char collideWithRBodies;
				unsigned char hullType;

				// Vector data:
				glm::vec3 massCenter;

				// Mesh properties:
				float mass;
				float staticFriction;
				float dynamicFriction;
				float bounciness;
				float linearDamping;
				float angularDamping;
				unsigned int nrOfHulls;
				unsigned int _pad;

				// Pointers:
				void* physObj;
				void* hull;
			};

			PhysProps mp;
			memcpy(&mp, data + position, sizeof(PhysProps));
			position += sizeof(PhysProps);
			std::cout << "      Type . . . :  " << (int)mp.type << endl;
			std::cout << "      Hull type  :  " << (int)mp.hullType << endl;
			std::cout << "      Cont. coll.:  " << (int)mp.contCollisionDetection << endl;
			std::cout << "      Col. bodies:  " << (int)mp.collideWithRBodies << endl;
			std::cout << "      Center . . :  " << mp.massCenter.x << ", " << mp.massCenter.y << ", "
				<< mp.massCenter.z << endl;
			std::cout << "      Mass . . . :  " << mp.mass << endl;
			std::cout << "      Static . . :  " << mp.staticFriction << endl;
			std::cout << "      Dynamic  . :  " << mp.dynamicFriction << endl;
			std::cout << "      Bounciness :  " << mp.bounciness << endl;
			std::cout << "      Linear . . :  " << mp.linearDamping << endl;
			std::cout << "      Angular  . :  " << mp.angularDamping << endl;
			std::cout << "      Nr. hulls  :  " << mp.nrOfHulls << endl;

			// Custom hull(s) used?
			if (mp.nrOfHulls) {
				for (unsigned int c = 0; c < mp.nrOfHulls; c++) {
					if (verbose)
						std::cout << "         Hull  . :  " << c + 1 << endl;

					// Hull number of vertices:
					unsigned int nrOfVertices;
					memcpy(&nrOfVertices, data + position, sizeof(unsigned int));
					if (verbose)
						std::cout << "         Nr. v.  :  " << nrOfVertices << endl;
					position += sizeof(unsigned int);

					// Hull number of faces:
					unsigned int nrOfFaces;
					memcpy(&nrOfFaces, data + position, sizeof(unsigned int));
					if (verbose)
						std::cout << "         Nr. f.  :  " << nrOfFaces << endl;
					position += sizeof(unsigned int);

					// Hull centroid:
					glm::vec3 centroid;
					memcpy(&centroid, data + position, sizeof(glm::vec3));
					if (verbose)
						std::cout << "         Centr.  :  " << centroid.x << ", " << centroid.y << ", "
						<< centroid.z << endl;
					position += sizeof(glm::vec3);

					// Iterate through hull vertices:
					for (unsigned int c = 0; c < nrOfVertices; c++) {
						// Vertex coords:
						glm::vec3 vertex;
						memcpy(&vertex, data + position, sizeof(glm::vec3));
						if (verbose)
							std::cout << "         Data  . :  v" << c << " " << vertex.x << ", " << vertex.y << ", "
							<< vertex.z << endl;
						position += sizeof(glm::vec3);
					}

					// Iterate through hull faces:
					for (unsigned int c = 0; c < nrOfFaces; c++) {
						unsigned int face[3];
						memcpy(face, data + position, sizeof(unsigned int) * 3);
						if (verbose)
							std::cout << "         Data  . :  f" << c << " (" << face[0] << ", " << face[1] << ", "
							<< face[2] << ")" << endl;
						position += sizeof(unsigned int) * 3;
					}
				}
			}
		}

		// Nr. of LODs:
		unsigned int LODs;
		memcpy(&LODs, data + position, sizeof(unsigned int));
		std::cout << "   Nr. of LODs   :  " << LODs << endl;
		position += sizeof(unsigned int);

		// For each LOD...:
		vector<unsigned int> verticesPerLOD(LODs); // Let's store this information for the skinned part, in case

		std::vector <VertexData> vertexDataVector;
		for (unsigned int l = 0; l < LODs; l++) {

			std::cout << "      LOD . . :  " << l + 1 << "/" << LODs << endl;

			// Nr. of vertices:
			unsigned int vertices, faces;
			memcpy(&vertices, data + position, sizeof(unsigned int));
			std::cout << "   Nr. vertices  :  " << vertices << endl;
			position += sizeof(unsigned int);
			verticesPerLOD[l] = vertices;

			// ...and faces:
			memcpy(&faces, data + position, sizeof(unsigned int));
			std::cout << "   Nr. faces . . :  " << faces << endl;
			position += sizeof(unsigned int);

			// Dichiarazione lod class
			Lod lod;
			// Interleaved and compressed vertex/normal/UV/tangent data:
			for (unsigned int c = 0; c < vertices; c++) {
				if (verbose)
					std::cout << "   Vertex data . :  v" << c << endl;

				// Vertex coords:
				glm::vec3 vertex;
				memcpy(&vertex, data + position, sizeof(glm::vec3));
				if (verbose)
					std::cout << "      xyz  . . . :  " << vertex.x << ", " << vertex.y << ", " << vertex.z << endl;
				position += sizeof(glm::vec3);

				// Vertex normal:
				unsigned int normalData;
				memcpy(&normalData, data + position, sizeof(unsigned int));
				glm::vec4 normal = glm::unpackSnorm3x10_1x2(normalData);
				if (verbose) {
					std::cout << "      normal . . :  " << normal.x << ", " << normal.y << ", " << normal.z << endl;
				}
				position += sizeof(unsigned int);

				// Texture coordinates:
				unsigned int textureData;
				memcpy(&textureData, data + position, sizeof(unsigned int));
				glm::vec2 uv = glm::unpackHalf2x16(textureData);
				if (verbose) {
					std::cout << "      uv . . . . :  " << uv.x << ", " << uv.y << endl;
				}
				position += sizeof(unsigned int);

				// Tangent vector:
				unsigned int tangentData;
				memcpy(&tangentData, data + position, sizeof(unsigned int));
				if (verbose) {
					glm::vec4 tangent = glm::unpackSnorm3x10_1x2(tangentData);
					std::cout << "      tangent  . :  " << tangent.x << ", " << tangent.y << ", " << tangent.z
						<< ", sign: " << tangent.w << endl;
				}
				position += sizeof(unsigned int);

				// Creare 3 array separati, poi da copiare nei tre vbo di la stessa cosa per le facce
				// Eliminare VertexData
				VertexData vertexData;
				vertexData.posVertex(vertex);
				vertexData.normal(normal);
				vertexData.coordText(uv);
				lod.addVertexData(vertexData);
			}

			// Faces:
			for (unsigned int c = 0; c < faces; c++) {
				// Face indexes:
				unsigned int face[3];
				memcpy(face, data + position, sizeof(unsigned int) * 3);
				lod.addFaces(glm::uvec3{ face[0],face[1],face[2] });
				position += sizeof(unsigned int) * 3;
				if (verbose) {
					std::cout << "   Face data . . :  f" << c << " (" << face[0] << ", " << face[1] << ", "
						<< face[2] << ")" << endl;

				}

			}
			meshNode->addLod(lod);
		}

		// Extra information for skinned meshes:
		if (isSkinned) {
			// Initial mesh pose matrix:
			glm::mat4 poseMatrix;
			memcpy(&poseMatrix, data + position, sizeof(glm::mat4));
			if (verbose)
				MAT2STR(poseMatrix);
			position += sizeof(glm::vec4);

			// Bone list:
			unsigned int nrOfBones;
			memcpy(&nrOfBones, data + position, sizeof(unsigned int));
			std::cout << "   Nr. bones . . :  " << nrOfBones << endl;
			position += sizeof(unsigned int);

			// For each bone...:
			for (unsigned int c = 0; c < nrOfBones; c++) {
				// Bone name:
				char boneName[FILENAME_MAX];
				strcpy(boneName, data + position);
				std::cout << "      Bone name  :  " << boneName << " (" << c << ")" << endl;
				position += (unsigned int)strlen(boneName) + 1;

				// Initial bone pose matrix (already inverted):
				glm::mat4 boneMatrix;
				memcpy(&boneMatrix, data + position, sizeof(glm::mat4));
				if (verbose)
					MAT2STR(boneMatrix);
				position += sizeof(glm::mat4);
			}

			// For each LOD...:
			for (unsigned int l = 0; l < LODs; l++) {
				std::cout << "      LOD . . :  " << l + 1 << "/" << LODs << endl;

				// Per vertex bone weights and indexes:
				for (unsigned int c = 0; c < verticesPerLOD[l]; c++) {
					if (verbose)
						std::cout << "   Bone data . . :  v" << c << endl;

					// Bone indexes:
					unsigned int boneIndex[4];
					memcpy(boneIndex, data + position, sizeof(unsigned int) * 4);
					if (verbose)
						std::cout << "      index  . . :  " << boneIndex[0] << ", " << boneIndex[1] << ", "
						<< boneIndex[2] << ", " << boneIndex[3] << endl;
					position += sizeof(unsigned int) * 4;

					// Bone weights:
					unsigned short boneWeightData[4];
					memcpy(boneWeightData, data + position, sizeof(unsigned short) * 4);
					if (verbose) {
						glm::vec4 boneWeight;
						boneWeight.x = glm::unpackHalf1x16(boneWeightData[0]);
						boneWeight.y = glm::unpackHalf1x16(boneWeightData[1]);
						boneWeight.z = glm::unpackHalf1x16(boneWeightData[2]);
						boneWeight.w = glm::unpackHalf1x16(boneWeightData[3]);
						std::cout << "      weight . . :  " << boneWeight.x << ", " << boneWeight.y << ", "
							<< boneWeight.z << ", " << boneWeight.w << endl;
					}
					position += sizeof(unsigned short) * 4;
				}
			}
		}

	}
								break;


								//////////////////////////////
	case OvObject::Type::LIGHT: //
	{
		std::cout << "light]" << endl;

		// Light name:
		char lightName[FILENAME_MAX];
		strcpy(lightName, data + position);
		std::cout << "   Name  . . . . :  " << lightName << endl;
		position += (unsigned int)strlen(lightName) + 1;

		// Light matrix:
		glm::mat4 matrix;
		memcpy(&matrix, data + position, sizeof(glm::mat4));
		position += sizeof(glm::mat4);

		// Nr. of children nodes:
		unsigned int children;
		memcpy(&children, data + position, sizeof(unsigned int));
		std::cout << "   Nr. children  :  " << children << endl;
		position += sizeof(unsigned int);

		// Optional target node name, or [none] if not used:
		char targetName[FILENAME_MAX];
		strcpy(targetName, data + position);
		std::cout << "   Target node . :  " << targetName << endl;
		position += (unsigned int)strlen(targetName) + 1;

		// Light subtype (see OvLight SUBTYPE enum):
		unsigned char subtype;
		memcpy(&subtype, data + position, sizeof(unsigned char));
		char subtypeName[FILENAME_MAX];
		switch ((OvLight::Subtype)subtype) {
		case OvLight::Subtype::DIRECTIONAL:
			strcpy(subtypeName, "directional");
			break;
		case OvLight::Subtype::OMNI:
			strcpy(subtypeName, "omni");
			break;
		case OvLight::Subtype::SPOT:
			strcpy(subtypeName, "spot");
			break;
		default:
			strcpy(subtypeName, "UNDEFINED");
		}
		std::cout << "   Subtype . . . :  " << (int)subtype << " (" << subtypeName << ")" << endl;
		position += sizeof(unsigned char);

		// Light color:
		glm::vec3 color;
		memcpy(&color, data + position, sizeof(glm::vec3));
		std::cout << "   Color . . . . :  " << color.r << ", " << color.g << ", " << color.b << endl;
		position += sizeof(glm::vec3);

		// Influence radius:
		float radius;
		memcpy(&radius, data + position, sizeof(float));
		std::cout << "   Radius  . . . :  " << radius << endl;
		position += sizeof(float);

		// Direction:
		glm::vec3 direction;
		memcpy(&direction, data + position, sizeof(glm::vec3));
		std::cout << "   Direction . . :  " << direction.r << ", " << direction.g << ", " << direction.b << endl;
		position += sizeof(glm::vec3);

		// Cutoff:
		float cutoff;
		memcpy(&cutoff, data + position, sizeof(float));
		std::cout << "   Cutoff  . . . :  " << cutoff << endl;
		position += sizeof(float);

		// Exponent:
		float spotExponent;
		memcpy(&spotExponent, data + position, sizeof(float));
		std::cout << "   Spot exponent :  " << spotExponent << endl;
		position += sizeof(float);

		// Cast shadow flag:
		unsigned char castShadows;
		memcpy(&castShadows, data + position, sizeof(unsigned char));
		std::cout << "   Cast shadows  :  " << (int)castShadows << endl;
		position += sizeof(unsigned char);

		// Volumetric lighting flag:
		unsigned char isVolumetric;
		memcpy(&isVolumetric, data + position, sizeof(unsigned char));
		std::cout << "   Volumetric  . :  " << (int)isVolumetric << endl;
		position += sizeof(unsigned char);

		nrOfChildren = children;

		switch ((OvLight::Subtype)subtype) {
		case OvLight::Subtype::DIRECTIONAL:
		{
			thisNode = new DirectionalLight(lightName, matrix);
			DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(thisNode);
			directionalLight->ambient(color.r, color.g, color.b, 1.0f);
			directionalLight->diffuse(color.r, color.g, color.b, 1.0f);
			directionalLight->specular(color.r, color.g, color.b, 1.0f);
		}
		break;
		case OvLight::Subtype::OMNI:
		{
			thisNode = new OmniLight(lightName, matrix);
			OmniLight* omniLight = dynamic_cast<OmniLight*>(thisNode);
			omniLight->ambient(color.r, color.g, color.b, 1.0f);
			omniLight->diffuse(color.r, color.g, color.b, 1.0f);
			omniLight->specular(color.r, color.g, color.b, 1.0f);
			omniLight->cutoff(cutoff);
		}
		break;
		case OvLight::Subtype::SPOT:
		{
			thisNode = new SpotLight(lightName, matrix);
			SpotLight* spotLight = dynamic_cast<SpotLight*>(thisNode);
			spotLight->ambient(color.r, color.g, color.b, 1.0f);
			spotLight->diffuse(color.r, color.g, color.b, 1.0f);
			spotLight->specular(color.r, color.g, color.b, 1.0f);
			spotLight->cutoff(cutoff);
			spotLight->direction(direction.x, direction.y, direction.z);
		}
		break;
		default:
			break;
		}
	}
	break;


	/////////////////////////////
	case OvObject::Type::BONE: //
	{
		std::cout << "bone]" << endl;

		// Bone name:
		char boneName[FILENAME_MAX];
		strcpy(boneName, data + position);
		std::cout << "   Name  . . . . :  " << boneName << endl;
		position += (unsigned int)strlen(boneName) + 1;

		// Bone matrix:
		glm::mat4 matrix;
		memcpy(&matrix, data + position, sizeof(glm::mat4));
		if (verbose)
			MAT2STR(matrix);
		position += sizeof(glm::mat4);

		// Nr. of children nodes:
		unsigned int children;
		memcpy(&children, data + position, sizeof(unsigned int));
		std::cout << "   Nr. children  :  " << children << endl;
		position += sizeof(unsigned int);

		// Optional target node, or [none] if not used:
		char targetName[FILENAME_MAX];
		strcpy(targetName, data + position);
		std::cout << "   Target node . :  " << targetName << endl;
		position += (unsigned int)strlen(targetName) + 1;

		// Mesh bounding box minimum corner:
		glm::vec3 bBoxMin;
		memcpy(&bBoxMin, data + position, sizeof(glm::vec3));
		std::cout << "   BBox minimum  :  " << bBoxMin.x << ", " << bBoxMin.y << ", " << bBoxMin.z << endl;
		position += sizeof(glm::vec3);

		// Mesh bounding box maximum corner:
		glm::vec3 bBoxMax;
		memcpy(&bBoxMax, data + position, sizeof(glm::vec3));
		std::cout << "   BBox maximum  :  " << bBoxMax.x << ", " << bBoxMax.y << ", " << bBoxMax.z << endl;
		position += sizeof(glm::vec3);
	}
	break;


	///////////
	default: //
		std::cout << "UNKNOWN]" << endl;
		std::cout << "ERROR: corrupted or bad data in file " << data << endl;
		break;
	}
	// Go recursive when child nodes are avaialble:
	// Update position for next chunk:
	//position += chunkSize;

	std::cout << "Nome : " + thisNode->name() << std::endl;
	if (nrOfChildren != 0) {
		while (thisNode->children().size() < nrOfChildren) {
			Node* childNode = recursiveLoad(data, position);
			thisNode->addChild(childNode);
		}
	}

	// Done:

	return thisNode;
}

/**
 * @brief Loads the scene from a file in the OVO format.
 *
 * This method reads a scene from a file in the OVO (Object Virtual Object) format. 
 * It parses the file, extracts information about textures and materials, and creates corresponding objects, saving them in dedicated vectors. 
 * The method also creates Mesh and Light objects based on the information read from the file, assembling the scene's structure. 
 * The resulting scene is represented by a tree structure, and a pointer to the root node of this structure is returned.
 * It uses the recursive method RecursiveLoad() internally.
 * 
 * @param fileName The name of the file to load.
 * @return A pointer to the root node of the scene.
 */

Node LIB_API* Ovoreader::load(std::string fileName) {

	FILE* dat = fopen(fileName.c_str(), "rb");
	if (dat == nullptr) {
		std::cout << "ERROR: unable to open file '" << fileName << "'" << endl;
		return NULL;
	}

	// trova dimensione del file
	fseek(dat, 0, SEEK_END);
	long fileSize = ftell(dat);
	fseek(dat, 0, SEEK_SET);
	// carica in memoria
	char* data = new char[fileSize];
	fread(data, fileSize, sizeof(char), dat);
	fclose(dat);
	unsigned int chunkSize = 0;
	unsigned int chunkId = 0;
	unsigned int position = 0;
	printf(" % d", fileSize);

	while ((OvObject::Type)chunkId == OvObject::Type::OBJECT || (OvObject::Type)chunkId == OvObject::Type::MATERIAL) {
		memcpy(&chunkId, data + position, sizeof(unsigned int));
		position += sizeof(unsigned int);
		memcpy(&chunkSize, data + position, sizeof(unsigned int));
		position += sizeof(unsigned int);
		std::cout << chunkId << " " << chunkSize << " " << position << std::endl;
		switch ((OvObject::Type)chunkId) {
			///////////////////////////////
		case OvObject::Type::OBJECT: //
		{
			std::cout << "version]" << endl;

			// OVO revision number:
			unsigned int versionId;
			memcpy(&versionId, data + position, sizeof(unsigned int));
			std::cout << "   Version . . . :  " << versionId << endl;
			position += sizeof(unsigned int);
		}
		break;

		/////////////////////////////////
		case OvObject::Type::MATERIAL: //
		{
			std::cout << "material]" << endl;

			// Material name:
			char materialName[FILENAME_MAX];
			strcpy(materialName, data + position);
			std::cout << "   Name  . . . . :  " << materialName << endl;
			position += (unsigned int)strlen(materialName) + 1;

			// Material term colors, starting with emissive:
			glm::vec3 emission, albedo;
			memcpy(&emission, data + position, sizeof(glm::vec3));
			std::cout << "   Emission  . . :  " << emission.r << ", " << emission.g << ", " << emission.b << endl;
			position += sizeof(glm::vec3);

			// Albedo:
			memcpy(&albedo, data + position, sizeof(glm::vec3));
			std::cout << "   Albedo  . . . :  " << albedo.r << ", " << albedo.g << ", " << albedo.b << endl;
			position += sizeof(glm::vec3);

			// Roughness factor:
			float roughness;
			memcpy(&roughness, data + position, sizeof(float));
			std::cout << "   Roughness . . :  " << roughness << endl;
			position += sizeof(float);

			// Metalness factor:
			float metalness;
			memcpy(&metalness, data + position, sizeof(float));
			std::cout << "   Metalness . . :  " << metalness << endl;
			position += sizeof(float);

			// Transparency factor:
			float alpha;
			memcpy(&alpha, data + position, sizeof(float));
			std::cout << "   Transparency  :  " << alpha << endl;
			position += sizeof(float);

			// Albedo texture filename, or [none] if not used:
			char textureName[FILENAME_MAX];
			strcpy(textureName, data + position);
			std::cout << "   Albedo tex. . :  " << textureName << endl;
			position += (unsigned int)strlen(textureName) + 1;

			// Normal map filename, or [none] if not used:
			char normalMapName[FILENAME_MAX];
			strcpy(normalMapName, data + position);
			std::cout << "   Normalmap tex.:  " << normalMapName << endl;
			position += (unsigned int)strlen(normalMapName) + 1;

			// Height map filename, or [none] if not used:
			char heightMapName[FILENAME_MAX];
			strcpy(heightMapName, data + position);
			std::cout << "   Heightmap tex.:  " << heightMapName << endl;
			position += (unsigned int)strlen(heightMapName) + 1;

			// Roughness map filename, or [none] if not used:
			char roughnessMapName[FILENAME_MAX];
			strcpy(roughnessMapName, data + position);
			std::cout << "   Roughness tex.:  " << roughnessMapName << endl;
			position += (unsigned int)strlen(roughnessMapName) + 1;

			// Metalness map filename, or [none] if not used:
			char metalnessMapName[FILENAME_MAX];
			strcpy(metalnessMapName, data + position);
			std::cout << "   Metalness tex.:  " << metalnessMapName << endl;
			position += (unsigned int)strlen(metalnessMapName) + 1;

			Material* material = new Material(materialName);

			glm::vec4 ambient{ albedo * 0.2f, 1.0f };
			material->ambient(ambient.r, ambient.g, ambient.b, ambient.a);

			glm::vec4 diffuse{ albedo * 0.6f, 1.0f };
			material->diffuse(diffuse.r, diffuse.g, diffuse.b, diffuse.a);

			glm::vec4 specular{ albedo * 0.4f, 1.0f };
			material->specular(specular.r, specular.g, specular.b, specular.a);

			float shininess{ (1 - glm::sqrt(roughness)) * 128.0f };
			material->shininess(shininess);

			material->emission(emission.r, emission.g, emission.b, 1.0f);

			// ADD TEXTURE
			if (strcmp(textureName, "[none]") != 0) {
				Texture* texture = nullptr;
				for (auto& pair : m_textures) {
					if (pair.first.compare(textureName) == 0) {
						texture = pair.second;
					}
				}

				if(texture == nullptr)
				{
					texture = new Texture(textureName);
					texture->load();
					m_textures.push_back(std::make_pair(textureName, texture));
				}

				material->setTexture(texture);
			}


			// Add material
			m_materials.push_back(std::make_pair(materialName, material));
		}

		break;
		default:
			break;
		}
		memcpy(&chunkId, data + position, sizeof(unsigned int));

	}
	std::cout << "Fuori while " << chunkId << " " << chunkSize << " " << position << std::endl;
	Node* root = recursiveLoad(data, position);
	std::cout << "FINITO LOAD" << std::endl;
	delete[] data;
	return root;
}

/**
 * @brief Gets the textures.
 *
 * @return A vector of pairs, where each pair contains a string representing a name and a pointer to a Texture object.
 */
std::vector<std::pair<std::string, Texture*>> LIB_API Ovoreader::textures()
{
	return m_textures;
}

/**
 * @brief Deletes dynamically allocated Material and Texture objects.
 *
 * This function iterates through the stored Material and Texture objects
 * in the respective vectors and deletes them to prevent memory leaks. It is called in the Engine::free();
 *
 */
void LIB_API Ovoreader::deleteTexturesAndMaterials()
{
	// Delete the dynamically allocated Material objects
	for (auto& entry : m_materials) {
		delete entry.second;
	}

	// Delete the dynamically allocated Texture objects
	for (auto& entry : m_textures) {
		delete entry.second;
	}
}
