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
		if (L > 0.0f)
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


GLint n = 4;
GLint fill = 1;
GLint TOL = 10;
#define sqr(x) ((x)*(x))
GLdouble tolerance = sqr(TOL);
typedef struct { GLfloat x, y, z; } point3D_type;

GLint viewport[4];
GLdouble mvmatrix[16], projmatrix[16];


float vAng = 40.0f, asp = 1.0f, nearD = 0.2f, farD = 40.0f;

//GLfloat ctrlpoints[4][4][3] =
point3D_type ctrlpoints[4][4] =

{
	{
		{ -1.5, -1.5, 4.0 },
		{ -0.5, -1.5, 2.0 },
		{ 0.5, -1.5, -1.0 },
		{ 1.5, -1.5, 2.0 } },
		{
			{ -1.5, -0.5, 1.0 },
			{ -0.5, -0.5, 3.0 },
			{ 0.5, -0.5, 0.0 },
			{ 1.5, -0.5, -1.0 } },
			{
				{ -1.5, 0.5, 4.0 },
				{ -0.5, 0.5, 0.0 },
				{ 0.5, 0.5, 3.0 },
				{ 1.5, 0.5, 4.0 } },
				{
					{ -1.5, 1.5, -2.0 },
					{ -0.5, 1.5, -2.0 },
					{ 0.5, 1.5, 0.0 },
					{ 1.5, 1.5, -1.0 } }
};





//atenção: n é o grau do polinomio.
void Casteljau(GLfloat t, point3D_type a[], point3D_type b[], point3D_type c[], int n)
{
	int i, j; GLfloat  t_1 = 1 - t;
	for (i = 0; i <= n; i++)
		c[i] = a[i];
	b[0] = a[0];
	for (j = 1; j <= n; j++)
	{
		for (i = 0; i <= n - j; i++)
		{
			c[i].x = t_1*c[i].x + t*c[i + 1].x;
			c[i].y = t_1*c[i].y + t*c[i + 1].y;
			c[i].z = t_1*c[i].z + t*c[i + 1].z;
		}
		b[j] = c[0];
	}

}

void SaveBezier(char *file_name, point3D_type a[], GLint n, GLint m)
{
	FILE *file;
	fopen_s(&file, file_name, "w");
	fprintf(file, "%d %d\n", n, m);
	for (int j = 0; j < m; j++)
	{
		for (int i = 0; i < n; i++)
			fprintf(file, "%f %f %f , ", a[j*n + i].x, a[j*n + i].y, a[j*n + i].z);
		fprintf(file, "\n");
	}
	fclose(file);
}
void LoadBezier(char *file_name, point3D_type a[], GLint n, GLint m)
{
	FILE *file;
	fopen_s(&file, file_name, "r");

	fscanf_s(file, "%d %d\n", &n, &m);
	for (int j = 0; j < m; j++)
	{
		for (int i = 0; i < n; i++)
			fscanf_s(file, "%f %f %f , ", &a[j*n + i].x, &a[j*n + i].y, &a[j*n + i].z);
	}
	fclose(file);
}
void BezierSubdivision(GLfloat u, GLfloat v, point3D_type a[],
	point3D_type b[], point3D_type c[], point3D_type d[], point3D_type e[],
	GLint n, GLint m)
{
	point3D_type *col_a = new point3D_type[m];
	point3D_type *col_b = new point3D_type[m];
	point3D_type *col_c = new point3D_type[m];
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
			col_a[j] = a[j*n + i];
		Casteljau(v, col_a, col_b, col_c, m - 1);
		for (j = 0; j < m; j++)
		{
			b[j*n + i] = col_b[j];
			c[j*n + i] = col_c[j];
		}
	}

	for (j = 0; j < m; j++)
	{
		Casteljau(u, &b[j*n], &b[j*n], &d[j*n], n - 1);
		Casteljau(u, &c[j*n], &c[j*n], &e[j*n], n - 1);
	}


}

void DrawBezier(point3D_type a[], GLint n, GLint m, GLint nn, GLint mm)
{
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, n, 0, 1, n * 3, m, &(GLfloat &)a[0]);
	glMapGrid2f(n, 0.0, 1.0, m, 0.0, 1.0);
	if (fill == 1)
		glEvalMesh2(GL_FILL, 0, nn, 0, mm);
	else glEvalMesh2(GL_LINE, 0, nn, 0, mm);
}

