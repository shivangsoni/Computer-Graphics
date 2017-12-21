#include "stdio.h"

 
#ifdef WIN32
#include <windows.h>
#endif
#include<iostream>
#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>
#define window_wide 1300
#define window_height 700

float x1,y1,x2,y2,check=0;
void setPixel(int x, int y)
{
glBegin(GL_POINTS);
glVertex2i(x,y);
glEnd();
glFlush();
}

void algorithm()
{
float dx = x2-x1;
float dy = y2-y1;
float m = dy/dx;
if(m>=-1 && m<=1)
{
if(dx<0)
{
int temp = x1;
x1 = x2;
x2 = temp;
temp = y1;
y1 = y2;
y2 = temp;
dx *= -1;
dy *= -1;
}
int x = x1;
int y = y1;
while(x < x2)
{
x++;
y = y+m+0.5;
setPixel(x,y);
}
}
else
{
if(dy<0)
{
int temp = x1;
x1 = x2;
x2 = temp;
temp = y1;
y1 = y2;
y2 = temp;
dx *= -1;
dy *= -1;
}
int x = x1;
int y = y1;
while(y < y2)
{
y++;
x = x+1/m+0.5;
setPixel(x,y);
}
}
}

void myMouse(int button, int state, int x, int y)
{
if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
{
if(check==0)
{
x1 = x;
y1 = window_height-y;
check = 1;
setPixel(x1,y1);
}
else if(check==1)
{
x2 = x;
y2 = window_height-y;
check = 0;
setPixel(x2,y2);
algorithm();
//glFlush();
}
}
}

void myDisplay(void)
{
glClearColor(1.0,1.0,1.0,0.0);
glColor3f(1.0,0.0,0.0);
gluOrtho2D(0.0,window_wide,0.0,window_height);
glClear(GL_COLOR_BUFFER_BIT);
glutMouseFunc(myMouse);
}

int _tmain(int argc, char** argv)
{
glutInitWindowSize(window_wide,window_height);
glutInitWindowPosition(0, 0);
glutCreateWindow("Bresenham Center Algorithm");
glutDisplayFunc(myDisplay);
glutMainLoop();
return 0;
}
