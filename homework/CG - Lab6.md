## Lab 6 - 3D model display

Polygonalize a sphere with some control arguments to specify the resolution. The result should be written to a file which can also be read for mesh display.

The main cplusplus code and the content in `sphere.txt` file are given below. Please put `sphere.txt` and `main.cpp` in the same directory.

```cpp
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <fstream>
using std::vector;
using std::ifstream;

static int windowNumber;
const float pi = 3.1415926;

class Pos3d {
public:
	float x;
	float y;
	float z;
	int i;
	Pos3d(float a, float b, float c, int d) {
		x = a;
		y = b;
		z = c;
		i = d;
	}
};

void drawMesh(vector<vector<int>> &input, vector<Pos3d> &points) {
	for (int i = 0; i < input.size(); i++) {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < input[i].size(); j++) {
			Pos3d tmp = points[input[i][j]];
			glVertex3f(points[input[i][j]].x, points[input[i][j]].y, points[input[i][j]].z);
		}
		glEnd();
	}
}

void draw(vector<vector<int>> &input, vector<Pos3d> &points) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);

	drawMesh(input, points);

	glFlush();
}

void readFile(vector<vector<int>> &input, vector<Pos3d> &points, ifstream *inputFile) {
	if (inputFile->is_open()) {
		int verticesNum, normalsNum, facesNum;
		*inputFile >> verticesNum >> normalsNum >> facesNum;
		//vertices num
		for (int i = 0; i < verticesNum; i++) {
			float tmpx, tmpy, tmpz;
			*inputFile >> tmpx >> tmpy >> tmpz;
			points.push_back(Pos3d(tmpx, tmpy, tmpz, 1));
		}
		//normals num
		for (int i = 0; i < normalsNum; i++) {
			float tmpx, tmpy, tmpz;
			*inputFile >> tmpx >> tmpy >> tmpz;
			//can be stored and be used in loading material.
		}
		for (int i = 0; i < facesNum; i++) {
			int pointsNum;
			*inputFile >> pointsNum;
			input.push_back(vector<int>());
			//vertices code
			for (int j = 0; j < pointsNum; j++) {
				int codeNum;
				*inputFile >> codeNum;
				input[input.size() - 1].push_back(codeNum);
			}
			//normals code
			for (int j = 0; j < pointsNum; j++) {
				int codeNum;
				*inputFile >> codeNum;
				//can be stored and be used in loading material.
			}
		}
		printf("ok!");
	}
	else {
		printf("Error in opening file.");
		return;
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glMatrixMode(GL_PROJECTION);
	windowNumber = glutCreateWindow("OpenGL Lab 9");
	glLoadIdentity();
	glOrtho(-3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f);

	vector<vector<int>> input;
	vector<Pos3d> points;
	ifstream file("./sphere.txt");
	readFile(input, points, &file);
	draw(input, points);

	glutMainLoop();
	return 0;
}
```

