## Lab 3 - Cohen-Sutherland clipper in C++

Draw a box with black lines. Then create some lines randomly in the window, some of the lines might pass through the box border and get inside of the box. The inside-part of the line need to be marked with red color.

```C++
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <string>
using std::find;
using std::vector;
using std::string;
using std::reverse;

static int windowNumber;

class Pos {
public:
	float x;
	float y;
	int code;
	Pos() {	}
	Pos(float a, float b) {
		x = a;
		y = b;
	}
	void resetCode() {
		if (x >= 0 && x < 200 && y >= 0 && y < 200) {
			code = 5;
		}
		else if (x >= 200 && x <= 400 && y >= 0 && y < 200) {
			code = 4;
		}
		else if (x > 400 && x <= 600 && y >= 0 && y < 200) {
			code = 6;
		}
		else if (x >= 0 && x < 200 && y >= 200 && y <= 400) {
			code = 1;
		}
		else if (x >= 200 && x <= 400 && y >= 200 && y <= 400) {
			code = 0;
		}
		else if (x > 400 && x <= 600 && y >= 200 && y <= 400) {
			code = 2;
		}
		else if (x >= 0 && x < 200 && y > 400 && y <= 600) {
			code = 9;
		}
		else if (x >= 200 && x <= 400 && y > 400 && y <= 600) {
			code = 8;
		}
		else if (x > 400 && x <= 600 && y > 400 && y <= 600) {
			code = 10;
		}
	}
};

static vector<vector<Pos>> polylineGroup;
static int groupNum = 0;

void prebuild() {
	polylineGroup.push_back(vector<Pos>());
	polylineGroup[0].push_back(Pos(200.0f, 200.0f));
	polylineGroup[0].push_back(Pos(200.0f, 400.0f));
	polylineGroup[0].push_back(Pos(400.0f, 400.0f));
	polylineGroup[0].push_back(Pos(400.0f, 200.0f));
	polylineGroup[0].push_back(Pos(-100.0f, -100.0f));
}

void drawStructure() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j < polylineGroup[0].size(); j++) {
		float x = polylineGroup[0][j].x;
		float y = polylineGroup[0][j].y;
		if (x == -100 && y == -100) {
			x = polylineGroup[0][0].x;
			y = polylineGroup[0][0].y;
		}
		glVertex2f(x / 100.0f - 3.0f, -(y / 100.0f - 3.0f));
	}
	glEnd();
}

int checkIfCross(Pos &a, Pos &b) {
	if (a.code == 0 && b.code == 0) {
		return 0;
	}
	else if ((a.code & b.code) != 0) {
		return 1;
	}
	else {
		return 2;
	}
}

bool cmp(Pos &a, Pos &b) {
	return a.x < b.x;
}

bool checkCrossNum(Pos &a, Pos &b) {
	printf("%d,%d,%d\n", a.code ^ b.code, a.code, b.code);
	if (a.code == 0 || b.code == 0) {
		return false;
	}
	return true;
}

void drawLines() {
	for (int i = 1; i < polylineGroup.size(); i++) {
		Pos startPos = polylineGroup[i][0];
		Pos endPos = polylineGroup[i][1];
		if (checkIfCross(startPos, endPos) == 0) {
			glBegin(GL_LINE_STRIP);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(startPos.x / 100.0f - 3.0f, -(startPos.y / 100.0f - 3.0f));
			glVertex2f(endPos.x / 100.0f - 3.0f, -(endPos.y / 100.0f - 3.0f));
			glEnd();
		}
		else if (checkIfCross(startPos, endPos) == 1) {
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(startPos.x / 100.0f - 3.0f, -(startPos.y / 100.0f - 3.0f));
			glVertex2f(endPos.x / 100.0f - 3.0f, -(endPos.y / 100.0f - 3.0f));
			glEnd();
		}
		else if (checkIfCross(startPos, endPos) == 2) {
			glBegin(GL_LINES);
			float y1 = (startPos.y - endPos.y) / (startPos.x - endPos.x) * (200 - endPos.x) + endPos.y;
			float y2 = (startPos.y - endPos.y) / (startPos.x - endPos.x) * (400 - endPos.x) + endPos.y;
			float x1 = (startPos.x - endPos.x) / (startPos.y - endPos.y) * (200 - endPos.y) + endPos.x;
			float x2 = (startPos.x - endPos.x) / (startPos.y - endPos.y) * (400 - endPos.y) + endPos.x;
			printf("%f,%f,%f,%f\n", x1, x2, y1, y2);
			vector<Pos> tmpSeq;
			tmpSeq.push_back(startPos);
			tmpSeq.push_back(endPos);
			if (y1 >= 200 && y1 <= 400) {
				tmpSeq.push_back(Pos(200, y1));
			}
			if (y2 >= 200 && y2 <= 400) {
				tmpSeq.push_back(Pos(400, y2));
			}
			if (x1 >= 200 && x1 <= 400) {
				tmpSeq.push_back(Pos(x1, 200));
			}
			if (x2 >= 200 && x2 <= 400) {
				tmpSeq.push_back(Pos(x2, 400));
			}
			sort(tmpSeq.begin(), tmpSeq.end(), cmp);

			if (checkCrossNum(startPos, endPos)) {
				if (tmpSeq.size() == 4) {
					glColor3f(0.0f, 0.0f, 0.0f);
					glVertex2f(tmpSeq[0].x / 100.0f - 3.0f, -(tmpSeq[0].y / 100.0f - 3.0f));
					glVertex2f(tmpSeq[1].x / 100.0f - 3.0f, -(tmpSeq[1].y / 100.0f - 3.0f));
					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex2f(tmpSeq[1].x / 100.0f - 3.0f, -(tmpSeq[1].y / 100.0f - 3.0f));
					glVertex2f(tmpSeq[2].x / 100.0f - 3.0f, -(tmpSeq[2].y / 100.0f - 3.0f));
					glColor3f(0.0f, 0.0f, 0.0f);
					glVertex2f(tmpSeq[2].x / 100.0f - 3.0f, -(tmpSeq[2].y / 100.0f - 3.0f));
					glVertex2f(tmpSeq[3].x / 100.0f - 3.0f, -(tmpSeq[3].y / 100.0f - 3.0f));
				}
				glEnd();
			}
			else {
				printf("get!\n");
				if (tmpSeq[3].x != startPos.x && tmpSeq[3].x != endPos.x) {
					tmpSeq.erase(tmpSeq.end() - 1);
				}
				if (tmpSeq[0].x != startPos.x && tmpSeq[0].x != endPos.x) {
					tmpSeq.erase(tmpSeq.begin());
				}
				if (tmpSeq[tmpSeq.size() - 1].code < tmpSeq[0].code) {
					reverse(tmpSeq.begin(), tmpSeq.end());
				}

				if (tmpSeq.size() == 3) {
					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex2f(tmpSeq[0].x / 100.0f - 3.0f, -(tmpSeq[0].y / 100.0f - 3.0f));
					glVertex2f(tmpSeq[1].x / 100.0f - 3.0f, -(tmpSeq[1].y / 100.0f - 3.0f));
					glColor3f(0.0f, 0.0f, 0.0f);
					glVertex2f(tmpSeq[1].x / 100.0f - 3.0f, -(tmpSeq[1].y / 100.0f - 3.0f));
					glVertex2f(tmpSeq[2].x / 100.0f - 3.0f, -(tmpSeq[2].y / 100.0f - 3.0f));
				}
				glEnd();
			}
		}
	}
}

void draw() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	drawStructure();
	drawLines();
	glFlush();
}

void randomLine() {
	srand(time(NULL));
	for (int i = 0; i < 30; i++) {
		Pos tmpPos1;
		tmpPos1.x = rand() % 600;
		tmpPos1.y = rand() % 600;
		tmpPos1.resetCode();

		Pos tmpPos2;
		tmpPos2.x = rand() % 600;
		tmpPos2.y = rand() % 600;
		tmpPos2.resetCode();

		polylineGroup.push_back(vector<Pos>());
		groupNum++;
		polylineGroup[groupNum].push_back(tmpPos1);
		polylineGroup[groupNum].push_back(tmpPos2);
	}
}

void refreshFunc() {
	polylineGroup.clear();
	prebuild();
	groupNum = 0;

	randomLine();

	draw();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	windowNumber = glutCreateWindow("OpenGL Lab 3");
	gluOrtho2D(-3.0f, 3.0f, -3.0f, 3.0f);
	
	refreshFunc();

	glutMainLoop();
	return 0;
}
```
