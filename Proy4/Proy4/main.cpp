#include "GL/freeglut.h"
#include <iostream>
#include <cmath>
#include "vecmath/vecmath.h"
#include "vecmath/mat.h"
#include "vecmath/vec.h"
#include "Cube.h"

using namespace std;

const double HUGE_DOUBLE = 1e100;
const double RAY_EPSILON = 0.00001;
const double NORMAL_EPSILON = 0.00001;

float zoom = 9.0;
float zoomSensibility = 0.2;
float camspeed = 0.1;

float camx = 0.0,	camy = 0.0, camz = zoom;
Vec3f rightVect(1.0f, 0.0f, 0.0f);
Vec3f upVect(0.0f, 1.0f, 0.0f);
Vec3f backVect(0.0f, 0.0f, 1.0f);
Vec3f col(0.0f, 0.0f, 0.0f);
Vec3f antCam(0.0f, 0.0f, 0.0f);
Mat3f rotMatrix(Vec3f u, float ang);

int currPlayer = 1;
int width = 720, height = 720;

Cube cube;

void init(void);
void reshape(int w, int h);
bool centerTest(int x, int y, int z);
void drawCube();
void keyboard(unsigned char key, int x, int y);
float collideBox(Vec3f o, Vec3f d);
float collideSphere(Vec3f o, Vec3f d);
int* checkCollide(Vec3f d);

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
		upVect[0], upVect[1], upVect[2]
	);//sitúa el punto de vista respecto al punto de atención
	/* test de camara */


	drawCube();
	glColor3f(1.0, 0.5, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(upVect[0], upVect[1], upVect[2]);
	glEnd();
	glColor3f(1.0, 1.0, 0.5);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(rightVect[0], rightVect[1], rightVect[2]);
	glEnd();
	glColor3f(0.5, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(backVect[0], backVect[1], backVect[2]);
	glEnd();

	// el clic
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(col[0], col[1], col[2]);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(antCam[0], antCam[1], antCam[2]);
	glVertex3f(antCam[0] + col[0] * 100, antCam[1] + col[1] * 100, antCam[2] + col[2] * 100);
	glEnd();


	glFlush();
	glutSwapBuffers();

}

void mouseFunc(int button, int state, int coor_x, int coor_y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		antCam = Vec3f(camx, camy, camz);
		// obtengo el punto de impacto en el espacio 3d antes de mover la camara
		float x = ((2.0f * (float)coor_x) / (float)width) - 1.0f;
		float y = 1.0f - (((float)coor_y * 2) / (float)height);

		float z = -1.0f;

		float angx = tan(x);
		float angy = tan(y);

		cout << "camera " << x << ", " << y;

		Mat3f rotx = rotMatrix(upVect, -angx);
		Mat3f roty = rotMatrix(rightVect, angy);
		//Mat3f rotz = rotMatrix(rightVect, 180);
		Mat3f rot = rotx * roty;
		Mat3f points(
			backVect[0], 0.0f, 0.0f,
			backVect[1], 0.0f, 0.0f,
			backVect[2], 0.0f, 0.0f
		);
		Mat3f resp = rot * points;
		Vec3f dir(-resp[0][0], -resp[1][0] - 0.02, -resp[2][0]);

		dir.normalize();


		col = dir;
		// compruebo si colisiona con algun ojeto
		int* collide = checkCollide(dir);
		if (collide != NULL) {
			int player = cube.playerAt(collide[0], collide[1], collide[2]);
			cout << "choco con x:" << collide[0] << ", y:" << collide[1] << ", z:" << collide[2] << " player: " << player << "\n";
			if (player == 0) {
				cube.mark(currPlayer, collide[0], collide[1], collide[2]);
			}
			else {
				// TODO: anunciar que es una pieza ocupada
			}
		}
		else {
			cout << "sin choque\n";
		}
		delete[] collide;
	}
	display();
}


int* checkCollide(Vec3f d) {
	// usar las colisiones del proyecto 3
	float collideDist = INFINITY;
	int* retPos = new int[3];
	for (int z = -1; z <= 1; z++) {
		for (int y = -1; y <= 1; y++) {
			for (int x = -1; x <= 1; x++) {
				float collided;
				Vec3f lo(camx - x * 1.5, camy - y * 1.5, camz - z * 1.5);
				if (centerTest(x, y, z)) {
					collided = collideBox(lo, d);
				}
				else {
					collided = collideSphere(lo, d);
				}
				if (collided >= 0 && collided < collideDist) {
					collideDist = collided;
					retPos[0] = x + 1;
					retPos[1] = y + 1;
					retPos[2] = z + 1;
				}
			}
		}
	}
	if (collideDist != INFINITY) {
		cout << "\nt: " << collideDist << "\n";
		return retPos;
	}
	return NULL;
}

