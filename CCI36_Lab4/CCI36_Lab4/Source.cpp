/*
* CCI-36
* Lab 04: Introdução a OpenGL
*
* Alunos:
*     Felipe Vincent Yannik Romero Pereira
*     Luiz Filipe Martins Ramos
*
* Data: 23/10/14
*/

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h> // For wglSwapInterval

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>



#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

#define sqr(x) ((x)*(x))
#define BUFFER_OFFSET(offset) ((void*)(offset))
#define MEMBER_OFFSET(s,m) ((char*)NULL + (offsetof(s,m)))


class CPoint3D {
public:
	float x, y, z;

	CPoint3D(){};

	CPoint3D(float x1, float y1, float z1)
	{
		x = x1; y = y1; z = z1;
	}

	void Set(float x1, float y1, float z1)
	{
		x = x1; y = y1; z = z1;
	}

	void Normalize()
	{
		float L = sqr(x) + sqr(y) + sqr(z);
		if (L>0.0f)
		{
			x /= L; y /= L; z /= L;
		}
	}

	CPoint3D& operator +=(CPoint3D p)
	{
		x += p.x; y += p.y; z += p.z; return *this;
	}
	CPoint3D& operator -=(CPoint3D p)
	{
		x -= p.x; y -= p.y; z -= p.z; return *this;
	}
	CPoint3D& operator *=(CPoint3D p)
	{
		x *= p.x; y *= p.y; z *= p.z; return *this;
	}
	CPoint3D& operator /=(CPoint3D p)
	{
		x /= p.x; y /= p.y; z /= p.z; return *this;
	}

	CPoint3D& operator %=(CPoint3D p)
	{
		double nx = y*p.z - z*p.y;
		double ny = -x*p.z + z*p.x;
		double nz = x*p.y - y*p.x;
		x = nx;
		y = ny;
		z = nz;
		return *this;
	}
};

