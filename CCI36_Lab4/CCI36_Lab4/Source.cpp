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

int frame, time, timebase = 0;
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
	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
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

void drawSnowMan() {
	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body	
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 40, 40);

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
	executeLights();
	for (int i = -3; i < 3; i++) {
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i * 10.0f, 0, j * 10.0f);
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
	//executeLights();

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
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(s, "FPS:%4.2f", frame*1000.0 / (time - timebase));
		glutSetWindowTitle(s);
		timebase = time;
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab4");

	initScene();

	glutKeyboardFunc(inputKey);
	glutSpecialFunc(specialInputKey);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}
