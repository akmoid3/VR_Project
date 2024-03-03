/*
#include "engine.h"
#include <iostream>
#include <cassert> // for assert
#include <cstdlib> // for std::abort

// This method checks for increment (we don't know initial value, some objects might already be initialized)
int test_id() {
	// Create an object
	Node* object1 = new Node("object1", glm::mat4(1.0f));

	// Save id
	unsigned int id1 = object1->id();

	// Create another object
	Object* object2 = new Node("object2", glm::mat4(1.0f));

	// Test increment
	unsigned int id2 = object2->id();
	assert(id2 == id1 + 1);

	delete object1;
	delete object2;

	return 0;
}

int test_getters_and_setters() {
	// Create a node with an identity matrix
	Node* node = new Node("node", glm::mat4(1.0f));

	// Test the name() getter and setter methods in the Object class
	assert(node->name() == "node");
	node->name("newNode");
	assert(node->name() == "newNode");

	// Test the matrix() getter and setter methods in the Node class
	assert(node->matrix() == glm::mat4(1.0f));
	glm::mat4 newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
	node->matrix(newMatrix);
	assert(node->matrix() == newMatrix);

	// Create a parent node and a child node
	Node* parentNode = new Node("parent", glm::mat4(1.0f));
	Node* childNode = new Node("child", glm::mat4(1.0f));

	// Test the parent(Node*) and parent() methods in the Node class
	assert(childNode->parent() == nullptr);
	childNode->parent(parentNode);
	assert(childNode->parent() == parentNode);

	delete node;
	delete parentNode;
	delete childNode;

	return 0;
}


int test_addChild() {
	// Create a parent node with an identity matrix
	Node* parentNode = new Node("parent", glm::mat4(1.0f));

	// Create a child node with a translation matrix
	glm::mat4 childMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
	Node* childNode = new Node("child", childMatrix);

	// Add the child node to the parent node
	parentNode->addChild(childNode);

	// Test that the child node was added to the parent node
	std::vector<Node*> children = parentNode->children();
	assert(children.size() == 1);
	assert(children[0] == childNode);

	// Test that the parent of the child node is the parent node
	assert(childNode->parent() == parentNode);

	delete parentNode;

	return 0;
}

int test_getFinalMatrix() {
	// Create a root node with an identity matrix
	Node* rootNode = new Node("root", glm::mat4(1.0f));

	// Create child nodes for the root node
	glm::mat4 child1Matrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
	Node* child1Node = new Node("child1", child1Matrix);
	rootNode->addChild(child1Node);

	glm::mat4 child2Matrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 5.0f, 6.0f));
	Node* child2Node = new Node("child2", child2Matrix);
	rootNode->addChild(child2Node);

	// Create child nodes for the first child node
	glm::mat4 grandchild1Matrix = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 8.0f, 9.0f));
	Node* grandchild1Node = new Node("grandchild1", grandchild1Matrix);
	child1Node->addChild(grandchild1Node);

	glm::mat4 grandchild2Matrix = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 11.0f, 12.0f));
	Node* grandchild2Node = new Node("grandchild2", grandchild2Matrix);
	child1Node->addChild(grandchild2Node);

	// Test the final matrix of each node
	assert(rootNode->getFinalMatrix() == glm::mat4(1.0f));
	assert(child1Node->getFinalMatrix() == child1Matrix);
	assert(child2Node->getFinalMatrix() == child2Matrix);
	assert(grandchild1Node->getFinalMatrix() == grandchild1Matrix * child1Matrix);
	assert(grandchild2Node->getFinalMatrix() == grandchild2Matrix * child1Matrix);

	delete rootNode;

	return 0;
}

int test_pass_node()
{
	// Create a list
	auto myList = std::make_shared<List>();

	// Create a root node
	Mesh rootNode("Root", glm::mat4(1.0f));

	// Add some child nodes to the root node
	Mesh childNode1("Child1", glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	Mesh childNode2("Child2", glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	rootNode.addChild(&childNode1);
	rootNode.addChild(&childNode2);

	// Call the pass method on the root node
	rootNode.pass(myList, glm::mat4(1.0f));

	// Verify the results using assertions
	assert(myList->getList().size() == 3);  // Assuming that the root node and its two children are added to the list

	// Check the content of the list
	auto listContents = myList->getList();
	auto it = listContents.begin();

	assert(it->first == &rootNode);
	assert(it->second == glm::mat4(1.0f));

	++it;
	assert(it->first == &childNode1);
	assert(it->second == glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	++it;
	assert(it->first == &childNode2);
	assert(it->second == glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	return 0;
}

int test_camera() {
	// Derived class for testing
	class TestCamera : public Camera {
	public:
		TestCamera(const std::string& name, const glm::mat4& matrix, float near, float far)
			: Camera(name, matrix, near, far) {}

		// Implement the pure virtual function for testing
		void update(int width, int height) override {}
	};

	// Create a test camera
	std::string cameraName = "TestCamera";
	glm::mat4 projMatrix = glm::mat4(1.0f);
	TestCamera* camera = new TestCamera(cameraName, projMatrix, 0.1f, 100.0f);

	// Check the name and projection matrix
	assert(camera->name() == cameraName);
	assert(camera->projectionMatrix() == projMatrix);

	// Dummy node for the test
	Node* dummyNode = new Node("DummyNode", glm::mat4(1.0f));

	// Set the camera as a child of the dummy node
	dummyNode->addChild(camera);

	// Calculate the inverse matrix
	glm::mat4 inverseMatrix = camera->getCameraInverse();

	// Verify that the inverse matrix is correct
	assert(inverseMatrix == glm::inverse(dummyNode->getFinalMatrix()));

	return 0;
}

int test_light()
{
	class TestLight : public Light {
	public:
		using Light::Light; // Inherit constructor

		// Implement the pure virtual method
		void position(float x, float y, float z) override {
			// Store the position for testing purposes
			m_position = glm::vec4(x, y, z, 1.0f);  // Assuming w = 1.0 for simplicity
		}

		// Additional methods to access the stored position for testing
		glm::vec4 getPositionForTesting() const {
			return m_position;
		}

		glm::vec4 getAmbient() {
			return m_ambient;
		}

		glm::vec4 getDiffuse() {
			return m_diffuse;
		}

		glm::vec4 getSpecular() {
			return m_specular;
		}

		unsigned int getLightNumber() {
			return m_lightNumber;
		}

	private:
		// Member variable to store the position
		glm::vec4 m_position;
	};

	// Create a Light object
	TestLight light("TestLight", glm::mat4(1.0f));

	// Test default values
	assert(light.name() == "TestLight");
	assert(light.matrix() == glm::mat4(1.0f));
	assert(light.getAmbient() == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	assert(light.getDiffuse() == glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	assert(light.getSpecular() == glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	assert(light.getLightNumber() == 0);

	// Test setting values
	light.ambient(0.2f, 0.3f, 0.4f, 0.5f);
	light.diffuse(0.3f, 0.4f, 0.5f, 0.6f);
	light.specular(0.4f, 0.5f, 0.6f, 0.7f);
	light.lightNumber(1);

	// Test updated values
	assert(light.getAmbient() == glm::vec4(0.2f, 0.3f, 0.4f, 0.5f));
	assert(light.getDiffuse() == glm::vec4(0.3f, 0.4f, 0.5f, 0.6f));
	assert(light.getSpecular() == glm::vec4(0.4f, 0.5f, 0.6f, 0.7f));
	assert(light.getLightNumber() == 1);

	return 0;
}

int test_directionalLight() {
	DirectionalLight light("TestLight", glm::mat4(2.0f));

	// Test constructor
	assert(light.matrix() == glm::mat4(2.0f));

	return 0;
}

int test_omniLight() {
	OmniLight light("TestLight", glm::mat4(2.0f));

	// Test constructor
	assert(light.matrix() == glm::mat4(2.0f));

	return 0;
}

int test_spotLight() {
	SpotLight light("TestLight", glm::mat4(2.0f));

	// Test constructor
	assert(light.matrix() == glm::mat4(2.0f));

	return 0;
}

// add() method of list class is already tested in pass() test of the node class
int test_list() {
	// Mock Light class
	class LightMock : public Light {
	public:
		using Light::Light;

		void render(const glm::mat4&, void* = nullptr) override {
			if (!m_render)
				m_render = true;
			m_renderCnt++;
		}

		bool isRenderCalled() {
			return m_render;
		}

		unsigned int renderCnt() {
			return m_renderCnt;
		}

		void position(float x, float y, float z) override {};

	protected:
		bool m_render;
		unsigned int m_renderCnt = 0;
	};

	LightMock* obj1 = new LightMock("TestObject1", glm::mat4(1.0f));
	LightMock* obj2 = new LightMock("TestObject2", glm::mat4(1.0f));

	// Initialize list
	// Create a list
	auto myList = std::make_shared<List>();
	myList->add(obj1, glm::mat4(1.0f));
	myList->add(obj2, glm::mat4(1.0f));

	// TEST RENDER
	myList->render();
	assert(obj1->isRenderCalled());
	assert(obj1->renderCnt() == 1);
	assert(obj2->isRenderCalled());
	assert(obj2->renderCnt() == 1);
	myList->render();
	assert(obj1->isRenderCalled());
	assert(obj1->renderCnt() == 2);
	assert(obj2->isRenderCalled());
	assert(obj2->renderCnt() == 2);

	// TEST COPY CONSTRUCTOR
	auto other(myList);
	assert(other->getList() == myList->getList());

	// TEST COPY ASSIGNMENT CONSTRUCTOR
	other = std::make_shared<List>();
	other = myList;
	assert(other->getList() == myList->getList());

	// TEST RESET
	myList->resetList();
	assert(myList->getList().size() == 0);

	delete obj1;
	delete obj2;
	return 0;
}

int test_lod() {
	// Create an instance of the Lod class
	Lod lod;

	// Create some sample data
	VertexData vertex{};

	// Set sample data
	glm::vec3 samplePosVertex(1.0f, 2.0f, 3.0f);
	glm::vec3 sampleNormal(0.0f, 1.0f, 0.0f);
	glm::vec2 sampleCoordText(0.5f, 0.7f);

	// Test the posVertex, normal, and coordText methods
	vertex.posVertex(samplePosVertex);
	vertex.normal(sampleNormal);
	vertex.coordText(sampleCoordText);

	glm::uvec3 face(0, 1, 2);

	// Test the addVertexData and addFaces methods
	lod.addVertexData(vertex);
	lod.addFaces(face);

	// Test the faces and vertex methods
	std::vector<glm::uvec3> faces = lod.faces();
	std::vector<VertexData> vertices = lod.vertex();

	assert(faces.size() == 1);
	assert(faces[0] == face);

	assert(vertices.size() == 1);
	assert(vertices[0].coordText() == vertex.coordText());
	assert(vertices[0].normal() == vertex.normal());
	assert(vertices[0].posVertex() == vertex.posVertex());

	return 0;
}

int test_material() {
	// Create material 
	Material* material = new Material("TestMaterial");

	// TEST CONSTRUCTOR
	assert(material->name() == "TestMaterial");

	// TEST SET EMISSION
	material->emission(5.0f, 5.0f, 5.0f, 5.0f);
	for (int i = 0; i < 4; i++) {
		assert(material->emission()[i] == 5.0f);
	}

	// TEST CLONE
	Material* clone = material->clone();
	for (int i = 0; i < 4; i++) {
		assert(material->emission()[i] == 5.0f);
	}

	delete material;
	delete clone;

	return 0;
}

int test_mesh() {
	// Create a material for testing
	Material* testMaterial = new Material("testMaterial");

	// Create a Mesh object
	Mesh* mesh = new Mesh("TestMesh", glm::mat4(1.0f), testMaterial);

	// TEST START AND STOP BLINK METHODS
	mesh->startBlink();
	assert(mesh->isBlinking());
	mesh->stopBlink();
	assert(!mesh->isBlinking());

	// TEST IS CASTING SHADOW METHOD
	mesh->isCastingShadow(true);
	assert(mesh->isCastingShadow());
	mesh->isCastingShadow(false);
	assert(!mesh->isCastingShadow());

	delete mesh;
	delete testMaterial;

	return 0;
}

int test_engine() {
	Engine engine = Engine::instance();

	// TEST HEIGHT AND WIDTH METHODS
	engine.height(200);
	engine.width(200);
	assert(engine.height() == 200);
	assert(engine.width() == 200);

	// TEST INIT FLAG METHODS
	engine.initFlag(true);
	assert(engine.initFlag());
	engine.initFlag(false);
	assert(!engine.initFlag());

	// TEST IS RUNNING METHODS
	engine.isRunning(true);
	assert(engine.isRunning());
	engine.isRunning(false);
	assert(!engine.isRunning());

	// TEST FIND NODE
	Node* parent = new Node("Parent", glm::mat4(1.0f));
	Node* child = new Node("Child", glm::mat4(1.0f));
	Node* subChild = new Node("subChild", glm::mat4(1.0f));

	parent->addChild(child);
	child->addChild(subChild);

	Node* found = engine.findNode(parent, "subChild");
	assert(found == subChild);
	found = engine.findNode(parent, "Child");
	assert(found == child);

	// TEST LIST METHODS
	auto myList = std::make_shared<List>();
	engine.list(myList);
	assert(engine.list() == myList);
	engine.list(nullptr);
	assert(engine.list() == nullptr);

	delete parent;
	return 0;
}

int test_orthographicCamera() {
	OrthographicCamera camera("TestCamera", glm::mat4(2.0f), 1.0f, 1.0f);

	// Test constructor
	assert(camera.name() == "TestCamera");
	assert(camera.matrix() == glm::mat4(2.0f));

	return 0;
}

int test_perspectiveCamera() {
	PerspectiveCamera camera("TestCamera", glm::mat4(2.0f), 1.0f, 1.0f, 1.0f);

	// Test constructor
	assert(camera.name() == "TestCamera");
	assert(camera.matrix() == glm::mat4(2.0f));

	return 0;
}

int test_ovo() {
	Ovoreader ovoreader;

	// TEST VALID FILE
	Node* rootNode = ovoreader.load("../ovo/Test.ovo");  // Provide a valid OVO file path

	assert(rootNode != nullptr);  // Ensure that the root node is not null

	auto myList = std::make_shared<List>();
	rootNode->pass(myList);

	assert(rootNode->name() == "[root]");
	assert(myList->getList().size() == 3);

	delete rootNode;

	// TEST INVALID FILE
	rootNode = ovoreader.load("invalid_file.ovo");

	assert(rootNode == nullptr);

	return 0;
}

int test_texture() {
	Texture* texture = new Texture("TestTexture");

	// TEST CONSTRUCTOR
	assert(texture->name() == "TestTexture");

	delete texture;

	return 0;
}

int test_vertexdata() {
	VertexData vertex{};

	// Set values using setters
	vertex.posVertex(glm::vec3(1.0f, 2.0f, 3.0f));
	vertex.normal(glm::vec3(0.0f, 1.0f, 0.0f));
	vertex.coordText(glm::vec2(0.5f, 0.5f));

	// Retrieve values using getters
	glm::vec3 pos = vertex.posVertex();
	glm::vec3 norm = vertex.normal();
	glm::vec2 texCoord = vertex.coordText();

	// Use assert to check whether the values are set and retrieved correctly
	assert(pos == glm::vec3(1.0f, 2.0f, 3.0f));
	assert(norm == glm::vec3(0.0f, 1.0f, 0.0f));
	assert(texCoord == glm::vec2(0.5f, 0.5f));

	return 0;
}


int main() {

	// node_test.cpp
	test_id();
	std::cout << "test_id() passed successfully!" << std::endl;

	test_getters_and_setters();
	std::cout << "test_getters_and_setters() passed successfully!" << std::endl;

	test_addChild();
	std::cout << "test_addChild() passed successfully!" << std::endl;

	test_getFinalMatrix();
	std::cout << "test_getFinalMatrix() passed successfully!" << std::endl;

	test_camera();
	std::cout << "test_camera() passed successfully!" << std::endl;

	test_light();
	std::cout << "test_light() passed successfully!" << std::endl;

	test_directionalLight();
	std::cout << "test_directionalLight() passed successfully!" << std::endl;

	test_omniLight();
	std::cout << "test_omniLight() passed successfully!" << std::endl;

	test_spotLight();
	std::cout << "test_spotLight() passed successfully!" << std::endl;

	test_list();
	std::cout << "test_list() passed successfully!" << std::endl;

	test_lod();
	std::cout << "test_lod() passed successfully!" << std::endl;

	test_material();
	std::cout << "test_material() passed successfully!" << std::endl;

	test_mesh();
	std::cout << "test_mesh() passed successfully!" << std::endl;

	test_engine();
	std::cout << "test_engine() passed successfully!" << std::endl;

	test_orthographicCamera();
	std::cout << "test_orthographicCamera() passed successfully!" << std::endl;

	test_perspectiveCamera();
	std::cout << "test_perspectiveCamera() passed successfully!" << std::endl;

	test_ovo();
	std::cout << "test_ovo() passed successfully!" << std::endl;

	test_texture();
	std::cout << "test_texture() passed successfully!" << std::endl;

	test_vertexdata();
	std::cout << "test_vertexdata() passed successfully!" << std::endl;
	
	std::cout << "All tests passed successfully!" << std::endl;

	return 0;
}

*/