float collideSphere(Vec3f o, Vec3f d) {
	float k, c, t0, t1, disc, ft;
	unsigned x = 0, y = 1, z = 2;

	k = o[x] * d[x] + o[y] * d[y] + o[z] * d[z];
	c = o[x] * o[x] + o[y] * o[y] + o[z] * o[z] - 0.25;

	disc = k * k - c;

	if (disc < 0) {
		return -1;
	}

	if (disc == 0.0) {
		ft = -k + sqrt(disc);
	}
	else {
		t0 = -k + sqrt(disc);
		t1 = -k - sqrt(disc);
		if (t0 >= 0 && t1 >= 0) {
			if (t0 < t1) {
				ft = t0;
			}
			else {
				ft = t1;
			}
		}
		else if (t0 >= 0) {
			ft = t0;
		}
		else {
			ft = t1;
		}
	}

	Vec3f pos = o + d * ft;
	cout << "l" << pos.length()<<"  ";
	if (pos.length() > 0.5f) {
		return -1;
	}
	return ft;
}

float collideBox(Vec3f o, Vec3f d) {
	int it;
	float x, y, t, bestT;
	int mod0, mod1, mod2, bestIndex;

	bestT = HUGE_DOUBLE;
	bestIndex = -1;

	for (it = 0; it < 6; it++) {
		mod0 = it % 3;

		if (d[mod0] == 0) {
			continue;
		}

		t = ((it / 3) - 0.5 - o[mod0]) / d[mod0];

		mod1 = (it + 1) % 3;
		mod2 = (it + 2) % 3;
		x = o[mod1] + t * d[mod1];
		y = o[mod2] + t * d[mod2];

		if (x <= 0.5 && x >= -0.5 &&
			y <= 0.5 && y >= -0.5)
		{
			if (bestT > t) {
				bestT = t;
				bestIndex = it;
			}
		}

	}

	if (bestIndex < 0) return -1;

	return bestT;
}

void mouseWheel(int button, int dir, int x, int y) {
	float move = zoomSensibility * -dir;
	zoom += move;

	camx += move * backVect[0];
	camy += move * backVect[1];
	camz += move * backVect[2];
	cout << "zoom " << zoom << "\n";
	display();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB);

	glutCreateWindow("Proy 4 | Simon Esquivel | Martin Tamay");
	cube = Cube();
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
				int player = cube.playerAt(x + 1, y + 1, z + 1);
				// agregar textura dependiendo del player
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


void init(void) {
	glClearColor(0, 0, 0, 0);
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1, 20.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void reshape(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);//activar la matriz de proyeccion
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1, 20.0);// definicion del volumen de visualizacion
	glMatrixMode(GL_MODELVIEW);//restaurar la matriz de modelo-vista como activa*/
}

Mat3f rotMatrix(Vec3f u, float ang) {
	unsigned x = 0, y = 1, z = 2;
	float c = cos(ang);
	float s = sin(ang);
	float t = tan(ang);


	Mat3f rot(
		c + pow(u[x], 2) * (1 - c), u[x] * u[y] * (1 - c) - u[z] * s, u[x] * u[z] * (1 - c) + u[y] * s,
		u[y] * u[x] * (1 - c) + u[z] * s, c + pow(u[y], 2) * (1 - c), u[y] * u[z] * (1 - c) - u[x] * s,
		u[z] * u[x] * (1 - c) - u[y] * s, u[z] * u[y] * (1 - c) + u[x] * s, c + pow(u[z], 2) * (1 - c)
	);
	return rot;
}

void keyboard(unsigned char key, int x_key, int y_key) {
	int dir = 1;
	Vec3f u, *v2, *v3;
	switch (key) {
		// para mover la cámara
	case 'w':
	case 'W':
		u = rightVect;
		v2 = &upVect;
		v3 = &backVect;
		break;
	case 's':
	case 'S':
		dir = -1;
		u = rightVect;
		v2 = &upVect;
		v3 = &backVect;
		break;
	case 'a':
	case 'A':
		u = upVect;
		v2 = &rightVect;
		v3 = &backVect;
		break;
	case 'd':
	case 'D':
		dir = -1;
		u = upVect;
		v2 = &rightVect;
		v3 = &backVect;
		break;
	case 'q':
	case 'Q':
		u = backVect;
		v2 = &rightVect;
		v3 = &upVect;
		break;
	case 'e':
	case 'E':
		dir = -1;
		u = backVect;
		v2 = &rightVect;
		v3 = &upVect;
		break;
	default:
		return;
	}

	unsigned x = 0, y = 1, z = 2;
	Mat3f rot = rotMatrix(u, camspeed * dir);
	Mat3f points(
		camx, (*v2)[x], (*v3)[x],
		camy, (*v2)[y], (*v3)[y],
		camz, (*v2)[z], (*v3)[z]
	);

	Mat3f rest = rot * points;

	camx = rest[0][0];
	camy = rest[1][0];
	camz = rest[2][0];
	(*v2)[0] = rest[0][1];
	(*v2)[1] = rest[1][1];
	(*v2)[2] = rest[2][1];
	(*v3)[0] = rest[0][2];
	(*v3)[1] = rest[1][2];
	(*v3)[2] = rest[2][2];

	display();
}