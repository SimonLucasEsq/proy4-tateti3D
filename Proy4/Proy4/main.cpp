#include "GL/freeglut.h"
void init(void) {
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
}
void drawLines() {
	int i = 0;
	float w = 1;
	float z = -1;
	float c = 0;
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(3.0);
	glBegin(GL_POINTS);
	for (i = 0; i < 200; i++) {
		glColor3f(w, c, z);
		glVertex2d(z, 0);
		glVertex2d(0, w);
		z += .01f;
		w -= .01f;
		c -= .01f;
	}
	glEnd();
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(800, 400);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Example");
	init();
	glutDisplayFunc(drawLines);
	glutMainLoop();
	return EXIT_SUCCESS;
}