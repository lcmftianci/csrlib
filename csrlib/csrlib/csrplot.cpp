#include "csrplot.h"

GLfloat ctrlpoints[4][3] = {
	{ -4, -4, 0 },
	{ -2, 4, 0 },
	{ 2, -4, 0 },
	{ 4, 4, 0 }
};

void initGLLib(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);	//背景色
	/*
	控制点映射到曲线上
	参数1：GL_MAP1_VERTEX_3, 3维点坐标
	参数2，3：控制参数t或者u的取值范围[0,1]
	参数4：曲线内插值点间的步长3--三维坐标
	参数5：曲线间的补偿为定点数4个---总步长12
	参数6：控制点二维数组首元素地址
	*/
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glShadeModel(GL_FLAT);

	//代码开关2：去掉本注释，可启用反走样
	/*
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);  //允许直线反走样
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);  // Antialias the lines
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/
}

void displayPlot(void)
{
		int i = 0;
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);

		//代码开关1：去掉本注释，查看动态的曲线绘图效果：动态更新控制点坐标
		/*
		for(int t = 0; t < 4; t++) {
		for(int j = 0; j < 3; j++)
		ctrlpoints[t][j] = (rand() % 1024 / 1024.0 - 0.5) * 10;
		}
		//动态映射
		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
		*/

		glLoadIdentity();
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINE_STRIP);		//绘制连续线段

									//参数t或u取值为i/30,共计31个点
		for (i = 0; i <= 30; ++i)
			glEvalCoord1f((GLfloat)i / 30.0);		//根据四个控制点坐标参数化插值

		glEnd();

		//显示控制点
		glPointSize(5.0);
		glBegin(GL_POINTS);
		for (i = 0; i < 4; i++)
			glVertex3fv(&ctrlpoints[i][0]);
		glEnd();

		glTranslatef(-0.1f, 0.1f, 0.0f);
		glColor3f(0.0, 1.0, 0.0);

		glBegin(GL_LINE_STRIP);					//绘制连续线段越多，曲线越光滑
		glBegin(GL_LINE_STRIP);
		//设置参数t或u取值为i/60，共计61个点
		//实验：若让t从-2变化到+2，可看到什么效果
		for (i = 0; i <= 60; i++)
			glEvalCoord1f((GLfloat)i / 60.0);  //根据4个控制点坐标的参数化插值
		glEnd();

		glTranslatef(-0.1f, 0.1f, 0.0f);
		glColor3f(1.0, 1.0, 1.0);
		//绘制连续线段
		glBegin(GL_LINE_STRIP);
		//设置参数t或u取值为i/60，共计61个点
		//实验：若让t从-2变化到+2，可看到什么效果
		for (i = 0; i <= 100; i++)
			glEvalCoord1f((GLfloat)i / 100.0);
		glEnd();

		glutSwapBuffers();
}

void drawLine(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0xAAAA);
	glLineWidth(5.0);
	glBegin(GL_LINE_LOOP);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.25, 0.25, 0);
	glVertex3f(0.75, 0.25, 0);
	glVertex3f(0.75, 0.75, 0);
	glVertex3f(0.25, 0.75, 0);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	glFlush();
}

void drawLineNew(void)
{
	//清除颜色缓冲区，即窗口的背景色
	glClear(GL_COLOR_BUFFER_BIT);

	//设置当前绘图使用的颜色
	glColor3f(0.0f, 0.0f, 0.0f);

	//使用当前的颜色来绘制一个矩形
	//    glRectf(100.0f, 150.0f, 150.0f, 100.0f);

	//   1.画点
	GLfloat sizesPoint[2];//保存线宽的尺寸范围
	GLfloat stepPoint;//保存线宽度尺寸的最小间隔
	GLfloat curSizePoint;

	glGetFloatv(GL_POINT_SIZE_RANGE, sizesPoint);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &stepPoint);
	curSizePoint = sizesPoint[0];
	for (int i = 0;i < 25;i++) {
		glPointSize(curSizePoint);
		glBegin(GL_POINTS);
		glVertex3f(25.0 + i * 8, 200.0f, 0.0f);
		glEnd();
		curSizePoint += stepPoint * 2;
	}


	//  2. 画直线

	//    GLfloat sizesLine[2];//保存线宽的尺寸范围
	//    GLfloat stepLine;//保存线宽度尺寸的最小间隔
	GLfloat curSizeLine = 5;
	//    glGetFloatv(GL_LINE_WIDTH_RANGE,sizesLine);
	//    glGetFloatv(GL_LINE_WIDTH_GRANULARITY,&stepLine);
	glLineWidth(curSizeLine);
	glBegin(GL_LINES);
	glVertex3f(25.0f, 160.0f, 0.0f);
	glVertex3f(225.0f, 160.0f, 0.0f);

	glEnd();

	//   3.画连续的直线
	glBegin(GL_LINE_STRIP);
	glVertex3f(25.0f, 150.0f, 0.0f);
	glVertex3f(100.0f, 150.0f, 0.0f);
	glVertex3f(100.0f, 130.0f, 0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 10.0f, 0.0f);
	glVertex3f(20.0f, 5.0f, 0.0f);
	glEnd();

	//   4.画封闭式直线
	glBegin(GL_LINE_LOOP);
	glVertex3f(125.0f, 150.0f, 0.0f);
	glVertex3f(200.0f, 150.0f, 0.0f);
	glVertex3f(200.0f, 130.0f, 0.0f);
	glEnd();

	//   画虚线
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x00FF);//第一个参数是倍数，第二个参数是一个十六进制数，用来表示如何显示的
	glBegin(GL_LINES);
	glVertex3f(25.0f, 120.0f, 0.0f);
	glVertex3f(225.0f, 120.0f, 0.0f);
	glEnd();


	glLineWidth(3);
	glLineStipple(1, 0xFF0C);
	glBegin(GL_LINES);
	glVertex3f(25.0f, 80.0f, 0.0f);
	glVertex3f(225.0f, 80.0f, 0.0f);
	glEnd();


	glLineStipple(4, 0xFF0C);
	glBegin(GL_LINES);
	glVertex3f(25.0f, 40.0f, 0.0f);
	glVertex3f(225.0f, 40.0f, 0.0f);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	//刷新opengl命令队列
	glFlush();
}

void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-5.0, 5.0, -5.0*(GLfloat)h / (GLfloat)w, 5.0*(GLfloat)h / (GLfloat)w, -5.0, 5.0);
	else
		glOrtho(-5.0*(GLfloat)w / (GLfloat)h, 5.0*(GLfloat)w / (GLfloat)h, -5.0, 5.0, -5.0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
	case 'X':
	case 27:   //ESC键
		exit(0);
		break;
	default:
		break;
	}
}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0) {
		h = 1;
	}
	//设置视区尺寸
	glViewport(0, 0, w, h);
	//重置坐标系统，使用投影变换复位
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h) {
		glOrtho(0.0f, 250.0f, 0.0f, 250.0f*h / w, 1.0f, -1.0f);
	}
	else
	{
		glOrtho(0.0f, 250.0f*w / h, 0.0f, 250.0f, 1.0f, -1.0f);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

