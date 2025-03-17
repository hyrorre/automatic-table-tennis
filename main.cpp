#if defined(WIN32) || defined(WIN64) // Windowsのとき
#include "glut.h"
#include <GL/gl.h>
#else // macOSのとき (普段はmacOSで開発しています)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <stdlib.h>
#endif

#include <math.h>

void KeyboardHandler(unsigned char key, int x, int y);

void Display(void);
const float green[] = { 0.0, 0.2, 0.0, 1.0 };
const float lgreen[] = { 0.0, 0.4, 0.0, 1.0 };
const float amber[] = { 1.0, 0.5, 0.0, 1.0 };
const float red[] = { 0.3, 0.0, 0.0, 1.0 };
const float lred[] = { 0.6, 0.0, 0.0, 1.0 };

const double pi = 4 * atan(1);

const float light_pos[] = { 0, 0.5, 1, 0 };

float rad = 0, degree = 0;
float table_rad = 0, table_degree = 0;
const int update_ms = 17;
const int update_timer_id = 1;

void Update(int timer_id);

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Automatic Table Tennis");

	glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35, 1, 1, 400);
	gluLookAt(0, 2.5, 0, 0, 1.2, -3, 0, 1, 0);

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(Display);
	glutKeyboardFunc(KeyboardHandler);
	glutTimerFunc(update_ms, Update, update_timer_id);
	glutMainLoop();
}

void KeyboardHandler(unsigned char key, int x, int y) {
	if (key == ' ')
		exit(0);
}

void Prism(int N) {
	double angle = 2 * pi / N;
	int i;

	glBegin(GL_POLYGON);      //上面
	glNormal3f(0.0, 1.0, 0.0);
	for(i = 0; i < N; i++) {
		glVertex3f(cos(i*angle), 1.0, sin(i*angle));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for(i = 0; i <= N; i++) {
		glNormal3f(cos((i - 0.5)*angle), 0.0, sin((i - 0.5)*angle));
		glVertex3f(cos(i*angle), -1.0, sin(i*angle));
		glVertex3f(cos(i*angle), +1.0, sin(i*angle));
	}
	glEnd();

	glBegin(GL_POLYGON);      //下面
	glNormal3f(0.0, -1.0, 0.0);
	for(i = 0; i < N; i++) {
		glVertex3f(cos(i*angle), -1.0, sin(i*angle));
	}
	glEnd();

}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glTranslatef(0, -1.5, -8);
	glRotatef(table_degree, 0, 1, 0);

	// 卓球台のネット
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	{
		for(float y = -0.1; y <= 0.11; y += 0.05){
			glVertex3f(0, y, -1.5);
			glVertex3f(0, y, 1.5);
		}
		for(float z = -1.5; z <= 1.51; z += 0.05){
			glVertex3f(0, -0.1, z);
			glVertex3f(0, 0.1, z);
		}
	}
	glEnd();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	// ピン球
	glLightfv(GL_LIGHT0, GL_AMBIENT, amber);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, amber);
	glPushMatrix();
	{
		glTranslatef(fabs(4 - 4 * rad / pi) - 2, 1.2 * fabs(sin(rad + pi / 3)), 0);
		glRotatef(90, 1, 0, 0);
		glutSolidSphere(0.1, 20, 20);
	}
	glPopMatrix();

	// 卓球台
	glLightfv(GL_LIGHT0, GL_AMBIENT, lgreen);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, green);
	glPushMatrix();
	{
		glTranslatef(0, -0.2, 0);
		glScalef(4, 0.2, 3);
		glutSolidCube(1);
	}
	glPopMatrix();

	// ラケット

	for(int i = 0; i < 2; i++){
		glPushMatrix();
		{
			glTranslatef((0.5 - i) * 2 * 2.2, 1.0, (0.5 - i) * 2 * 0.35);
			glPushMatrix();
			{
				glRotatef(degree + 90, 0, 1, 0);
				glRotatef(90, 1, 0, 0);
				glPushMatrix();
				{ // ラケットの面
					glLightfv(GL_LIGHT0, GL_AMBIENT, lred);
					glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
					glTranslatef(0.45, 0, 0);
					glScalef(0.3, 0.05, 0.3);
					Prism(20);
				}
				glPopMatrix();
				glPushMatrix();
				{ // ラケットのグリップ
					glLightfv(GL_LIGHT0, GL_AMBIENT, amber);
					glLightfv(GL_LIGHT0, GL_DIFFUSE, amber);
					glScalef(0.3, 0.1, 0.1);
					glutSolidCube(1);
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}

	glFlush();
}

void Update(int timer_id) {
	if (timer_id != update_timer_id) return;

	rad += 0.03;
	if(rad >= 2 * pi) rad -= 2 * pi;
	degree = rad * 360 / (2 * pi);

	table_rad += 0.005;
	if(table_rad >= 2 * pi) table_rad -= 2 * pi;
	table_degree = table_rad * 360 / (2 * pi);


	glutPostRedisplay();
	glutTimerFunc(update_ms, Update, update_timer_id);
}
