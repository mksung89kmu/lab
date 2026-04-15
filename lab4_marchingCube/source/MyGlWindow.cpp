
#define GLM_ENABLE_EXPERIMENTAL
#include "MyGlWindow.h"
#include <vector>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

#include <FastNoise.h>
#include <CubeGrid.h>
#include <CellGrid.h>
#include <Mesh.h>


#define GRID_WIDTH      50
#define GRID_HEIGHT     50
#define GRID_DEPTH      50

// noise function parameters

#define OCTAVES         5
#define LACUNARITY      2.f
#define PERSISTANCE     0.5f
#define NOISE_SCALE     5.f

// cube grid parameters

#define SURFACE_LEVEL   0.5f
#define CUBE_SIZE       0.1f
#define MIN_REGION_SIZE 1000

// camera parameters

#define CAM_ROTATION_SPEED  0.5f
#define CAM_ZOOM_SPEED      0.3f
#define CAM_MIN_DIST        1.f
#define CAM_MAX_DIST        15.f

static FastNoise noise;
static CellGrid cellGrid;
static CubeGrid cubeGrid;
static Mesh mesh;

static GLuint vbo, vbo2, ibo, vaoHandle;
static unsigned int vboSize, iboSize;
static unsigned int normalsOffset;
static unsigned int iboCount;


static float DEFAULT_VIEW_POINT[3] = { 5, 5, 5 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

//fix it to use smart pointers
//std::unique_ptr<ShaderProgram> shaderProgram = nullptr;
std::unique_ptr<Program> program = nullptr;

static void generateMap()
{
	cout << "Generating new box";

	auto startTime = chrono::high_resolution_clock::now();

	// delete previous mesh data
	mesh.clear();
	// configure noise to a random seed
	noise.SetSeed(rand());
	// fill the 3D scalar field
	cellGrid.fillGrid(noise, OCTAVES, LACUNARITY, PERSISTANCE, NOISE_SCALE);
	
	//cellGrid.print();
	// generate the cube grid according to that scalar field
	//get rid of small regions to avoid useless small terrain features, this step helps to reduce the final number of vertices

	cubeGrid.generateGrid(cellGrid, CUBE_SIZE, SURFACE_LEVEL, MIN_REGION_SIZE);

	//perform marching cubes algorithm to generate the vertices of the mesh from the cube grid
	cubeGrid.marchCubes(mesh.vertices);

	mesh.generateMesh(cubeGrid, MIN_REGION_SIZE);

	// free memory of useless data
	cubeGrid.clear();

	auto finishTime = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(finishTime - startTime);

	cout << ": seed " << noise.GetSeed();
	cout << " - " << mesh.vertices.size() << " vertices, ";
	cout << mesh.triangles.size() << " triangles";
	cout << " - " << duration.count() << "ms" << endl;
}


static void generateBuffers()
{
	// generate the VBO and load the vertices and normals of the mesh inside

	float* vertices = mesh.getVertexArray();

	//normal offset
	normalsOffset = mesh.vertices.size() * 3 * sizeof(float);
	vboSize = normalsOffset * 2;

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vboSize, &(vertices[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(
		0, //attribute index
		3,  //number of elements per vertex, here (x,y,z,w)         
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 6,             //  stride : distance from the previous to next data
		(void*)0              //  offset  
	);
	glEnableVertexAttribArray(0);  //enable attribute index 0 (position)

	
	glVertexAttribPointer(
		1, //attribute index
		3,  //number of elements per vertex, here (x,y,z,w)         
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 6,             //  stride : distance from the previous to next data
		(void*)(sizeof(float) * 3)              //  offset  
	);
	glEnableVertexAttribArray(1);  //enable attribute index 0 (position)


	delete[] vertices;

	// generate the IBO and load the triangle indices inside

	unsigned int* triangles = mesh.getTriangleArray();

	iboCount = mesh.triangles.size() * 3;
	iboSize = iboCount * sizeof(unsigned int);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, &(triangles[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] triangles;
}



MyGlWindow::MyGlWindow(int w, int h)
//==========================================================================
{

	m_width = w;
	m_height = h;
	
	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float) h);
	//fix it to use smart pointers
	m_viewer = std::make_unique<Viewer>(viewPoint, viewCenter, upVector, 60.0, aspect);


	initialize();


	noise.SetNoiseType(FastNoise::Simplex);
	// create grid of cells (3D scalar field)
	cellGrid = CellGrid(GRID_WIDTH, GRID_HEIGHT, GRID_DEPTH);

	generateMap();
	generateBuffers();

	int winWidth, winHeight;
	float sx, sy, sz;
	float ax, ay, az;

	// get box size for wire box, size is constant
	sx = mesh.dimX / 2.f;
	sy = mesh.dimY / 2.f;
	sz = mesh.dimZ / 2.f;

	ax = -sx * 1.1f;
	ay = -sy * 1.1f;
	az = -sz * 1.1f;


}

glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up,zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis,xaxis));

	glm::mat4 R;
	
	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f,0.0f,0.0f,1.0f);
	
	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);

	
	return R*T;
}


