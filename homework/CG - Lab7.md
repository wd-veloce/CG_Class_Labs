## Lab 7 - A Simple 3D Scene

**Lab 7 is also known as Lab10 & Lab11 in CG Class Labs**

Create a scene which has one camera and two objects. It is a 3D scene and can be displayed in perspective projection. The camera and the objects can move, pitch, yaw and roll. 

This solution create a scene which has a camera and to cubes. The following shows how to control the camera:
> Camera Control:
> 'w' moves camera forward.
> 's' moves camera backward.
> 'a' moves camera left.
> 'd' moves camera right.
> 'q' moves camera up.
> 'e' moves camera down.
> 'r' pitches up.
> 'f' pitches down.
> 't' rolls clockwise.
> 'g' rolls anticlockwise.
> 'y' pitches left.
> 'h' pitches right.

The floowing shows how to control the objects:
> object control:
> '1' changes to object one.
> '2' chaeges to object two.
> ( The default object will be object one. )
>
> 'i' moves object forward.
> 'k' moves object backward.
> 'j' moves object left.
> 'l' moves object right.
> 'u' moves object up.
> 'o' moves object down.
> 'p' pitches up.
> ';' pitches down.
> '.' rolls clockwise.
> ',' rolls anticlockwise.
> 'n' pitches left.
> 'm' pitches right.