CPoint3D operator +(CPoint3D p1, CPoint3D p2)
{
	return CPoint3D(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}
CPoint3D operator -(CPoint3D p1, CPoint3D p2)
{
	return CPoint3D(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}
CPoint3D operator *(CPoint3D p1, CPoint3D p2)
{
	return CPoint3D(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
}
CPoint3D operator /(CPoint3D p1, CPoint3D p2)
{
	return CPoint3D(p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);
}

CPoint3D operator *(CPoint3D p1, float f)
{
	return CPoint3D(p1.x * f, p1.y * f, p1.z * f);
}

CPoint3D operator %(CPoint3D p1, CPoint3D p2)
{
	return CPoint3D(p1.y*p2.z - p1.z*p2.y, -p1.x*p2.z + p1.z*p2.x, p1.x*p2.y - p1.y*p2.x);
}

class CCamera{
private:
	CPoint3D P0, At, Up;
	float speed;
	const double DeltaAngle = 0.06;

public:
	CCamera()
	{
		speed = 0.5;
		P0.Set(0.0f, 0.0f, 0.0f); At.Set(0.0f, 0.0f, 1.0f); Up.Set(0.0f, 1.0f, 0.0f);
	}
	
	void Set(float x0, float y0, float z0, float px0, float py0, float pz0, float upx, float upy, float upz)
	{
		P0.Set(x0, y0, z0); At.Set(px0, py0, pz0); Up.Set(upx, upy, upz);
	}

	void IncreaseSpeed() {
		speed *= 1.1;
	}

	void DecreaseSpeed() {
		speed /= 1.1;
	}

	void MoveFront() {
		CPoint3D L = (At - P0) * speed;
		P0 += L;
		At += L;
	}

	void MoveBack() {
		CPoint3D L = (At - P0) * speed;
		P0 -= L;
		At -= L;
	}

	void MoveRight() {
		CPoint3D L = (At - P0) * speed;
		L %= Up;
		P0 += L;
		At += L;
	}

	void MoveLeft() {
		CPoint3D L = (At - P0) * speed;
		L %= Up;
		P0 -= L;
		At -= L;
	}

	void MoveUp() {
		CPoint3D L = Up * speed;
		P0 += L;
		At += L;
	}

	void MoveDown() {
		CPoint3D L = Up * speed;
		P0 -= L;
		At -= L;
	}

	void LookUp() {
		CPoint3D oldL = At - P0;
		CPoint3D oldUp = Up;
		CPoint3D newL = oldL * cosf(DeltaAngle * speed) + oldUp * sinf(DeltaAngle * speed);
		Up = oldUp * cosf(DeltaAngle * speed) - oldL * sinf(DeltaAngle * speed);
		At = P0 + newL;
	}

	void LookDown() {
		CPoint3D oldL = At - P0;
		CPoint3D oldUp = Up;
		CPoint3D newL = oldL * cosf(DeltaAngle * speed) - oldUp * sinf(DeltaAngle * speed);
		Up = oldUp * cosf(DeltaAngle * speed) + oldL * sinf(DeltaAngle * speed);
		At = P0 + newL;
	}

	void LookRight() {
		CPoint3D oldL = At - P0;
		CPoint3D oldRight = oldL % Up;
		CPoint3D newL = oldL * cosf(DeltaAngle * speed) + oldRight * sinf(DeltaAngle * speed);
		At = P0 + newL;
	}

	void LookLeft() {
		CPoint3D oldL = At - P0;
		CPoint3D oldRight = oldL % Up;
		CPoint3D newL = oldL * cosf(DeltaAngle * speed) - oldRight * sinf(DeltaAngle * speed);
		At = P0 + newL;
	}

	void RollRight() {
		CPoint3D right = (At - P0) % Up;
		Up = Up * cosf(DeltaAngle * speed) + right * sinf(DeltaAngle * speed);
	}

	void RollLeft() {
		CPoint3D right = (At - P0) % Up;
		Up = Up * cosf(DeltaAngle * speed) - right * sinf(DeltaAngle * speed);
	}

	void Update()
	{
		glLoadIdentity();
		gluLookAt((GLdouble)P0.x, (GLdouble)P0.y, (GLdouble)P0.z,
			(GLdouble)At.x, (GLdouble)At.y, (GLdouble)At.z,
			(GLdouble)Up.x, (GLdouble)Up.y, (GLdouble)Up.z);
	}
};

int frame, tempo, tempobase = 0;
char s[30];
CCamera cam;
GLuint DLid;

GLuint createDL(void);

void initLights() {
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_COLOR_MATERIAL);
}

void executeLights() {
	GLfloat light_position[4] = { 2.0, 0.75, 0.0, 1.0 }; // x, y, z, w
	GLfloat light_direction[3] = { -1.0, 0, 0.0 }; // x, y, z
	GLfloat light_diffuse[4] = { 0.1, 0.1, 0.8, 1.0 }; // r, g, b, a
	GLfloat light_specular[4] = { 0.1, 0.1, 1.0, 1.0 }; // r, g, b, a

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 50.0f);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat light_ambient1[4] = { 0.2, 0.2, 0.2, 1.0 }; // r, g, b, a
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);

	GLfloat light_position2[4] = { 0.0, 4.0, 0.0, 1.0 }; // x, y, z, w
	GLfloat light_ambient2[4] = { 0.2, 0.2, 0.2, 1.0 }; // r, g, b, a
	GLfloat light_diffuse2[4] = { 0.7, 0.7, 0.7, 1.0 }; // r, g, b, a
	GLfloat light_specular2[4] = { 0.9, 0.9, 0.9, 1.0 }; // r, g, b, a
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);

	GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0) h = 1;

	GLdouble ratio = (1.0 * w) / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	//executeLights();

	// this is for the snow man scenario 
	cam.Update();
}

void initScene() {
	glEnable(GL_DEPTH_TEST);
	initLights();

	// Load or call scenario
	DLid = createDL();

	float xi = 0.0f, yi = 1.75f, zi = 5.0f;
	float lx0 = 0.0f, ly0 = 0.0f, lz0 = -1.0f;
	float upx0 = 0.0f, upy0 = 1.0f, upz0 = 0.0f;
	cam.Set(xi, yi, zi, xi + lx0, yi + ly0, zi + lz0, upx0, upy0, upz0);
	cam.Update();
}


void drawTeapot() {
	glColor3f(0.8f, 0.4f, 0.0f);

	//Draw teapot
	glTranslatef(0.0f, 0.8f, 0);
	glColor3f(0.1f, 0.6f, 0.6f);
	glutSolidTeapot(0.5);

	glTranslatef(0.0f, -0.3f, 0);
}