glm::mat4 perspective(float fov, float aspect, float n, float f)
{
// 	glm::mat4 P(0.0f);
// 
// 	const float tanHalfFOV = 1.0f / (aspect * tan(glm::radians(fov) / 2.0f));
// 	const float tanHalfFOV2 = 1.0f / (tan(glm::radians(fov) / 2.0f));
// 	float A = (-n - f) / (n - f);
// 	float B =  (2 * (n * f)) / (n - f);
// 
// 	P[0] = glm::vec4(tanHalfFOV , 0, 0, 0);
// 	P[1] = glm::vec4(0, tanHalfFOV2, 0, 0);
// 	P[2] = glm::vec4(0, 0, A, 1.0f);
// 	P[3] = glm::vec4(0, 0, B, 0.0f);
// 
//  	return P;

// 
	glm::mat4 P(0.0f);

	const float tanHalfFOV = tan(glm::radians(fov) / 2.0f);

	float A = (-f - n) / (f - n);
	float B = (2 * (n * f)) / ( n - f);

	P[0] = glm::vec4(1.0f / (aspect * tanHalfFOV), 0, 0, 0);
	P[1] = glm::vec4(0, 1.0 / tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);

	return P;
}



void MyGlWindow::draw(void)
{
		
	glm::vec3 eye = m_viewer->getViewPoint(); // m_viewer->getViewPoint().x(), m_viewer->getViewPoint().y(), m_viewer->getViewPoint().z());
	glm::vec3 look = m_viewer->getViewCenter();   //(m_viewer->getViewCenter().x(), m_viewer->getViewCenter().y(), m_viewer->getViewCenter().z());
	glm::vec3 up = m_viewer->getUpVector(); // m_viewer->getUpVector().x(), m_viewer->getUpVector().y(), m_viewer->getUpVector().z());


	glm::mat4 model(1.0);
	glm::mat4 view = lookAt(eye, look, up);
	

	glm::mat4 projection = glm::perspective(45.0f, 1.0f*m_width / m_height, 0.1f, 500.0f);

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view *  model;
	
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mview)));

	/*
	shaderProgram->use();

						
		glUniformMatrix4fv(shaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		if (m_cube) m_cube->draw();
			

	shaderProgram->disable();		
	*/

	program->BindProgram();
		program->SetMatrix("model", model);
		program->SetMatrix("view", view);
		program->SetMatrix("projection", projection);

		program->SetMatrix("normalMatrix", normalMatrix);

		glBindVertexArray(vaoHandle);
			int size;
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			float tt = size / sizeof(unsigned int);
			glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	//	if (m_floor) m_floor->draw();
	program->UnbindProgram();
	
}

MyGlWindow::~MyGlWindow()
{

}


void MyGlWindow::initialize()
{
	//m_floor = std::make_unique<checkeredFloor>();

	std::string vert = std::string(DATA_DIR) + "/simple.vert";
	std::string frag = std::string(DATA_DIR) + "/simple.frag";

	try {
		program = std::unique_ptr<Program>(Program::GenerateFromFileVsFs(vert, frag));
	}
	catch (const std::runtime_error& e) {
		std::cerr << "SHADER ERROR: " << e.what() << std::endl;			
		std::cerr << "Failed to load shaders. Please check your shader files." << std::endl;
		exit(1);		
	}
	catch (const std::exception& e) {
		std::cerr << "UNEXPECTED ERROR: " << e.what() << std::endl;
		exit(1);
	}

	/*
	shaderProgram = std::make_unique<ShaderProgram>();
	//load shaders
	shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");
	//declaration of uniform var.
	shaderProgram->addUniform("model");  //add a uniform var.
	shaderProgram->addUniform("view");  //add a uniform var.
	shaderProgram->addUniform("projection");
	*/
}

