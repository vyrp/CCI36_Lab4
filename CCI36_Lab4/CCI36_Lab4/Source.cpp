/*
* CCI-36
* Exame: Shading
*
* Alunos:
*      Luiz Filipe Martins Ramos
*
* Data: 30/11/14
*/

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h> // For wglSwapInterval

#include <math.h>
#include <stdio.h>

#include <GL/glut.h>

#define sqr(x) ((x)*(x))

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

void initLights() {
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_COLOR_MATERIAL);
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

	// this is for the snow man scenario 
	cam.Update();
}

void initScene() {
	glEnable(GL_DEPTH_TEST);
	initLights();

	float xi = 0.0f, yi = 1.0f, zi = 5.0f;
	float lx0 = 0.0f, ly0 = 0.0f, lz0 = -1.0f;
	float upx0 = 0.0f, upy0 = 1.0f, upz0 = 0.0f;
	cam.Set(xi, yi, zi, xi + lx0, yi + ly0, zi + lz0, upx0, upy0, upz0);
	cam.Update();
}


float lpos[4] = { 1.0, 1.0, 1.0, 0.0 };
float a = 0;

void renderScene(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		cam.Update();

		glLightfv(GL_LIGHT0, GL_POSITION, lpos);
		glRotatef(a, 0, 1, 0);
		glColor3f(0.1,0.5,0.6);

		GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 500.0 };

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

		glutSolidTeapot(1);
		a += 0.05;

		frame++;
		tempo = glutGet(GLUT_ELAPSED_TIME);
		if (tempo - tempobase > 1000) {
			sprintf_s(s, "FPS:%4.2f", frame*1000.0 / (tempo - tempobase));
			glutSetWindowTitle(s);
			tempobase = tempo;
			frame = 0;
		}

		glutSwapBuffers();
		return;
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

int g_iWindowWidth = 800;
int g_iWindowHeight = 600;
int g_iWindowHandle = 0;


// Vertex array object for the cube
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

	glutKeyboardFunc(inputKey);
	glutSpecialFunc(specialInputKey);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
}

#define SHADE_TYPE 7

int main(int argc, char **argv)
{
	initGl(argc, argv);
	InitGLEW();

#if SHADE_TYPE == 1
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "toonShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "toonShader.frag");
#elif SHADE_TYPE == 2
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "diffuseLightShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "diffuseLightShader.frag");
#elif SHADE_TYPE == 3
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "ambient_diffuse_LightShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "ambient_diffuse_LightShader.frag");
#elif SHADE_TYPE == 4
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "ambient_diffuse_specular_Phong_LightShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "ambient_diffuse_specular_Phong_LightShader.frag");
#elif SHADE_TYPE == 5
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "ambient_diffuse_specular_BlinnPhong_LightShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "ambient_diffuse_specular_BlinnPhong_LightShader.frag");
#elif SHADE_TYPE == 6
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "PhongShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "PhongShader.frag");
#elif SHADE_TYPE == 7
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, "PhongPointShader.vert");
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "PhongPointShader.frag");
#endif

	std::vector<GLuint> shaders;
	shaders.push_back(vertexShader);
	shaders.push_back(fragmentShader);

	// Create the shader program.
	g_ShaderProgram = CreateShaderProgram(shaders);
	if (g_ShaderProgram == 0){
		std::cerr << "Erro! Impossivel de abrir programa" << std::endl;
		return 1;
	}

	glUseProgram(g_ShaderProgram);

	initScene();
	glutMainLoop();

	return 0;
}
