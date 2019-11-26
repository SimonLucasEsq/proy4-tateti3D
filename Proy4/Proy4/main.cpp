#include "GL/freeglut.h"
#include <iostream>
using namespace std;

float camx = 0.0, camy = 0.0, camz = 0.0;
float atx = 0.0, aty = 0.0;


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
		0.0 + camx, 5.0 + camy, 10.0 + camz,
		//hacia donde mira la cámara
		0.0 + atx, 0.0 + aty, 0.0,
		//orientación de la cámara (para que pueda mirar de cabeza o de costado)
		0.0, 1.0, 0.0
	);//sitúa el punto de vista respecto al punto de atención

	drawCube();

	glFlush();
	glutSwapBuffers();

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
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);//activar la matriz de proyeccion
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);// definicion del volumen de visualizacion
	glMatrixMode(GL_MODELVIEW);//restaurar la matriz de modelo-vista como activa*/
}

void keyboard(unsigned char key, int x, int y) {
	float speed = 0.2;
	switch (key) {
		// para mover la cámara
	case 'a':
	case 'A':
		camx -= speed;
		display();
		break;
	case 'd':
	case 'D':
		camx += speed;
		display();
		break;

	case 'w':
	case 'W':
		camz -= speed;
		display();
		break;

	case 's':
	case 'S':
		camz += speed;
		display();
		break;

	case 'q':
	case 'Q':
		camy += speed;
		display();
		break;

	case 'e':
	case 'E':
		camy -= speed;
		display();
		break;

		// para mover el punto de vista
	case 'j':
	case 'J':
		atx -= speed;
		display();
		break;
	case 'l':
	case 'L':
		atx += speed;
		display();
		break;

	case 'i':
	case 'I':
		aty += speed;
		display();
		break;

	case 'k':
	case 'K':
		aty -= speed;
		display();
		break;
	}
}