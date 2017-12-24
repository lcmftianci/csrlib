#ifndef _CSR_PLOT_H_
#define _CSR_PLOT_H_

#include "comheader.h"

//绘制折线图


//绘制点阵图


//绘制柱状图


void drawLineNew(void);

//绘制贝塞尔曲线
void initGLLib(void);

void displayPlot(void);

void drawLine(void);

//3D空间中绘制2D效果，采用正交投影
void reshape(GLsizei w, GLsizei h);

void keyboard(unsigned char key, int x, int y);

//改变大小时调用的函数
void ChangeSize(GLsizei w, GLsizei h);

//用于初始化，常用来设置场景的渲染状态
void SetupRC(void);



#endif
