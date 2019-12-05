## Lab 2 - Polyline Editor:

Using a mouse and keyboard to do polyline editing.


(a)	A house in the process of being drawn. The user has just clicked at the position drawn, and a line has been drawn from the previous point to the one designated by the mouse.
(b)	Moving a point. The user positions the cursor near the vertex of some polyline, presses down the mouse button, and “drags” the chosen point to some other location before releasing the button. Upon release of the button, the previous lines connected to this point are erased, and new lines are drawn to it.
(c)	A point is deleted from a polyline. The user clicks near the vertex of some polyline, and the two line segments connected to that vertex are erased. Then the two other endpoints of the segments just erased are connected with a line segment.

The functionality of the program should include the following “actions”:

Begin  (‘b’)  (create a new polyline)
Delete  (‘d’)  (delete the next point pointed to)
Move  (‘m’)  (drag the point pointed to to a new location)
Refresh  (‘r’)  (erase the screen and redraw all the polylines)
Quit   (‘q’)   (exit from the program)


```C++
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
using std::find;
using std::vector;

static bool isBegin = false;
static bool isDelete = false;
static bool isMove = false;
static int chosenPointI = -1;
static int chosenPointJ = -1;

static int windowNumber;

class Pos {
public:
	float x;
	float y;
	Pos() {}
	Pos(float a, float b) {
		x = a;
		y = b;
	}
};

static vector<vector<Pos>> polylineGroup;
static int groupNum = 0;

void prebuild() {
	polylineGroup.push_back(vector<Pos>());
	polylineGroup[0].push_back(Pos(150.0f, 450.0f));
	polylineGroup[0].push_back(Pos(450.0f, 450.0f));
	polylineGroup[0].push_back(Pos(450.0f, 240.0f));
	polylineGroup[0].push_back(Pos(300.0f, 150.0f));
	polylineGroup[0].push_back(Pos(150.0f, 240.0f));
	polylineGroup[0].push_back(Pos(-100.0f, -100.0f));

	polylineGroup.push_back(vector<Pos>());
	polylineGroup[1].push_back(Pos(350.0f, (-(-0.5f * 0.6f + 1.5f) + 3.0f) * 100.0f));
	polylineGroup[1].push_back(Pos(400.0f, (-(-1.0f * 0.6f + 1.5f) + 3.0f) * 100.0f));
	polylineGroup[1].push_back(Pos(400.0f, 100.0f));
	polylineGroup[1].push_back(Pos(350.0f, 100.0f));
	polylineGroup[1].push_back(Pos(-100.0f, -100.0f));

}

void draw() {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < polylineGroup.size(); i++) {
		glBegin(GL_LINE_STRIP);
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
	glFlush();
}

void beginFunc() {
	if (isBegin == false) {
		isBegin = true;
		isDelete = false;
		isMove = false;
		polylineGroup.push_back(vector<Pos>());
	}
	else {
		isBegin = false;
		groupNum += 1;
	}
}

void deleteFunc() {
	if (isDelete == false) {
		isBegin = false;
		isDelete = true;
		isMove = false;
	}
	else {
		isDelete = false;
	}
}

void moveFunc() {
	if (isMove == false) {
		isBegin = false;
		isDelete = false;
		isMove = true;
	}
	else {
		isMove = false;
	}
} 

void refreshFunc() {
	isBegin = false;
	isDelete = false;
	isMove = false;

	polylineGroup.clear();
	prebuild();
	groupNum = 2;

	draw();
}

void quitFunc() {
	isBegin = false;
	isDelete = false;
	isMove = false;
	polylineGroup.clear();
	glutDestroyWindow(windowNumber);
}

void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
		return;
	}
	if (state == GLUT_UP) {
		if (isMove) {
			chosenPointI = -1;
			chosenPointJ = -1;
		}
	}
	else {
		if (isBegin) {
			Pos nowpos = Pos();
			nowpos.x = x;
			nowpos.y = y;
			float dis = 10000.0f;
			if (polylineGroup[groupNum].size() != 0) {
				dis = pow(polylineGroup[groupNum][0].x - x, 2) + pow(polylineGroup[groupNum][0].y - y, 2);
			}
			if (dis <= 25) {
				nowpos.x = -100;
				nowpos.y = -100;
			}
			polylineGroup[groupNum].push_back(nowpos);
			draw();
		}
		else if (isDelete) {
			int minDis = 150000;
			int PosI = -1;
			int PosJ = -1;
			for (int i = 0; i < polylineGroup.size(); i++) {
				for (int j = 0; j < polylineGroup[i].size(); j++) {
					float dis = pow(polylineGroup[i][j].x - x, 2) + pow(polylineGroup[i][j].y - y, 2);
					if (dis < minDis) {
						minDis = dis;
						PosI = i;
						PosJ = j;
					}
				}
			}
			float dis = pow(polylineGroup[PosI][PosJ].x - x, 2) + pow(polylineGroup[PosI][PosJ].y - y, 2);
			if (dis <= 100) {
				polylineGroup[PosI].erase(polylineGroup[PosI].begin() + PosJ);
				draw();
				return;
			}
		}
		else if (isMove) {
			int minDis = 150000;
			int PosI = -1;
			int PosJ = -1;
			for (int i = 0; i < polylineGroup.size(); i++) {
				for (int j = 0; j < polylineGroup[i].size(); j++) {
					float dis = pow(polylineGroup[i][j].x - x, 2) + pow(polylineGroup[i][j].y - y, 2);
					if (dis < minDis) {
						minDis = dis;
						PosI = i;
						PosJ = j;
					}
				}
			}
			float dis = pow(polylineGroup[PosI][PosJ].x - x, 2) + pow(polylineGroup[PosI][PosJ].y - y, 2);
			if (dis <= 100) {
				chosenPointI = PosI;
				chosenPointJ = PosJ;
				return;
			}
		}
	}
}

void mouseMove(int x, int y) {
	draw();
	if (isBegin && polylineGroup[groupNum].size() != 0) {
		Pos last = polylineGroup[polylineGroup.size() - 1][polylineGroup[polylineGroup.size() - 1].size() - 1];
		if (last.x == -100) {
			beginFunc();
			return;
		}
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		glVertex2f(last.x / 100.0f - 3.0f, -(last.y / 100.0f - 3.0f));
		glVertex2f(x / 100.0f - 3.0f, -(y / 100.0f - 3.0f));
		glEnd();
		glFlush();
	}
	
}

void mouseClickMove(int x, int y) {
	if (isMove) {
		for (int i = 0; i < polylineGroup.size(); i++) {
			for (int j = 0; j < polylineGroup[i].size(); j++) {
				float dis = pow(polylineGroup[i][j].x - x, 2) + pow(polylineGroup[i][j].y - y, 2);
				if (dis <= 900 && chosenPointI == i && chosenPointJ == j) {
					polylineGroup[i][j].x = x;
					polylineGroup[i][j].y = y;
					draw();
				}
			}
		}
	}
}

void keypress(unsigned char key, int x, int y) {
	if (key == 'b') {
		//create a new polyline.
		beginFunc();
	}
	else if (key == 'd') {
		//delete the next point pointed to.
		deleteFunc();
	}
	else if (key == 'm') {
		//drag the point pointed to a new location.
		moveFunc();
	}
	else if (key == 'r') {
		//erase the screen and redraw all the polylines.
		refreshFunc();
	}
	else if (key == 'q') {
		//exit from the program.
		quitFunc();
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE );
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	windowNumber = glutCreateWindow("OpenGL Lab 2");
	gluOrtho2D(-3.0f, 3.0f, -3.0f, 3.0f);
	
	refreshFunc();

	glutMouseFunc(&mouseClick);
	glutPassiveMotionFunc(&mouseMove);
	glutMotionFunc(&mouseClickMove);
	glutKeyboardFunc(&keypress);

	glutMainLoop();
	return 0;
}
```