#include "GL/freeglut.h"
#include <iostream>
#include <cmath>
using namespace std;

float zoom = 12.0;
float zoomSensibility = 0.2;
float camspeed = 0.1;

float camx = 0.0,	camy = 0.0, camz = zoom;
float angx = 0.0,	angy = 0.0, angz = 0.0;
float tx = 0.0,		ty = 1.0,	tz = 0.0;

int width, height;

void reshape(int w, int h);
void drawCube();
void keyboard(unsigned char key, int x, int y);

void init(void) {
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glLoadIdentity();//limpia la matriz 
	gluLookAt(
		//posición de la cámara
		camx, camy, camz,
		//0.0, 0.0, 0.0,
		//hacia donde mira la cámara
		0.0, 0.0, 0.0,
		//orientación de la cámara (para que pueda mirar de cabeza o de costado)
		//0.0, 0.0, 1.0
		tx, ty, tz
	);//sitúa el punto de vista respecto al punto de atención
	/* test de camara 
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(tx, ty, tz);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(camx, camy, camz);
	glEnd();
	*/
	drawCube();

	glFlush();
	glutSwapBuffers();

}

void mouseFunc(int button, int state, int coor_x, int coor_y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		float x = ((float)coor_x / (float)width) * 2.0 - 1.0;
		float y = ((float)coor_y / (float)height) * 2.0 - 1.0;
		cout << x << ", " << y << "\n";
	}
}

void mouseWheel(int button, int dir, int x, int y) {
	if (dir > 0) {
		zoom -= zoomSensibility;
	}
	else {
		zoom += zoomSensibility;
	}

	float sx = sin(angx);
	float cx = cos(angx);
	float sy = sin(angy);
	float cy = cos(angy);

	camx = zoom * (sy);
	camy = zoom * (sx * cy);
	camz = zoom * (cx * cy);

	display();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(720, 640);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);

	glutCreateWindow("Proy 4 | Simon Esquivel | Martin Tamay");
	init();
	glDepthFunc(GL_ALWAYS);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseFunc);
	glutMouseWheelFunc(mouseWheel);
	glutMainLoop();
	return EXIT_SUCCESS;
}

bool centerTest(int x, int y, int z) {
	unsigned zeroCount = 0;
	if (x == 0) {
		zeroCount++;
	}
	if (y == 0) {
		zeroCount++;
	}
	if (z == 0) {
		zeroCount++;
	}
	return zeroCount > 1;
}

void drawCube() {
	float count = 0.0;
	for (int z = -1; z <= 1; z++) {
		for (int y = -1; y <= 1; y++) {
			for (int x = -1; x <= 1; x++) {
				count += 0.02;
				glColor3f(count, 1-count, 1-count);
				glPushMatrix();
				glTranslatef(x * 1.5, y * 1.5, z * 1.5);
				if (centerTest(x, y, z)) {
					glutSolidCube(1);
				}
				else {
					glutSolidSphere(0.5, 50, 50);
				}
				glPopMatrix();
			}
		}
	}

	glColor3f(0, 1, 0);
	glutWireCube(3.0);
	glutWireCube(1.0);
}


void reshape(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);//activar la matriz de proyeccion
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);// definicion del volumen de visualizacion
	glMatrixMode(GL_MODELVIEW);//restaurar la matriz de modelo-vista como activa*/
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		// para mover la cámara
	case 'a':
	case 'A':
		angy -= camspeed;
		break;
	case 'd':
	case 'D':
		angy += camspeed;
		break;

	case 'w':
	case 'W':
		angx -= camspeed;
		break;

	case 's':
	case 'S':
		angx += camspeed;
		break;
	case 'q':
	case 'Q':
		angz -= camspeed;
		break;
	case 'e':
	case 'E':
		angz += camspeed;
		break;
	}

	float sx = sin(angx);
	float cx = cos(angx);
	float sy = sin(angy);
	float cy = cos(angy);

	camx = zoom * (sy);
	camy = zoom * (sx * cy);
	camz = zoom * (cx * cy);

	sx = sin(-angx);
	cx = cos(-angx);
	sy = sin(-angy);
	cy = cos(-angy);
	float sz = sin(-angz);
	float cz = cos(-angz);

	tx = -(sz * cy);
	ty = -sx * sy * sz + cx * cz;
	tz = cx * sy * sz + cz * sx;

	display();
}