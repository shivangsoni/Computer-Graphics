 
#ifdef WIN32
#include <windows.h>
#endif

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


void render(void);

int main(int argc, char **argv)
{
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE,GLUT_RGBA);
 glutInitWindowPosition(100,100);
 glutInitWindowSize(640,400);
 glutCreateDisplay("Simple Application");
 glutDisplayFunc(render);
 glutMainLoop();
}
void render(void)
{

}