``` cpp
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

const double M_PI = 3.14159265358979323846;

enum Direction {
	FRONT_AND_BACK, LEFT_AND_RIGHT, UP_AND_DOWN 
};

class Base {
protected:
	GLfloat posx, posy, posz;
	GLfloat centerx, centery, centerz;
	GLfloat upx, upy, upz;
	GLfloat rightx, righty, rightz;

public:
	Base(float a, float b, float c,
		float d, float e, float f,
		float g, float h, float i)
	{
		posx = a; posy = b; posz = c;
		centerx = d - a; centery = e - b; centerz = f - c;
		upx = g; upy = h; upz = i;

		rightx = centery * upz - centerz * upy;
		righty = centerz * upx - centerx * upz;
		rightz = centerx * upy - centery * upx;

		double base;
		base = sqrt(centerx*centerx + centery * centery + centerz * centerz);
		centerx /= base;
		centery /= base;
		centerz /= base;

		base = sqrt(upx*upx + upy * upy + upz * upz);
		upx /= base;
		upy /= base;
		upz /= base;

		base = sqrt(rightx*rightx + righty * righty + rightz * rightz);
		rightx /= base;
		righty /= base;
		rightz /= base;
	}
	void move(Direction d, float speed) {
		float x, y, z;
		switch (d) {
		case FRONT_AND_BACK:
			x = centerx; y = centery; z = centerz;
			break;
		case LEFT_AND_RIGHT:
			x = rightx; y = righty; z = rightz;
			break;
		case UP_AND_DOWN:
			x = upx; y = upy; z = upz;
			break;
		default:
			break;
		}
		posx = posx + x * speed; posy = posy + y * speed; posz = posz + z * speed;
	}
	void pitch(float degree) {
		const long double c = cosl(degree * M_PI / 180.0f);
		const long double s = sinl(degree * M_PI / 180.0f);
		float tmpx, tmpy, tmpz;

		tmpx = (rightx*rightx*(1.0f - c) + c)*centerx + (rightx*righty*(1.0f - c) - rightz * s)*centery + (rightx*rightz*(1.0f - c) + righty * s)*centerz;
		tmpy = (rightx*righty*(1.0f - c) + rightz * s)*centerx + (righty*righty*(1.0f - c) + c)*centery + (righty*rightz*(1.0f - c) - rightx * s)*centerz;
		tmpz = (rightx*rightz*(1.0f - c) - righty * s)*centerx + (righty*rightz*(1.0f - c) + rightx * s)*centery + (rightz*rightz*(1.0f - c) + c)*centerz;
		centerx = tmpx; centery = tmpy; centerz = tmpz;

		tmpx = (rightx*rightx*(1.0f - c) + c)*upx + (rightx*righty*(1.0f - c) - rightz * s)*upy + (rightx*rightz*(1.0f - c) + righty * s)*upz;
		tmpy = (rightx*righty*(1.0f - c) + rightz * s)*upx + (righty*righty*(1.0f - c) + c)*upy + (righty*rightz*(1.0f - c) - rightx * s)*upz;
		tmpz = (rightx*rightz*(1.0f - c) - righty * s)*upx + (righty*rightz*(1.0f - c) + rightx * s)*upy + (rightz*rightz*(1.0f - c) + c)*upz;
		upx = tmpx; upy = tmpy; upz = tmpz;
	}
	void roll(float degree) {
		const long double c = cosl(degree * M_PI / 180.0f);
		const long double s = sinl(degree * M_PI / 180.0f);
		float tmpx, tmpy, tmpz;

		tmpx = (centerx*centerx*(1.0f - c) + c)*rightx + (centerx*centery*(1.0f - c) - centerz * s)*righty + (centerx*centerz*(1.0f - c) + centery * s)*rightz;
		tmpy = (centerx*centery*(1.0f - c) + centerz * s)*rightx + (centery*centery*(1.0f - c) + c)*righty + (centery*centerz*(1.0f - c) - centerx * s)*rightz;
		tmpz = (centerx*centerz*(1.0f - c) - centery * s)*rightx + (centery*centerz*(1.0f - c) + centerx * s)*righty + (centerz*centerz*(1.0f - c) + c)*rightz;
		rightx = tmpx; righty = tmpy; rightz = tmpz;

		tmpx = (centerx*centerx*(1.0f - c) + c)*upx + (centerx*centery*(1.0f - c) - centerz * s)*upy + (centerx*centerz*(1.0f - c) + centery * s)*upz;
		tmpy = (centerx*centery*(1.0f - c) + centerz * s)*upx + (centery*centery*(1.0f - c) + c)*upy + (centery*centerz*(1.0f - c) - centerx * s)*upz;
		tmpz = (centerx*centerz*(1.0f - c) - centery * s)*upx + (centery*centerz*(1.0f - c) + centerx * s)*upy + (centerz*centerz*(1.0f - c) + c)*upz;
		upx = tmpx; upy = tmpy; upz = tmpz;
	}
	void yaw(float degree) {
		const long double c = cosl(degree * M_PI / 180.0f);
		const long double s = sinl(degree * M_PI / 180.0f);
		float tmpx, tmpy, tmpz;

		tmpx = (upx*upx*(1.0f - c) + c)*centerx + (upx*upy*(1.0f - c) - upz * s)*centery + (upx*upz*(1.0f - c) + upy * s)*centerz;
		tmpy = (upx*upy*(1.0f - c) + upz * s)*centerx + (upy*upy*(1.0f - c) + c)*centery + (upy*upz*(1.0f - c) - upx * s)*centerz;
		tmpz = (upx*upz*(1.0f - c) - upy * s)*centerx + (upy*upz*(1.0f - c) + upx * s)*centery + (upz*upz*(1.0f - c) + c)*centerz;
		centerx = tmpx; centery = tmpy; centerz = tmpz;

		tmpx = (upx*upx*(1.0f - c) + c)*rightx + (upx*upy*(1.0f - c) - upz * s)*righty + (upx*upz*(1.0f - c) + upy * s)*rightz;
		tmpy = (upx*upy*(1.0f - c) + upz * s)*rightx + (upy*upy*(1.0f - c) + c)*righty + (upy*upz*(1.0f - c) - upx * s)*rightz;
		tmpz = (upx*upz*(1.0f - c) - upy * s)*rightx + (upy*upz*(1.0f - c) + upx * s)*righty + (upz*upz*(1.0f - c) + c)*rightz;
		rightx = tmpx; righty = tmpy; rightz = tmpz;
	}
	virtual void display() = 0;
};

class Object : public Base {
public:
	Object(float a, float b, float c,
		float d, float e, float f,
		float g, float h, float i) : Base(a, b, c, d, e, f, g, h, i)
	{	}
	void display() {
		glColor3f(0.0f, 0.0f, 0.0f);
		glPopMatrix();
		const GLfloat tgtMatrix[] = { centerx, centery, centerz, 0.0f, upx, upy, upz, 0.0f, rightx, righty, rightz, 0.0f, posx, posy, posz, 1.0f };
		glMultMatrixf(tgtMatrix);
		glutWireCube(1.5f);
	}
};

class Camera : public Base{
public:
	Camera(float a, float b, float c,
		float d, float e, float f,
		float g, float h, float i) : Base(a, b, c, d, e, f, g, h, i)
	{	}
	void display() {
		gluLookAt(posx, posy, posz, posx+centerx, posy+centery, posz+centerz, upx, upy, upz);
	}
};

static Camera sceneCam(0.0f, 2.0f, 10.0f,  0.0f, 1.0f, -100.0f,  0.0f, 1.0f, 0.0f);
static Object cube1(0.0f, 0.0f, 5.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);
static Object cube2(0.0f, 0.0f, -10.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f);
static Object *cube = &cube1;

void drawPlane() {
	glBegin(GL_LINES);
	for (int i = -100.0f; i < 100.f; i += 1) {
		if (i == 0) {
			glColor3f(0.0f, 0.0f, 0.0f);
		}
		else {
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		glVertex3f(i, 0.0f, 100.0f);
		glVertex3f(i, 0.0f, -100.0f);
		glVertex3f(100.0f, 0.0f, i);
		glVertex3f(-100.0f, 0.0f, i);
	}
	glEnd();
}

void display(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	sceneCam.display();

	int ObjNum = 2;
	for (int i = 0; i < ObjNum; i++) {
		glPushMatrix();
	}

	drawPlane();
	cube1.display();
	cube2.display();
	
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat) w / (GLfloat) h, 0.0f, 20.0f);
}

void onKeyPress(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		cube = &cube1;
		break;
	case '2':
		cube = &cube2;
		break;
	default:
		break;
	}
	switch (key) {
	case 'w':
		sceneCam.move(FRONT_AND_BACK, 0.1f);
		break;
	case 's':
		sceneCam.move(FRONT_AND_BACK, -0.1f);
		break;
	case 'a':
		sceneCam.move(LEFT_AND_RIGHT, -0.1);
		break;
	case 'd':
		sceneCam.move(LEFT_AND_RIGHT, 0.1f);
		break;
	case 'q':
		sceneCam.move(UP_AND_DOWN, 0.1f);
		break;
	case 'e':
		sceneCam.move(UP_AND_DOWN, -0.1f);
		break;
	case 'r':
		sceneCam.pitch(1);
		break;
	case 'f':
		sceneCam.pitch(-1);
		break;
	case 't':
		sceneCam.roll(1);
		break;
	case 'g':
		sceneCam.roll(-1);
		break;
	case 'y':
		sceneCam.yaw(1);
		break;
	case 'h':
		sceneCam.yaw(-1);
		break;
	case 'i':
		cube->move(FRONT_AND_BACK, 0.1f);
		break;
	case 'k':
		cube->move(FRONT_AND_BACK, -0.1f);
		break;
	case 'j':
		cube->move(LEFT_AND_RIGHT, -0.1f);
		break;
	case 'l':
		cube->move(LEFT_AND_RIGHT, 0.1f);
		break;
	case 'u':
		cube->move(UP_AND_DOWN, 0.1f);
		break;
	case 'o':
		cube->move(UP_AND_DOWN, -0.1f);
		break;
	case 'p':
		cube->pitch(1);
		break;
	case ';':
		cube->pitch(-1);
		break;
	case 'n':
		cube->yaw(1);
		break;
	case 'm':
		cube->yaw(-1);
		break;
	case ',':
		cube->roll(-1);
		break;
	case '.':
		cube->roll(1);
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Lab 10 & 11");
	
	glutIdleFunc(&display);
	glutDisplayFunc(&display);
	glutReshapeFunc(&reshape);
	glutKeyboardFunc(&onKeyPress);
	glutMainLoop();
	return 0;
}
```