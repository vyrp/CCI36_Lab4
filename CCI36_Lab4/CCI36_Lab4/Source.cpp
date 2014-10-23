#include <stdio.h>
#include <GL/glut.h>

int sidewidth = 800;
int sideheight = 600;

// draw a car centered at x=0 and z=0, and with bottom y=0
// with y as the up direction
// with z as the front direction
// with length 2 and width 1
void DrawCar() {
	// draw the driver
	glPushMatrix();
	glTranslatef(0.25, 1.25, 0.5);
	glutSolidSphere(0.25, 10, 10); // r, nLatitudes, nLongitudes
	glPopMatrix();
	// draw the car
	glPushMatrix();
	glTranslatef(0.0, 0.5, 0.0);
	glScalef(1.0, 1.0, 2.0);
	glutSolidCube(1); // side length
	glPopMatrix();
}

void sideGlutDisplay()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black 
	glClear(GL_COLOR_BUFFER_BIT); // Clear the background)

	float light_ambient[4] = { 0.2, 0.2, 0.2, 1.0 }; // r, g, b, a
	float light_diffuse[4] = { 0.8, 0.3, 0.1, 1.0 }; // r, g, b, a
	float light_specular[4] = { 0.8, 0.3, 0.1, 1.0 }; // r, g, b, a
	float light_position[4] = { -1.0, 0.0, 0.0, 0.0 }; // x, y, z, w

	float ad_col[4] = { 1.0, 0.5, 0.5, 1.0 }; // r, g, b, a
	float ad_col2[4] = { 1.0, 1.0, 1.0, 1.0 }; // r, g, b, a
	float spec_col[4] = { 1.0, 1.0, 1.0, 1.0 }; // r, g, b, a

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)sidewidth / (float)sideheight, 5.0, 5000.0);
	// theta, aspect, dnear, dfar
	glViewport(0, 0, sidewidth, sideheight); // startx, starty, xsize, ysize 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ad_col);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec_col);

	// viewer is at (-10,10,-10) looking towards the center of the terrain
	gluLookAt(-10, 10, -10, 128, 0, 128, 0, 1, 0);

	// draw the terrain
	// a 256x256 square with lower left corner (0,0,0)
	// up direction is y
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(256.0, 0.0, 0.0);
	glVertex3f(256.0, 0.0, 256.0);
	glVertex3f(0.0, 0.0, 256.0);
	glEnd();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ad_col2);
	// draw the first car at the origin pointing in the z direction
	glPushMatrix();
	DrawCar();
	glPopMatrix();
	// draw the second car at (0,0,10) pointing 45 degrees inwards 
	glPushMatrix();
	glTranslatef(0.0, 0.0, 10.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	DrawCar();
	glPopMatrix();
	glutSwapBuffers();

	glFlush(); // Render now 
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);					// Inicializa GLUT 
	
	glutInitWindowSize(sidewidth, sideheight);// Tamanho de Janela
	glutInitWindowPosition(200, 100);		// Posiçao inicial da janela
	glutCreateWindow("CCI-36 Lab4");		// Cria Janela
	glutDisplayFunc(sideGlutDisplay);		// Registra display pra repintar janela
	glutMainLoop();							// Main Loop de eventos
}
