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
CCamera cam;

GLint n = 4;
bool fill = false;
GLint TOL = 10;
GLdouble tolerance = sqr(TOL);
typedef struct { GLfloat x, y, z; } point3D_type;

GLint viewport[4];
GLdouble mvmatrix[16], projmatrix[16];
bool perspectiva = false;
GLdouble x_angle = 0.0, y_angle = 0.0, z_angle = 0.0;

float vAng = 40.0f, asp = 1.0f, nearD = 0.2f, farD = 40.0f;

GLfloat ctrlpoints[4][4][3] =
//point3D_type ctrlpoints[4][4] =
{
	{
		{ -1.5, -1.5, 4.0 },
		{ -0.5, -1.5, 2.0 },
		{ 0.5, -1.5, -1.0 },
		{ 1.5, -1.5, 2.0 }
	}, {
		{ -1.5, -0.5, 1.0 },
		{ -0.5, -0.5, 3.0 },
		{ 0.5, -0.5, 0.0 },
		{ 1.5, -0.5, -1.0 }
	}, {
		{ -1.5, 0.5, 4.0 },
		{ -0.5, 0.5, 0.0 },
		{ 0.5, 0.5, 3.0 },
		{ 1.5, 0.5, 4.0 }
	}, {
		{ -1.5, 1.5, -2.0 },
		{ -0.5, 1.5, -2.0 },
		{ 0.5, 1.5, 0.0 },
		{ 1.5, 1.5, -1.0 }
	}
};

//atenção: n é o grau do polinomio.
void Casteljau(GLfloat t, point3D_type a[], point3D_type b[], point3D_type c[], int n)
{
	int i, j; GLfloat  t_1 = 1 - t;
	for (i = 0; i <= n; i++) {
		c[i] = a[i];
	}
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

void BezierSubdivision(GLfloat u, GLfloat v, point3D_type a[], point3D_type b[], point3D_type c[], point3D_type d[], point3D_type e[], GLint n, GLint m)
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
	delete[] col_a;
	delete[] col_b;
	delete[] col_c;
}

void DrawBezier(point3D_type a[], GLint n, GLint m, GLint nn, GLint mm)
{
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, n, 0, 1, n * 3, m, &(GLfloat &)a[0]);
	glMapGrid2f(n, 0.0, 1.0, m, 0.0, 1.0);
	if (fill)
		glEvalMesh2(GL_FILL, 0, nn, 0, mm);
	else
		glEvalMesh2(GL_LINE, 0, nn, 0, mm);
}

bool Limit(point3D_type a[], GLint n, GLint m)
{
	int i, j;
	GLdouble x1, y1, z1, x2, y2, z2, max = 0.0, d;
	
	// analisar os pares (j,i) (j,i+1)
	for (j = 0; j < m; j++)
	{
		for (i = 0; i < n - 1; i++)
		{
			gluProject((GLdouble)a[j*n + i].x, (GLdouble)a[j*n + i].y, (GLdouble)a[j*n + i].z,
				mvmatrix, projmatrix, viewport, &x1, &y1, &z1);
			gluProject((GLdouble)a[j*n + i + 1].x, (GLdouble)a[j*n + i + 1].y, (GLdouble)a[j*n + i + 1].z,
				mvmatrix, projmatrix, viewport, &x2, &y2, &z2);
			d = sqr(x2 - x2) + sqr(y2 - y1);
			if (d > max)
				max = d;
		}
	}

	//TO DO fazer o mesmo na outra direção(j, i) (j + 1, i)
	for (j = 0; j < m-1; j++)
	{
		for (i = 0; i < n; i++)
		{
			gluProject((GLdouble)a[j*n + i].x, (GLdouble)a[j*n + i].y, (GLdouble)a[j*n + i].z,
				mvmatrix, projmatrix, viewport, &x1, &y1, &z1);
			gluProject((GLdouble)a[(j + 1)*n + i].x, (GLdouble)a[(j + 1)*n + i].y, (GLdouble)a[(j + 1)*n + i + 1].z,
				mvmatrix, projmatrix, viewport, &x2, &y2, &z2);
			d = sqr(x2 - x2) + sqr(y2 - y1);
			if (d > max)
				max = d;
		}
	}

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
		else
			BezierRecursiveSubdivision(b, n, m);
		
		if (Limit(c, n, m))
			DrawBezier(c, n, m, n, m);
		else
			BezierRecursiveSubdivision(c, n, m);

		if (Limit(d, n, m))
			DrawBezier(d, n, m, n, m);
		else
			BezierRecursiveSubdivision(d, n, m);

		if (Limit(e, n, m))
			DrawBezier(e, n, m, n, m);
		else
			BezierRecursiveSubdivision(e, n, m);

		delete[] b;
		delete[] c;
		delete[] d;
		delete[] e;
	}
}

void initlights(void)
{
	GLfloat ambient[] = { 0.1, 0.7, 0.7, 1.0 };
	GLfloat position[] = { 0.0, 0.0, 3.0, 2.0 };
	GLfloat mat_diffuse[] = { 0.1, 0.9, 0.9, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}


GLfloat texpts[2][2][2] = {
	{ { 0.0, 0.0 }, { 0.0, 1.0 } },
	{ { 1.0, 0.0 }, { 1.0, 1.0 } }
};

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMapGrid2f(n, 0.0, 1.0, n, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glRotated(85.0, 1.0, 1.0, 1.0);

	if (perspectiva)
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
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glMapGrid2f(n, 0.0, 1.0, n, 0.0, 1.0);
	initlights();
}

void myReshape(int w, int h)
{
	GLfloat f = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (perspectiva)
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
	case 'f':
		fill = !fill;
		break;
	case 27:
		exit(0);
		break;
	default: break;
	}
	display();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	glutCreateWindow(argv[0]);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(inputKey);

	glutMainLoop();
	return 0;
}