GLuint createDL() {
	GLuint sceneDL, loopDL;

	sceneDL = glGenLists(1);
	loopDL = glGenLists(1);

	glNewList(sceneDL, GL_COMPILE);



	drawTeapot();
	glEndList();

	glNewList(loopDL, GL_COMPILE);
	executeLights();
	for (int i = -3; i < 3; i++) {
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i * 10.0f, 0, j * 10.0f);
			glCallList(sceneDL);
			glPopMatrix();
		}
	}
	glEndList();

	return(loopDL);
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw ground

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	// Draw 36 Snow Men

	glCallList(DLid);
	frame++;
	tempo = glutGet(GLUT_ELAPSED_TIME);
	if (tempo - tempobase > 1000) {
		sprintf_s(s, "FPS:%4.2f", frame*1000.0 / (tempo - tempobase));
		glutSetWindowTitle(s);
		tempobase = tempo;
		frame = 0;
	}
	
	cam.Update();

	glutSwapBuffers();
}


void inputKey(unsigned char c, int x, int y) {
	switch (c) {
	case 'w':
		cam.LookUp();
		break;
	case 'a':
		cam.LookLeft();
		break;
	case 's':
		cam.LookDown();
		break;
	case 'd':
		cam.LookRight();
		break;
	case 'z':
		cam.RollLeft();
		break;
	case 'x':
		cam.RollRight();
		break;
	case 'p':
		cam.MoveUp();
		break;
	case 'l':
		cam.MoveDown();
		break;
	case '+':
		cam.IncreaseSpeed();
		break;
	case '-':
		cam.DecreaseSpeed();
		break;
	case 27:
		exit(0);
		break;
	default: break;
	}
}

void specialInputKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: 
		cam.MoveLeft();
		break;
	case GLUT_KEY_RIGHT: 
		cam.MoveRight();
		break;
	case GLUT_KEY_UP: 
		cam.MoveFront();
		break;
	case GLUT_KEY_DOWN: 
		cam.MoveBack();
		break;
	}
}

///////////////////////////////////////////////////////

void IdleGL();
void DisplayGL();
void KeyboardGL(unsigned char c, int x, int y);
void KeyboardUpGL(unsigned char c, int x, int y);
void SpecialGL(int key, int x, int y);
void SpecialUpGL(int key, int x, int y);
void MouseGL(int button, int state, int x, int y);
void MotionGL(int x, int y);
void ReshapeGL(int w, int h);


int g_iWindowWidth = 800;
int g_iWindowHeight = 600;
int g_iWindowHandle = 0;

int g_W, g_A, g_S, g_D, g_Q, g_E;
bool g_bShift = false;

glm::ivec2 g_MousePos;

glm::quat g_Rotation;

std::clock_t g_PreviousTicks;
std::clock_t g_CurrentTicks;

//Camera g_Camera;
glm::vec3 g_InitialCameraPosition;
glm::quat g_InitialCameraRotation;

struct VertexXYZColor
{
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
};

// Define the 8 vertices of a unit cube
VertexXYZColor g_Vertices[8] = {
		{ glm::vec3(1, 1, 1), glm::vec3(1, 1, 1) }, // 0
		{ glm::vec3(-1, 1, 1), glm::vec3(0, 1, 1) }, // 1
		{ glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1) }, // 2
		{ glm::vec3(1, -1, 1), glm::vec3(1, 0, 1) }, // 3
		{ glm::vec3(1, -1, -1), glm::vec3(1, 0, 0) }, // 4
		{ glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0) }, // 5
		{ glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0) }, // 6
		{ glm::vec3(1, 1, -1), glm::vec3(1, 1, 0) }, // 7
};

// Define the vertex indices for the cube.
// Each set of 6 vertices represents a set of triangles in 
// counter-clockwise winding order.
GLuint g_Indices[36] = {
	0, 1, 2, 2, 3, 0,           // Front face
	7, 4, 5, 5, 6, 7,           // Back face
	6, 5, 2, 2, 1, 6,           // Left face
	7, 0, 3, 3, 4, 7,           // Right face
	7, 6, 1, 1, 0, 7,           // Top face
	3, 2, 5, 5, 4, 3            // Bottom face
};


