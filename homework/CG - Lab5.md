## Lab 5 - Cyrus-Beck Clipping

Using Cyrus-Beck clipping to clip random lines.
The polygon will be created in blue lines.
The part inside the polygon will be marked in red color.
The part outside the polygon will be marked in green color.

Press button 'b' and use mouse clicking to create a convex polygon.
Press button 'c' to create ten lines randomly.
Press button 'r' to clear the canvas.

```cpp
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <algorithm>
using std::vector;
using std::reverse;

static bool isBegin = false;
static int windowNumber;

const float pi = 3.1415926;

enum pointType {
	NORMALPOINT, INPOINT, OUTPOINT
};

class Pos {
public:
	float x;
	float y;
	int i;
	int type;
	Pos(float a, float b, int c) {
		x = a;
		y = b;
		i = c;
		type = NORMALPOINT;
	}
};

bool judgeCross(Pos pointA1, Pos pointA2, Pos pointB1, Pos pointB2) {
	Pos V1(pointB1.x - pointA1.x, pointB1.y - pointA1.y, 0);
	Pos V2(pointB2.x - pointA1.x, pointB2.y - pointA1.y, 0);
	Pos V3(pointA2.x - pointA1.x, pointA2.y - pointA1.y, 0);

	Pos V4(pointA1.x - pointB1.x, pointA1.y - pointB1.y, 0);
	Pos V5(pointA2.x - pointB1.x, pointA2.y - pointB1.y, 0);
	Pos V6(pointB2.x - pointB1.x, pointB2.y - pointB1.y, 0);

	if ((V1.x*V3.y - V1.y*V3.x)*(V2.x*V3.y - V2.y*V3.x) < 0 && (V4.x*V6.y - V4.y*V6.x)*(V5.x*V6.y - V5.y*V6.x) < 0) return true;
	else return false;
}

static vector<vector<Pos>> polylineGroup;
static vector<vector<Pos>> segmentGroup;

bool compare(Pos &a, Pos &b) {
	return a.x < b.x;
}

void getCrossPoint(vector<Pos> &crossPoints, Pos &start, Pos &end) {
	crossPoints.push_back(start);
	for (int i = 0; i < polylineGroup.size(); i++) {
		for (int j = 0; j < polylineGroup[i].size() - 1; j++) {
			Pos pointA1 = start;
			Pos pointA2 = end;
			Pos pointB1 = polylineGroup[i][j];
			Pos pointB2 = polylineGroup[i][j + 1];
			if (judgeCross(pointA1, pointA2, pointB1, pointB2)) {
				Pos V1(pointB1.x - pointA1.x, pointB1.y - pointA1.y, 0);
				Pos V2(pointB2.x - pointA1.x, pointB2.y - pointA1.y, 0);
				Pos V3(pointA2.x - pointA1.x, pointA2.y - pointA1.y, 0);
				Pos V4(pointB2.x - pointB1.x, pointB2.y - pointB1.y, 0);

				float t = abs(V3.x*V1.y - V3.y*V1.x) / (abs(V3.x*V1.y - V3.y*V1.x) + abs(V3.x*V2.y - V3.y*V2.x));

				Pos CrossPoint(pointB1.x + V4.x * t, pointB1.y + V4.y * t, 1);
				
				if ((V3.x*(-V4.y) + V3.y*V4.x) > 0) {
					CrossPoint.type = OUTPOINT;
				}
				else {
					CrossPoint.type = INPOINT;
				}
				
				crossPoints.push_back(CrossPoint);
			} 
		}
	}
	crossPoints.push_back(end);
	sort(crossPoints.begin(), crossPoints.end(), compare);
	if (crossPoints[0].x != start.x || crossPoints[0].y != start.y) {
		reverse(crossPoints.begin(), crossPoints.end());
	}
}

bool judgeIfInside(Pos &start, Pos &end) {
	vector<Pos> points;
	points.push_back(start);
	points.push_back(end);

	for (int i = 0; i < polylineGroup[0].size() - 1; i++) {
		float a1 = end.y - start.y;
		float b1 = start.x - end.x;
		float c1 = start.x*end.y - start.y*end.x;
		float a2 = polylineGroup[0][i + 1].y - polylineGroup[0][i].y;
		float b2 = polylineGroup[0][i].x - polylineGroup[0][i + 1].x;
		float c2 = polylineGroup[0][i].x*polylineGroup[0][i + 1].y - polylineGroup[0][i].y*polylineGroup[0][i + 1].x;
		float det = a1 * b2 - a2 * b1;

		if (det == 0) continue;
		Pos CrossPoint((c1*b2 - c2*b1) / det, (c2*a1 - c1*a2) / det, 1);
		if ((polylineGroup[0][i].x < CrossPoint.x && polylineGroup[0][i + 1].x > CrossPoint.x) || (polylineGroup[0][i].x > CrossPoint.x && polylineGroup[0][i + 1].x < CrossPoint.x)) {
			points.push_back(CrossPoint);
		}
	}
	sort(points.begin(), points.end(), compare);
	if ((points[0].x != start.x && points[0].x != end.x) && (points[points.size() - 1].x != start.x && points[points.size() - 1].x != end.x)) return true;
	else return false;
}

void drawPoly() {
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < polylineGroup.size(); i++) {
		for (int j = 0; j < polylineGroup[i].size(); j++) {
			float x = polylineGroup[i][j].x;
			float y = polylineGroup[i][j].y;
			if (x == -100 && y == -100) {
				x = polylineGroup[i][0].x;
				y = polylineGroup[i][0].y;
			}
			glVertex2f(x / 100.0f - 3.0f, -(y / 100.0f - 3.0f));
		}
		glEnd();
	}
}

void drawSegment() {
	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < segmentGroup.size(); i++) {
		vector<Pos> crossPoints;
		getCrossPoint(crossPoints, segmentGroup[i][0], segmentGroup[i][1]);
		if (crossPoints[1].type == OUTPOINT) {
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else if ((crossPoints.size() == 2 && judgeIfInside(segmentGroup[i][0], segmentGroup[i][1]))) {
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		else {
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		for (int j = 0; j < crossPoints.size() - 1; j++) {
			glBegin(GL_LINES);
			glVertex2f(crossPoints[j].x / 100.0f - 3.0f, -(crossPoints[j].y / 100.0f - 3.0f));
			glVertex2f(crossPoints[j + 1].x / 100.0f - 3.0f, -(crossPoints[j + 1].y / 100.0f - 3.0f));
			if (crossPoints[j + 1].type == INPOINT) {
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else if (crossPoints[j + 1].type == OUTPOINT) {
				glColor3f(0.0f, 1.0f, 0.0f);
			}
		}
		glEnd();
	}
}

void draw() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);

	//draw a line which is out of the window to make sure the flush will be executed.
	glBegin(GL_LINES);
	glVertex2f(-100, -100);
	glVertex2f(-100, -101);
	glEnd();

	//actual line which we want to be displayed.
	drawPoly();
	drawSegment();

	glFlush();
}

void randomLineFunc() {
	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		Pos tmpPos1(rand() % 600, rand() % 600, 1);
		Pos tmpPos2(rand() % 600, rand() % 600, 1);
		segmentGroup.push_back(vector<Pos>());
		segmentGroup[segmentGroup.size() - 1].push_back(tmpPos1);
		segmentGroup[segmentGroup.size() - 1].push_back(tmpPos2);
	}
	draw();
}

void beginFunc() {
	if (isBegin == false) {
		isBegin = true;
		polylineGroup.push_back(vector<Pos>());
	}
	else {
		isBegin = false;
	}
}

void refreshFunc() {
	isBegin = false;
	polylineGroup.clear();
	segmentGroup.clear();
	draw();
}

void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
		return;
	}
	if (state != GLUT_UP) {
		if (isBegin) {
			Pos nowpos(x, y, 1);
			float dis = 10000.0f;
			if (polylineGroup[polylineGroup.size() - 1].size() != 0) {
				dis = pow(polylineGroup[polylineGroup.size() - 1][0].x - x, 2) + pow(polylineGroup[polylineGroup.size() - 1][0].y - y, 2);
			}
			if (dis <= 25) {
				nowpos.x = polylineGroup[polylineGroup.size() - 1][0].x;
				nowpos.y = polylineGroup[polylineGroup.size() - 1][0].y;
				isBegin = false;
			}
			polylineGroup[polylineGroup.size() - 1].push_back(nowpos);
			draw();
		}
	}
}

void mouseMove(int x, int y) {
	draw();
	if (isBegin && polylineGroup[polylineGroup.size() - 1].size() != 0) {
		Pos last = polylineGroup[polylineGroup.size() - 1][polylineGroup[polylineGroup.size() - 1].size() - 1];
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
		glVertex2f(last.x / 100.0f - 3.0f, -(last.y / 100.0f - 3.0f));
		glVertex2f(x / 100.0f - 3.0f, -(y / 100.0f - 3.0f));
		glEnd();
		glFlush();
	}
}

void keypress(unsigned char key, int x, int y) {
	if (key == 'b') {
		//create a new polyline.
		beginFunc();
	}
	else if (key == 'r') {
		//erase the screen and redraw all the polylines.
		refreshFunc();
	}
	else if (key == 'c') {
		randomLineFunc();
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	windowNumber = glutCreateWindow("OpenGL Lab 5");
	gluOrtho2D(-3.0f, 3.0f, -3.0f, 3.0f);

	refreshFunc();

	glutMouseFunc(&mouseClick);
	glutPassiveMotionFunc(&mouseMove);
	glutKeyboardFunc(&keypress);

	glutMainLoop();
	return 0;
}
```