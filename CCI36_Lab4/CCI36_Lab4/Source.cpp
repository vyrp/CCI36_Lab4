// OpenGlDisplayList.cpp : Defines the entry point for the console application.
//

#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

#define sqr(x) ((x)*(x))
#define ROTX 1
#define ROTY 2
#define ROTZ 3
#define DIRX 4
#define DIRY 5
#define DIRZ 6
#define FRONT 7
#define BACK 8

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

	CPoint3D operator +(CPoint3D p)
	{
		x += p.x; y += p.y; z += p.z; return *this;
	}
	CPoint3D operator -(CPoint3D p)
	{
		x -= p.x; y -= p.y; z -= p.z; return *this;
	}
	CPoint3D operator *(CPoint3D p)
	{
		x *= p.x; y *= p.y; z *= p.z; return *this;
	}
	CPoint3D operator /(CPoint3D p)
	{
		x /= p.x; y /= p.y; z /= p.z; return *this;
	}

};

class CCamera{
public:
	CPoint3D P0, At, Up;
	float AngX, AngY, AngZ;

	CCamera()
	{
		P0.Set(0.0f, 0.0f, 0.0f); At.Set(0.0f, 0.0f, 1.0f); Up.Set(0.0f, 1.0f, 0.0f); AngX = 0.0f; AngY = 0.0f;  AngZ = 0.0f;
	}
	CCamera(CPoint3D p0, CPoint3D p, CPoint3D up)
	{
		P0 = p0; At = p; Up = up; AngX = 0.0f; AngY = 0.0f;  AngZ = 0.0f;
	}

	void Set(CPoint3D p0, CPoint3D p, CPoint3D up)
	{
		P0 = p0; At = p; Up = up; AngX = 0.0f; AngY = 0.0f;  AngZ = 0.0f;
	}
	void Set(float x0, float y0, float z0, float px0, float py0, float pz0, float upx, float upy, float upz)
	{
		P0.Set(x0, y0, z0); At.Set(px0, py0, pz0); Up.Set(upx, upy, upz); AngX = 0.0f; AngY = 0.0f;  AngZ = 0.0f;
	}
	void SetUp(float x1, float y1, float z1)
	{
		Up.x = x1; Up.y = y1; Up.z = z1;
	}
	void SetUp(CPoint3D up)
	{
		Up = up;
	}
	void SetPosition(float x, float y, float z)
	{
		P0.Set(x, y, z);
	}
	void SetPosition(CPoint3D p)
	{
		P0 = p;
	}
	void SetDirection(CPoint3D D)
	{
		At = D;
	}
	void SetDirection(float x, float y, float z)
	{
		At.Set(x, y, z);
	}
	void SetRotateX(float ang)
	{
		AngX = ang;
	}
	void SetRotateY(float ang)
	{
		AngY = ang;
	}
	void SetRotateZ(float ang)
	{
		AngZ = ang;
	}
	CPoint3D GetPosition()
	{
		return P0;
	}
	CPoint3D GetDirection()
	{
		return At;
	}

	void RotateX() // use to perform/update X rotation
	{
		glRotatef(AngX, 1.0f, 0.0f, 0.0f);
	}

	void RotateY()
	{
		glRotatef(AngY, 0.0f, 1.0f, 0.0f);
	}
	void RotateZ()
	{
		glRotatef(AngZ, 0.0f, 0.0f, 1.0f);
	}

	void RotateX(float ang)  // Equivalent to Rotate in X from angle 0 to ang
	{
		AngX = ang;
		glRotatef(AngX, 1.0f, 0.0f, 0.0f);
	}
	void RotateY(float ang)
	{
		// TODO
	}
	void RotateZ(float ang)
	{
		// TODO
	}

	void RotateRelX(float ang) // Incremental, acummulated rotation in X
	{
		AngX += ang;
		glRotatef(AngX, 1.0f, 0.0f, 0.0f);
	}
	void RotateRelY(float ang)
	{
		// TODO
	}
	void RotateRelZ(float ang)
	{
		// TODO
	}
	void LookAt()  // call gluLookAt
	{
		gluLookAt((GLdouble)P0.x, (GLdouble)P0.y, (GLdouble)P0.z,
			(GLdouble)At.x, (GLdouble)At.y, (GLdouble)At.z,
			(GLdouble)Up.x, (GLdouble)Up.y, (GLdouble)Up.z);
	}
	void Update()
	{
		glLoadIdentity();
		// TODO // Rotation in x, y and z
	}
};

float angle = 0.0;
float x = 0.0f, y = 1.75f, z = 5.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;

float ratio = 1.0;
int frame, time, timebase = 0;
char s[30];
CCamera cam;
GLuint DLid;

GLuint createDL(void);

void changeSize(int w, int h)	{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0) h = 1;

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	// TODO // Set camera initial position 
	// this is for the snow man scenario 
	cam.Set(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
	cam.Update();
}

void initScene() {
	glEnable(GL_DEPTH_TEST);
	// Load or call scenario
	DLid = createDL();
}


void drawSnowMan() {
	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body	
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
}



GLuint createDL() {
	GLuint snowManDL, loopDL;

	snowManDL = glGenLists(1);
	loopDL = glGenLists(1);

	glNewList(snowManDL, GL_COMPILE);
	drawSnowMan();
	glEndList();

	glNewList(loopDL, GL_COMPILE);
	for (int i = -3; i < 3; i++) {
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0f, 0, j * 10.0f);
			glCallList(snowManDL);
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

	// Draw 36 SnowMen

	glCallList(DLid);
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(s, "FPS:%4.2f", frame*1000.0 / (time - timebase));
		glutSetWindowTitle(s);
		timebase = time;
		frame = 0;
	}
	glutSwapBuffers();
}

//TODO // RotateCamera can be a method of CCamera
void RotateCamera(int rotMode, float ang) {
	switch (rotMode) {
		case ROTX: cam.SetRotateX(ang); break;
		case ROTY: //TO do;break;
		case ROTZ: cam.SetRotateZ(ang); break;
	}
	cam.Update();
}

// TODO // MoveCamera can be method also can deal with speed
void MoveCamera(int diretion /* TODO: ,... */) {
	switch (diretion){
		// do some transformation if needed 
		case DIRX: x = x + 0 /* TODO + ... */; break;
		case DIRY: y = y + 0 /* TODO + ... */; break;
		case DIRZ: z = z + 0 /* TODO + ... */; break;
		case FRONT: // move forward following camera direction
		case BACK: // move backward following camera direction
			break;
	}
	// TODO
	// call camera SetPosition;
	// call camera Update;
}

float da = 0.5f;

void inputKey(unsigned char c, int x, int y) {
	switch (c) {
		// TODO all your key input
		default: break;
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
		//  button can be GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
		// state can be r GLUT_UP or GLUT_DOWN (pressed)
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				// TODO change parameter you want
				// call glutPostRedisplay if display must be update for new parameters
				glutPostRedisplay();
			}
			break;
		default:
			break;
	}
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Lab4");

	initScene();

	glutKeyboardFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutReshapeFunc(changeSize);
	glutMouseFunc(mouse);
	glutMainLoop();

	return(0);
}

