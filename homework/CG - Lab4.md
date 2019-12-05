## Lab 4 - Animation with tweening

Transform a tree pattern to a letter "F" pattern

```C++
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <string>
#include <Windows.h>
using std::find;
using std::vector;
using std::string;
using std::reverse;

class Pos {
public:
	float x;
	float y;
	int code;
	Pos() {	}
	Pos(float a, float b, int c) {
		x = a;
		y = b;
		code = c;
	}
};

static vector<vector<Pos>> polylineGroup;
static int groupNum = 0;
static int windowNumber;
static int counter = 0;
static bool isStop = false;

void prebuild() {
	polylineGroup.push_back(vector<Pos>());
	polylineGroup[0].push_back(Pos(300.0f, 150.0f, 1));
	polylineGroup[0].push_back(Pos(400.0f, 250.0f, 1));
	polylineGroup[0].push_back(Pos(350.0f, 250.0f, 1));
	polylineGroup[0].push_back(Pos(450.0f, 350.0f, 1));
	polylineGroup[0].push_back(Pos(350.0f, 350.0f, 1));
	polylineGroup[0].push_back(Pos(350.0f, 400.0f, 1));
	polylineGroup[0].push_back(Pos(250.0f, 400.0f, 1));
	polylineGroup[0].push_back(Pos(250.0f, 350.0f, 1));
	polylineGroup[0].push_back(Pos(150.0f, 350.0f, 1));
	polylineGroup[0].push_back(Pos(250.0f, 250.0f, 1));
	polylineGroup[0].push_back(Pos(200.0f, 250.0f, 1));
	polylineGroup[0].push_back(Pos(300.0f, 150.0f, 1));
	polylineGroup.push_back(vector<Pos>());
	polylineGroup[1].push_back(Pos(250.0f, 150.0f, 1));
	polylineGroup[1].push_back(Pos(400.0f, 150.0f, 1));
	polylineGroup[1].push_back(Pos(400.0f, 200.0f, 1));
	polylineGroup[1].push_back(Pos(300.0f, 200.0f, 1));
	polylineGroup[1].push_back(Pos(300.0f, 250.0f, 1));
	polylineGroup[1].push_back(Pos(350.0f, 250.0f, 1));
	polylineGroup[1].push_back(Pos(350.0f, 300.0f, 1));
	polylineGroup[1].push_back(Pos(300.0f, 300.0f, 1));
	polylineGroup[1].push_back(Pos(300.0f, 450.0f, 1));
	polylineGroup[1].push_back(Pos(250.0f, 450.0f, 1));
	polylineGroup[1].push_back(Pos(250.0f, 150.0f, 1));
}

void draw() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j < polylineGroup[1].size(); j++) {
		float x = polylineGroup[1][j].x + polylineGroup[2][j].x * counter;
		float y = polylineGroup[1][j].y + polylineGroup[2][j].y * counter;
		glVertex2f(x / 100.0f - 3.0f, -(y / 100.0f - 3.0f));
	}
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j < polylineGroup[2].size(); j++) {
		float x = polylineGroup[1][j].x;
		float y = polylineGroup[1][j].y;
		glVertex2f(x / 100.0f - 3.0f, -(y / 100.0f - 3.0f));
	}
	glEnd();
	glFlush();
}

void checkPointNum() {
	while (polylineGroup[0].size() != polylineGroup[1].size()) {
		if (polylineGroup[0].size() < polylineGroup[1].size()) {
			Pos tmp = polylineGroup[0][polylineGroup[0].size() - 1];
			polylineGroup[0].push_back(tmp);
		}
		else if (polylineGroup[0].size() > polylineGroup[1].size()) {
			Pos tmp = polylineGroup[1][polylineGroup[1].size() - 1];
			polylineGroup[1].push_back(tmp);
		}
	}
}

void calculateDelta() {
	polylineGroup.push_back(vector<Pos>());
	for (int i = 0; i < polylineGroup[0].size(); i++) {
		polylineGroup[2].push_back(Pos((polylineGroup[0][i].x - polylineGroup[1][i].x) / counter, (polylineGroup[0][i].y - polylineGroup[1][i].y) / counter, 0));
	}
}

void refreshFunc() {
	prebuild();
	checkPointNum();
	calculateDelta();
	groupNum = 0;
	draw();
}

void CALLBACK transform(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTimer) {
	if (!isStop) {
		counter--;
	}
	draw();
	if (counter == 0) {
		KillTimer(NULL, iTimerID);
	}
}

void stopFunc() {
	if (!isStop) {
		isStop = true;
	}
	else {
		isStop = false;
	}
}

void keyPress(unsigned char key, int x, int y) {
	if (key == 32) {
		stopFunc();
		printf("GET");
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	windowNumber = glutCreateWindow("OpenGL Lab 4");
	gluOrtho2D(-3.0f, 3.0f, -3.0f, 3.0f);

	scanf_s("%d", &counter);
	refreshFunc();
	SetTimer(NULL, 1, 100, transform);

	glutKeyboardFunc(&keyPress);
	glutMainLoop();
	return 0;
}
```