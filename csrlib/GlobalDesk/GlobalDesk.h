#pragma once


/************************************************************************/
/* 
	数学基础-
	-球面参数方程
	-   x = cx + r*sin(πv)*cos(2πu)
	-   y = cy + r*sin(πv)*sin(2πu)
	-   z = cz + r*con(πv)

	球面法向量-
	对u和v求偏导，然后对两个所得向量进行X积运算

	-	A(sin(πv)*cos(2πu))
		 (sin(πv)*sin(2πu))
		      cos(πv)
		A = -2π²r²*sin(πv)
*/
/************************************************************************/

#include <stdio.h>
#include <math.h>
#include <gl/glut.h>

#define PI 3.1415926
GLsizei width = 600;
GLsizei height = 600;
int uStepNum = 50;
int vStepNum = 50;

//点结构
class CsrPoint
{
public:
	CsrPoint() {};
	CsrPoint(double a, double b, double c) :x(a), y(b), z(c) {};

public:
	double x;
	double y;
	double z;
};

//坐标系数（u,v）转换成坐标函数（x,y,z）

CsrPoint getPoint(double u, double v)
{
	double x = sin(PI*v)*cos(PI * 2 * u);
	double y = sin(PI*v)*sin(PI * 2 * u);
	double z = cos(PI*v);
	return CsrPoint(x, y, z);
}

void drawWire()
{
	double ustep = 1 / (double)uStepNum;
	double vstep = 1 / (double)vStepNum;
	double u = 0;
	double v = 0;

	//绘制下端三角形组
	for (int i = 0; i < uStepNum; ++i)
	{
		glBegin(GL_LINE_LOOP);
		CsrPoint a = getPoint(0, 0);
		glVertex3d(a.x, a.y, a.z);
		CsrPoint b = getPoint(u, vstep);
		glVertex3d(b.x, b.y, b.z);
		CsrPoint c = getPoint(u + ustep, vstep);
		glVertex3d(c.x, c.y, c.z);
		u += ustep;
		glEnd();
	}

	//绘制中间四边形组
	u = 0; 
	v = vstep;
	for (int i = 1; i < vStepNum - 1; ++i)
	{
		for (int j = 0; j < uStepNum; ++j)
		{
			glBegin(GL_LINE_LOOP);
			CsrPoint a = getPoint(u, v);
			CsrPoint b = getPoint(u + ustep, v);
			CsrPoint c = getPoint(u + ustep, v + vstep);
			CsrPoint d = getPoint(u, v + vstep);
			glVertex3d(a.x, a.y, a.z);
			glVertex3d(b.x, b.y, b.z);
			glVertex3d(c.x, c.y, c.z);
			glVertex3d(d.x, d.y, d.z);
			u += ustep;
			glEnd();
		}
		v += vstep;
	}

	//绘制下端三角形组
	u = 0;
	for (int i = 0; i < uStepNum; ++i)
	{
		glBegin(GL_LINE_LOOP);
		CsrPoint a = getPoint(0, 1);
		CsrPoint b = getPoint(u, 1 - vstep);
		CsrPoint c = getPoint(u + ustep, 1 - vstep);
		glVertex3d(a.x, b.x, c.x);
		glVertex3d(b.x, b.y, b.z);
		glVertex3d(c.x, c.y, c.z);
		glEnd();
	}
}

void initgl()
{
	glClearColor(0, 1, 1, 1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f };
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 0.2f };
	GLfloat specular[] = { 0.5f, 0.5f, 0.5f, 0.2f };

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void displayFunc()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(1.0);
	glRotated(30, 1, 0, 0);
	glRotated(60, 0, 1, 0);
	glRotated(90, 0, 0, 1);
	drawWire();
	glutSwapBuffers();
}