```txt
62 1 72
0 1 0  
0 0.866 0.5  0.25 0.866 0.433  0.433 0.866 0.25  0.5 0.866 0  0.433 0.866 -0.25  0.25 0.866 -0.433  
0 0.866 -0.5  -0.25 0.866 -0.433  -0.433 0.866 -0.25  -0.5 0.866 0  -0.433 0.866 0.25 -0.25 0.866 0.433
0 0.5 0.866  0.433 0.5 0.75  0.75 0.5 0.433  0.866 0.5 0  0.75 0.5 -0.433  0.433 0.5 -0.75  
0 0.5 -0.866  -0.433 0.5 -0.75  -0.75 0.5 -0.433  -0.866 0.5 0  -0.75 0.5 0.433  -0.433 0.5 0.75
0 0 1  0.5 0 0.866  0.866 0 0.5  1 0 0  0.866 0 -0.5  0.5 0 -0.866  
0 0 -1  -0.5 0 -0.866  -0.866 0 -0.5  -1 0 0 -0.866 0 0.5  -0.5 0 0.866
0 -0.5 0.866  0.433 -0.5 0.75  0.75 -0.5 0.433  0.866 -0.5 0  0.75 -0.5 -0.433  0.433 -0.5 -0.75  
0 -0.5 -0.866  -0.433 -0.5 -0.75  -0.75 -0.5 -0.433  -0.866 -0.5 0  -0.75 -0.5 0.433  -0.433 -0.5 0.75
0 -0.866 0.5  0.25 -0.866 0.433  0.433 -0.866 0.25  0.5 -0.866 0  0.433 -0.866 -0.25  0.25 -0.866 -0.433  
0 -0.866 -0.5  -0.25 -0.866 -0.433  -0.433 -0.866 -0.25  -0.5 -0.866 0  -0.433 -0.866 0.25 -0.25 -0.866 0.433
0 -1 0
0 0 1
3  0 1 2  0 0 0
3  0 2 3  0 0 0
3  0 3 4  0 0 0
3  0 4 5  0 0 0
3  0 5 6  0 0 0
3  0 6 7  0 0 0
3  0 7 8  0 0 0
3  0 8 9  0 0 0
3  0 9 10  0 0 0
3  0 10 11  0 0 0
3  0 11 12  0 0 0
3  0 12 1  0 0 0

4  1 2 14 13  0 0 0 0
4  2 3 15 14  0 0 0 0
4  3 4 16 15  0 0 0 0
4  4 5 17 16  0 0 0 0
4  5 6 18 17  0 0 0 0
4  6 7 19 18  0 0 0 0
4  7 8 20 19  0 0 0 0
4  8 9 21 20  0 0 0 0
4  9 10 22 21  0 0 0 0
4  10 11 23 22  0 0 0 0
4  11 12 24 23  0 0 0 0
4  12 1 13 24  0 0 0 0

4  13 14 26 25  0 0 0 0
4  14 15 27 26  0 0 0 0 
4  15 16 28 27  0 0 0 0
4  16 17 29 28  0 0 0 0
4  17 18 30 29  0 0 0 0
4  18 19 31 30  0 0 0 0
4  19 20 32 31  0 0 0 0 
4  20 21 33 32  0 0 0 0
4  21 22 34 33  0 0 0 0
4  22 23 35 34  0 0 0 0
4  23 24 36 35  0 0 0 0
4  24 13 25 36  0 0 0 0

4  25 26 38 37  0 0 0 0
4  26 27 39 38  0 0 0 0
4  27 28 40 39  0 0 0 0
4  28 29 41 40  0 0 0 0
4  29 30 42 41  0 0 0 0
4  30 31 43 42  0 0 0 0
4  31 32 44 43  0 0 0 0
4  32 33 45 44  0 0 0 0
4  33 34 46 45  0 0 0 0
4  34 35 47 46  0 0 0 0
4  35 36 48 47  0 0 0 0
4  36 25 37 48  0 0 0 0

4  37 38 50 49  0 0 0 0
4  38 39 51 50  0 0 0 0
4  39 40 52 51  0 0 0 0
4  40 41 53 52  0 0 0 0
4  41 42 54 53  0 0 0 0
4  42 43 55 54  0 0 0 0
4  43 44 56 55  0 0 0 0
4  44 45 57 56  0 0 0 0
4  45 46 58 57  0 0 0 0
4  46 47 59 58  0 0 0 0
4  47 48 60 59  0 0 0 0
4  48 37 49 60  0 0 0 0

3  49 50 61  0 0 0
3  50 51 61  0 0 0
3  51 52 61  0 0 0
3  52 53 61  0 0 0 
3  53 54 61  0 0 0
3  54 55 61  0 0 0
3  55 56 61  0 0 0
3  56 57 61  0 0 0
3  57 58 61  0 0 0
3  58 59 61  0 0 0
3  59 60 61  0 0 0
3  60 49 61  0 0 0
```