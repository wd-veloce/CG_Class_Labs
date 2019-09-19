## Lab 1 - Draw the function:

f(x) = sin x / x<br>
x ∈ [-20, 20]<br>

Using the following transform:<br>
sx = Ax + B<br>
sy = Cy + D<br>

```C++
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

const GLfloat factor = 0.1f;
void myDisplay(void) {
	GLfloat x;
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(-100.0f, 0.0f);
	glVertex2f(100.0f, 0.0f);
	glVertex2f(0.0f, -100.0f);
	glVertex2f(0.0f, 100.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (x = -100.0f / factor; x < 100.0f / factor; x += 0.01f) {
		glVertex2f(x*factor, sin(x)*factor/x);
	}
	glEnd();
	glFlush();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("OpenGL函数图像程序");
	gluOrtho2D(-3.0f, 3.0f, -0.1f, 0.3f);
	glutDisplayFunc(&myDisplay);
	glutMainLoop();
	return 0;
}
```