
#define GLM_ENABLE_EXPERIMENTAL
#include "MyGlWindow.h"
#include <vector>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

static float DEFAULT_VIEW_POINT[3] = { 10, 10, 10 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

//fix it to use smart pointers
//std::unique_ptr<ShaderProgram> shaderProgram = nullptr;
std::unique_ptr<Program> program = nullptr;

MyGlWindow::MyGlWindow(int w, int h)
//==========================================================================
{

	m_width = w;
	m_height = h;

	m_plane = nullptr;
	m_torus = nullptr;
	m_teapot = nullptr;	

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float) h);
	//fix it to use smart pointers
	m_viewer = std::make_unique<Viewer>(viewPoint, viewCenter, upVector, 60.0, aspect);


	initialize();
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


void MyGlWindow::drawPlane(glm::mat4 &view, glm::mat4 &projection, glm::vec3& camPos)
{
	glm::mat4 model = glm::mat4(1.0);

	

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;


	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));


	glm::vec3 Kd(0.2, 0.2, 0.2);
	glm::vec3 Ka(0.1, 0.1, 0.1);
	float shiness = 180.0f;

	//these uniforms are fixed for the simple shader, so we set them here. You can change the shader to add more uniforms and set them here.
	program->BindProgram();
	

	program->SetVector("Ka", Ka);
	program->SetVector("Kd", Kd);
	program->SetFloat("shiness", shiness);

	
	program->SetMatrix("NormalMatrix", nmat);
	program->SetMatrix("MVP", mvp);
	program->SetMatrix("Model", model);
	program->SetVector("camPos", camPos);


	if (m_plane) m_plane->draw();


	program->UnbindProgram();

}

void MyGlWindow::drawTorus(glm::mat4& view, glm::mat4& projection, glm::vec3& camPos)
{

	glm::mat4 model = glm::mat4(1.0);
	
	model = glm::translate(model, glm::vec3(-3, 5, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	//we have to make it small because the light position is fixed and the bunny model is too big. You can change the light position in shader if you want to see the bunny bigger.

	//model = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;


	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	glm::vec3 Kd(0.5, 0.1, 0.1);
	glm::vec3 Ka(0.1, 0.1, 0);

	float shiness = 180.0f;

	program->BindProgram();

	program->SetVector("Ka", Ka);
	program->SetVector("Kd", Kd);
	program->SetFloat("shiness", shiness);

	program->SetMatrix("NormalMatrix", nmat);
	program->SetMatrix("MVP", mvp);
	program->SetMatrix("Model", model);
	program->SetVector("camPos", camPos);


	if (m_torus) m_torus->draw();


	program->UnbindProgram();
}

void MyGlWindow::drawTeapot(glm::mat4& view, glm::mat4& projection, glm::vec3 & camPos)
{

	glm::mat4 model = glm::mat4(1.0);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

	//we have to make it small because the light position is fixed and the bunny model is too big. You can change the light position in shader if you want to see the bunny bigger.

	//model = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;


	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));


	glm::vec3 Kd(0.5, 0.5, 0.1);
	glm::vec3 Ka(0.2, 0.2, 0);

	float shiness = 180.0f;

	program->BindProgram();

	program->SetVector("Ka", Ka);
	program->SetVector("Kd", Kd);
	program->SetFloat("shiness", shiness);

	program->SetMatrix("NormalMatrix", nmat);
	program->SetMatrix("MVP", mvp);
	program->SetMatrix("Model", model);
	program->SetVector("camPos", camPos);

	if (m_teapot) m_teapot->draw();

	program->UnbindProgram();
}

void MyGlWindow::draw(void)
{
		
	glm::vec3 eye = m_viewer->getViewPoint(); // m_viewer->getViewPoint().x(), m_viewer->getViewPoint().y(), m_viewer->getViewPoint().z());
	glm::vec3 look = m_viewer->getViewCenter();   //(m_viewer->getViewCenter().x(), m_viewer->getViewCenter().y(), m_viewer->getViewCenter().z());
	glm::vec3 up = m_viewer->getUpVector(); // m_viewer->getUpVector().x(), m_viewer->getUpVector().y(), m_viewer->getUpVector().z());

	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = perspective(45.0f, 1.0f * m_width / m_height, 0.1f, 500.0f);

	drawPlane(view, projection, eye);
	drawTorus(view, projection, eye);
	drawTeapot(view, projection, eye);
	
	
}

MyGlWindow::~MyGlWindow()
{

}


void MyGlWindow::initialize()
{
	m_plane = std::make_unique<VBOPlane>(30.0f, 30.0f, 1, 1);
	
	m_torus = std::make_unique<VBOTorus>(1.0f, 0.5f, 32, 32);
	m_teapot = std::make_unique<VBOTeapot>(64, glm::mat4(1.0f));

	
	try {
		program = std::unique_ptr<Program>(Program::GenerateFromFileVsFs("shaders/simple.vert", "shaders/simple.frag"));
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
	

	
}

