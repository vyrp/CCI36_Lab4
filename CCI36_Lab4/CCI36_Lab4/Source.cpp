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

class CCamera{
private:
	CPoint3D P0, At, Up;
	float AngX, AngY, AngZ;
	float speed;

public:
	CCamera()
	{
		speed = 0.5;
		P0.Set(0.0f, 0.0f, 0.0f); At.Set(0.0f, 0.0f, 1.0f); Up.Set(0.0f, 1.0f, 0.0f); AngX = 0.0f; AngY = 0.0f;  AngZ = 0.0f;
	}
	
	void Set(float x0, float y0, float z0, float px0, float py0, float pz0, float upx, float upy, float upz)
	{
		P0.Set(x0, y0, z0); At.Set(px0, py0, pz0); Up.Set(upx, upy, upz); AngX = 0.0f; AngY = 0.0f;  AngZ = 0.0f;
	}
	void SetUp(float x1, float y1, float z1)
	{
		Up.x = x1; Up.y = y1; Up.z = z1;
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
		AngY = ang;
		glRotatef(AngY, 0.0f, 1.0f, 0.0f);
	}
	void RotateZ(float ang)
	{
		AngZ = ang;
		glRotatef(AngZ, 0.0f, 0.0f, 1.0f);
	}

	void RotateRelX(float ang) // Incremental, acummulated rotation in X
	{
		AngX += ang;
		glRotatef(AngX, 1.0f, 0.0f, 0.0f);
	}
	void RotateRelY(float ang)
	{
		AngY += ang;
		glRotatef(AngY, 0.0, 1.0f, 0.0f);
	}
	void RotateRelZ(float ang)
	{
		AngY += ang;
		glRotatef(AngZ, 0.0, 0.0f, 1.0f);
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

	}

	void MoveLeft() {

	}

	void MoveUp() {

	}

	void MoveDown() {

	}

	void LookUp() {

	}

	void LookDown() {

	}

	void LookRight() {

	}

	void LookLeft() {

	}

	void RollLeft() {

	}

	void RollRight() {

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

void changeSize(int w, int h)	{
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
	/*
	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}
	*/
	//cam.SetPosition(x, y, z);

	cam.Update();

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
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(s, "FPS:%4.2f", frame*1000.0 / (time - timebase));
		glutSetWindowTitle(s);
		timebase = time;
		frame = 0;
	}

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
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
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