// Vertex array object for the cube.
GLuint g_vaoCube = 0;
GLuint g_ShaderProgram = 0;
// Model, View, Projection matrix uniform variable in shader program.
GLint g_uniformMVP = -1;



void InitGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "There was a problem initializing GLEW. Exiting..." << std::endl;
		exit(-1);
	}

	// Check for 3.3 support.
	// I've specified that a 3.3 forward-compatible context should be created.
	// so this parameter check should always pass if our context creation passed.
	// If we need access to deprecated features of OpenGL, we should check
	// the state of the GL_ARB_compatibility extension.
	if (!GLEW_VERSION_3_3)
	{
		std::cerr << "OpenGL 3.3 required version support not present." << std::endl;
		exit(-1);
	}

#ifdef _WIN32
	if (WGLEW_EXT_swap_control)
	{
		wglSwapIntervalEXT(0); // Disable vertical sync
	}
#endif
}

// Loads a shader and returns the compiled shader object.
// If the shader source file could not be opened or compiling the 
// shader fails, then this function returns 0.
GLuint LoadShader(GLenum shaderType, const std::string& shaderFile)
{
	std::ifstream ifs;

	// Load the shader source file.
	ifs.open(shaderFile);

	if (!ifs)
	{
		std::cerr << "Can not open shader file: \"" << shaderFile << "\"" << std::endl;
		return 0;
	}

	std::string source(std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()));
	ifs.close();

	// Create a shader object.
	GLuint shader = glCreateShader(shaderType);

	// Load the shader source for each shader object.
	const GLchar* sources[] = { source.c_str() };
	glShaderSource(shader, 1, sources, NULL);
	glCompileShader(shader);
	// Check for errors
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif
		delete infoLog;
		return 0;
	}
	return shader;
}


// Create a shader program from a set of compiled shader objects.
GLuint CreateShaderProgram(std::vector<GLuint> shaders)
{
	// Create a shader program.
	GLuint program = glCreateProgram();

	// Attach the appropriate shader objects.
	for (int i = 0; i < shaders.size(); i++)
	{
		GLuint shader = shaders[i];
		glAttachShader(program, shader);
	}

	// Link the program
	glLinkProgram(program);

	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(program, logLength, NULL, infoLog);

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif

		delete infoLog;
		return 0;
	}
	return program;
}


void initGl(int argc, char **argv)
{
	glutInit(&argc, argv);

	int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition((iScreenWidth - g_iWindowWidth) / 2, (iScreenHeight - g_iWindowHeight) / 2);
	//glutInitWindowPosition(200, 200);
	glutInitWindowSize(g_iWindowWidth, g_iWindowHeight);

	g_iWindowHandle = glutCreateWindow("Exame");


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	//glutKeyboardFunc(inputKey);
	//glutSpecialFunc(specialInputKey);
	//glutDisplayFunc(renderScene);
	//glutIdleFunc(renderScene);
	//glutReshapeFunc(changeSize);

	glutIdleFunc(IdleGL);
	glutDisplayFunc(DisplayGL);
	glutKeyboardFunc(KeyboardGL);
	glutKeyboardUpFunc(KeyboardUpGL);
	glutSpecialFunc(SpecialGL);
	glutSpecialUpFunc(SpecialUpGL);
	glutMouseFunc(MouseGL);
	glutMotionFunc(MotionGL);
	glutReshapeFunc(ReshapeGL);

}


//////////////////////////////////////////////////////









/**
* Basic camera class.
*/

class Camera
{
public:

	Camera();
	Camera(int screenWidth, int screenHeight);

	void SetViewport(int x, int y, int width, int height);
	glm::vec4 GetViewport() const;

	void SetProjectionRH(float fov, float aspectRatio, float zNear, float zFar);

	void ApplyViewMatrix();

	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition() const;

	// Translate the camera by some amount. If local is TRUE (default) then the translation should
	// be applied in the local-space of the camera. If local is FALSE, then the translation is 
	// applied in world-space.
	void Translate(const glm::vec3& delta, bool local = true);

	void SetRotation(const glm::quat& rot);
	glm::quat GetRotation() const;

