#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

static int windowNumber;

void refreshFunc() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 27; i++) {
		float x1 = 0;
		float y1 = i * 40;
		glVertex2f(x1, y1);
		float x2 = 1920;
		float y2 = i * 40;
		glVertex2f(x2, y2);
	}
	for (int i = 0; i < 48; i++) {
		float x1 = i * 40;
		float y1 = 0;
		glVertex2f(x1, y1);
		float x2 = i * 40;
		float y2 = 1080;
		glVertex2f(x2, y2);
	}
	glEnd();
	glFlush();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1920, 1080);
	windowNumber = glutCreateWindow("Mesh Generator");
	gluOrtho2D(0.0f, 1920.0f, 0.0f, 1080.0f);
	glutFullScreen();

	refreshFunc();

	glutMainLoop();
	return 0;
}