#include "GlobalDesk.h"

extern void drawWire();
extern void initgl();
extern void displayFunc();

//ªÊ÷∆«ÚÃÂ
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Spahere");
	initgl();
	glutDisplayFunc(displayFunc);
	glutMainLoop();
}