	void SetEulerAngles(const glm::vec3& eulerAngles);
	glm::vec3 GetEulerAngles() const;

	// Rotate the camera by some amount.
	void Rotate(const glm::quat& rot);

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

protected:

	void UpdateViewMatrix();

	glm::vec4 m_Viewport;

	glm::vec3 m_Position;
	glm::quat m_Rotation;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

private:
	bool m_ViewDirty;
};

Camera::Camera()
	: m_Viewport(0)
	, m_Position(0)
	, m_Rotation()
	, m_ProjectionMatrix(1)
	, m_ViewMatrix(1)
	, m_ViewDirty(false)
{}

Camera::Camera(int screenWidth, int screenHeight)
	: m_Viewport(0, 0, screenWidth, screenHeight)
	, m_Position(0)
	, m_Rotation()
	, m_ProjectionMatrix(1)
	, m_ViewMatrix(1)
	, m_ViewDirty(false)
{

}

void Camera::SetViewport(int x, int y, int width, int height)
{
	m_Viewport = glm::vec4(x, y, width, height);
	glViewport(x, y, width, height);
}

glm::vec4 Camera::GetViewport() const
{
	return m_Viewport;
}

void Camera::SetProjectionRH(float fov, float aspectRatio, float zNear, float zFar)
{
	m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}

void Camera::ApplyViewMatrix()
{
	UpdateViewMatrix();
}

void Camera::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
	m_ViewDirty = true;
}

glm::vec3 Camera::GetPosition() const
{
	return m_Position;
}

void Camera::Translate(const glm::vec3& delta, bool local /* = true */)
{
	if (local)
	{
		m_Position += m_Rotation * delta;
	}
	else
	{
		m_Position += delta;
	}
	m_ViewDirty = true;
}

void Camera::SetRotation(const glm::quat& rot)
{
	m_Rotation = rot;
	m_ViewDirty = true;
}

glm::quat Camera::GetRotation() const
{
	return m_Rotation;
}

void Camera::SetEulerAngles(const glm::vec3& eulerAngles)
{
	m_Rotation = glm::quat(glm::radians(eulerAngles));
}

glm::vec3 Camera::GetEulerAngles() const
{
	return glm::degrees(glm::eulerAngles(m_Rotation));
}

void Camera::Rotate(const glm::quat& rot)
{
	m_Rotation = m_Rotation * rot;
	m_ViewDirty = true;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	UpdateViewMatrix();
	return m_ViewMatrix;
}

void Camera::UpdateViewMatrix()
{
	if (m_ViewDirty)
	{
		glm::mat4 translate = glm::translate(-m_Position);
		// Since we know the rotation matrix is orthonormalized, we can simply 
		// transpose the rotation matrix instead of inversing.
		glm::mat4 rotate = glm::transpose(glm::toMat4(m_Rotation));

		m_ViewMatrix = rotate * translate;

		m_ViewDirty = false;
	}
}

Camera g_Camera;


void ReshapeGL(int w, int h)
{
	if (h == 0)
	{
		h = 1;
	}

	g_iWindowWidth = w;
	g_iWindowHeight = h;

	g_Camera.SetViewport(0, 0, w, h);
	g_Camera.SetProjectionRH(60.0f, w / (float)h, 0.1f, 100.0f);

	glutPostRedisplay();
}

void DisplayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(g_vaoCube);
	glUseProgram(g_ShaderProgram);

	glm::mat4 mvp = g_Camera.GetProjectionMatrix() * g_Camera.GetViewMatrix() * glm::toMat4(g_Rotation);
	glUniformMatrix4fv(g_uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	glUseProgram(0);
	glBindVertexArray(0);

	glutSwapBuffers();
}

void IdleGL()
{
	g_CurrentTicks = std::clock();
	float deltaTicks = (float)(g_CurrentTicks - g_PreviousTicks);
	g_PreviousTicks = g_CurrentTicks;

	float fDeltaTime = deltaTicks / (float)CLOCKS_PER_SEC;

	float cameraSpeed = 1.0f;
	if (g_bShift)
	{
		cameraSpeed = 5.0f;
	}

	g_Camera.Translate(glm::vec3(g_D - g_A, g_Q - g_E, g_S - g_W) * cameraSpeed * fDeltaTime);

	glutPostRedisplay();
}