bool Limit(point3D_type a[], GLint n, GLint m)
{
	int i, j; GLdouble x1, y1, z1, x2, y2, z2, max = 0.0, d;
	// analisar os pares (j,i) (j,i+1)
	for (j = 0; j < m; j++)
		for (i = 0; i < n - 1; i++)
		{  // projetar os pontos de controle (j,i) e (j,i+1)
			gluProject((GLdouble)a[j*n + i].x, (GLdouble)a[j*n + i].y, (GLdouble)a[j*n + i].z,
				mvmatrix, projmatrix, viewport, &x1, &y1, &z1);
			gluProject((GLdouble)a[j*n + i + 1].x, (GLdouble)a[j*n + i + 1].y, (GLdouble)a[j*n + i + 1].z,
				mvmatrix, projmatrix, viewport, &x2, &y2, &z2);
			d = sqr(x2 - x2) + sqr(y2 - y1);
			if (d > max)
				max = d;
		}

	//TO DO fazer o mesmo na outra direção(j, i) (j + 1, i)

	return (max <= tolerance);


}

void BezierRecursiveSubdivision(point3D_type a[], GLint n, GLint m)
{

	if (Limit(a, n, m))
		DrawBezier(a, n, m, n, m);
	else {
		point3D_type *b = new point3D_type[m*n];
		point3D_type *c = new point3D_type[m*n];
		point3D_type *d = new point3D_type[m*n];
		point3D_type *e = new point3D_type[m*n];
		BezierSubdivision(0.5f, 0.5f, a, b, c, d, e, n, m);

		if (Limit(b, n, m))
			DrawBezier(b, n, m, n, m);
		else BezierRecursiveSubdivision(b, n, m);
		//TO DO chamar recursivamente para os ptos de controle c, d, e

	}
}

void initlights(void)
{
	GLfloat ambient[] =
	{ 0.1, 0.7, 0.7, 1.0 };
	GLfloat position[] =
	{ 0.0, 0.0, 3.0, 2.0 };
	GLfloat mat_diffuse[] =
	{ 0.1, 0.9, 0.9, 1.0 };
	GLfloat mat_specular[] =
	{ 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] =
	{ 50.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}


GLfloat texpts[2][2][2] = { { { 0.0, 0.0 }, { 0.0, 1.0 } },
{ { 1.0, 0.0 }, { 1.0, 1.0 } } };




void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMapGrid2f(n, 0.0, 1.0, n, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glRotatef(85.0, 1.0, 1.0, 1.0);

	if (true)//perspectiva)
	{
		tolerance = sqr(TOL * 2);
		gluPerspective(vAng, asp, nearD, farD);
		//   cam.Set(eye1,look,up);

		cam.Update();
	}



	BezierRecursiveSubdivision((point3D_type *)ctrlpoints, 4, 4);
	glPopMatrix();

	glFlush();
	glutSwapBuffers(); // display the screen just made 



}


void myinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &(GLfloat &)ctrlpoints);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);



	glMapGrid2f(n, 0.0, 1.0, n, 0.0, 1.0);
	initlights();       /* for lighted version only */
}

void myReshape(int w, int h)
{
	GLfloat f = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (true)//perspectiva)
	{
		gluPerspective(vAng, asp, nearD, farD);
		//	cam.Set((CPoint3D)eye1, (CPoint3D)look, (CPoint3D)up);
		cam.Update();
	}
	else {
		if (w <= h)
			glOrtho(-4.0, 4.0, -4.0 * (GLfloat)h / (GLfloat)w,
			4.0 * (GLfloat)h / (GLfloat)w, -4.0, 4.0);
		else
			glOrtho(-4.0 * (GLfloat)w / (GLfloat)h,
			4.0 * (GLfloat)w / (GLfloat)h, -4.0, 4.0, -4.0, 4.0);
	}


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//cam.setAspect(((float)w)/h);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
}







void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {       /*  increase n */
			//   Put code here

			glutPostRedisplay();
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {      /*  change blue  */
			//   Put code here

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	//glutKeyboardFunc(myKeyboard);

	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}

