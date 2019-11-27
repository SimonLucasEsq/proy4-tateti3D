#include "GL/freeglut.h"
#include <iostream>
#include <cmath>
GLuint texture;
GLuint LoadTexture(const char* filename)
{

	GLuint texture;

	int width, height;

	unsigned char* data;

	FILE* file;

	fopen_s(&file, filename, "rb");

	if (file == NULL) return 0;
	width = 1024;
	height = 512;
	data = (unsigned char*)malloc(width * height * 3);
	//int size = fseek(file,);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;

	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}
void init(void) {
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}
void display(void) {
	glEnable(GL_TEXTURE_2D);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();//limpia la matriz 
    //sitúa el punto de vista respecto al punto de atención
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
	glBegin(GL_QUADS);
	//glColor3f(0,1,0); 
	glTexCoord2i(1, 0);
	glVertex3f(-0.5, -0.5, -0.5);

	//glColor3f(0,0,1); 
	glTexCoord2i(0, 0);
	glVertex3f(0.5, -0.5, -0.5);

	//glColor3f(1,0,0); 
	glTexCoord2i(0, 1);
	glVertex3f(0.5, 0.5, -0.5);

	//glColor3f(1,0,0); 
	glTexCoord2i(1, 1);
	glVertex3f(-0.5, 0.5, -0.5);
	glEnd();
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	// suppose that the texture has been loaded to texImage
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glutSolidSphere(1.0, 20, 20);	// draw a sphere with this sphere map
	glPopMatrix();
	glFlush();

}
int main1(int argc, char** argv) {
	
	
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(720, 640);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Proy 4 | Simon Esquivel | Martin Tamay");
	init();
	glDepthFunc(GL_ALWAYS);
	texture = LoadTexture("imagen.bmp");
	glBindTexture(GL_TEXTURE_2D, texture);
	glutDisplayFunc(display);
	glutMainLoop();
	return EXIT_SUCCESS;
}