void KeyboardGL(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'w':
	case 'W':
		g_W = 1;
		break;
	case 'a':
	case 'A':
		g_A = 1;
		break;
	case 's':
	case 'S':
		g_S = 1;
		break;
	case 'd':
	case 'D':
		g_D = 1;
		break;
	case 'q':
	case 'Q':
		g_Q = 1;
		break;
	case 'e':
	case 'E':
		g_E = 1;
		break;
	case 'r':
	case 'R':
		g_Camera.SetPosition(g_InitialCameraPosition);
		g_Camera.SetRotation(g_InitialCameraRotation);
		g_Rotation = glm::quat();
		break;
	case 27:
		//glutLeaveMainLoop();
		break;
	}
}

void KeyboardUpGL(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'w':
	case 'W':
		g_W = 0;
		break;
	case 'a':
	case 'A':
		g_A = 0;
		break;
	case 's':
	case 'S':
		g_S = 0;
		break;
	case 'd':
	case 'D':
		g_D = 0;
		break;
	case 'q':
	case 'Q':
		g_Q = 0;
		break;
	case 'e':
	case 'E':
		g_E = 0;
		break;

	default:
		break;
	}
}

void SpecialGL(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_HOME:
	{
		g_bShift = true;
	}
		break;
	}
}

void SpecialUpGL(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_HOME:
	{
		g_bShift = false;
	}
		break;
	}
}

void MouseGL(int button, int state, int x, int y)
{
	g_MousePos = glm::ivec2(x, y);
}

void MotionGL(int x, int y)
{
	glm::ivec2 mousePos = glm::ivec2(x, y);
	glm::vec2 delta = glm::vec2(mousePos - g_MousePos);
	g_MousePos = mousePos;

	std::cout << "dX: " << delta.x << " dy: " << delta.y << std::endl;

	glm::quat rotX = glm::angleAxis<float>(glm::radians(delta.y) * 0.5f, glm::vec3(1, 0, 0));
	glm::quat rotY = glm::angleAxis<float>(glm::radians(delta.x) * 0.5f, glm::vec3(0, 1, 0));

	//g_Camera.Rotate( rotX * rotY );
	g_Rotation = (rotX * rotY) * g_Rotation;

}




int main(int argc, char **argv)
{
	initGl(argc, argv);
	InitGLEW();
	//initScene();
	


	glutMainLoop();
	return 0;

	// Load some shaders.
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "simpleShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "simpleShader.frag");

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	// Create the shader program.
	g_ShaderProgram = CreateShaderProgram(shaders);
	assert(g_ShaderProgram != 0);

	GLint positionAtribID = glGetAttribLocation(g_ShaderProgram, "in_position");
	GLint colorAtribID = glGetAttribLocation(g_ShaderProgram, "in_color");
	g_uniformMVP = glGetUniformLocation(g_ShaderProgram, "MVP");


	// Create a VAO for the cube.
	glGenVertexArrays(1, &g_vaoCube);
	glBindVertexArray(g_vaoCube);
	
	GLuint vertexBuffer, indexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertices), g_Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_Indices), g_Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(positionAtribID, 3, GL_FLOAT, false, sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Pos));
	glEnableVertexAttribArray(positionAtribID);

	glVertexAttribPointer(colorAtribID, 3, GL_FLOAT, false, sizeof(VertexXYZColor), MEMBER_OFFSET(VertexXYZColor, m_Color));
	glEnableVertexAttribArray(colorAtribID);

	// Make sure we disable and unbind everything to prevent rendering issues later.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(positionAtribID);
	glDisableVertexAttribArray(colorAtribID);


	glutMainLoop();
	// Register GLUT callbacks.
	/*glutIdleFunc(IdleGL);
	glutDisplayFunc(DisplayGL);
	glutKeyboardFunc(KeyboardGL);
	glutKeyboardUpFunc(KeyboardUpGL);
	glutSpecialFunc(SpecialGL);
	glutSpecialUpFunc(SpecialUpGL);
	glutMouseFunc(MouseGL);
	glutMotionFunc(MotionGL);
	glutReshapeFunc(ReshapeGL);*/



	return(0);
}
