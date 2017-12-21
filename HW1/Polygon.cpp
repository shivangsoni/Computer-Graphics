/*
 * Simple glut demo that can be used as a template for
 * other projects by Garrett Aldrich
 */ 
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


/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;

//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;

/*Window information*/
int win_height;
int win_width;


void init();
void idle();
void display();
void draw_pix(int x, int y);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();

void do_cal(float x1,float y1,float x2,float y2);
float x,y;
float x1,x2;
float y1,y2;
float x3,x4;
float y3,y4;
float xin,yin;
int main(int argc, char **argv)
{
    
    //the number of pixels in the grid
    grid_width = 100;
    grid_height = 100;
    
    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 5;
    
    /*Window information*/
    win_height = grid_height*pixel_size;
    win_width = grid_width*pixel_size;
    
	/*Set up glut functions*/
    /** See https://www.opengl.org/resources/libraries/glut/spec3/spec3.html ***/
    
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	printf("Enter the value of x1 and y1\n");
	scanf("%f",&x1);
	scanf("%f",&y1);
	
	printf("Enter the value of x2 and y2\n");
	scanf("%f",&x2);
	scanf("%f",&y2);
	
	
            printf("Enter the value of x3 and y3\n");
	scanf("%f",&x3);
	scanf("%f",&y3);
	
	printf("Enter the value of x4 and y4\n");
	scanf("%f",&x4);
	scanf("%f",&y4);
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(win_width,win_height);
    //windown title is "glut demo"
	glutCreateWindow("glut demo");
    
	/*defined glut callback functions*/
	glutDisplayFunc(display); //rendering calls here
	glutReshapeFunc(reshape); //update GL on window size change
	glutMouseFunc(mouse);     //mouse button events
	glutMotionFunc(motion);   //mouse movement events
	glutKeyboardFunc(key);    //Keyboard events
	glutIdleFunc(idle);       //Function called while program is sitting "idle"
    
    //initialize opengl variables
    init();
    //start glut event loop
	glutMainLoop();
	return 0;
}

/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
	glClearColor(1.0,1.0,1.0,1.0);
	    
      do_cal(x1,y1,x2,y2);
      do_cal(x2,y2,x3,y3);
      do_cal(x3,y3,x4,y4);
      do_cal(x4,y4,x1,y1);
    //checks for OpenGL errors
	check();
}

//called repeatedly when glut isn't doing anything else
void idle()
{
    //redraw the scene over and over again
	glutPostRedisplay();	
}

//this is where we render the screen
void display()
{
    //clears the screen
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();
	
    //draws every other pixel on the screen
  /*  for (int j = 0; j < grid_height; j+=2){
        for (int i = 0; i < grid_width; i+=2){
            //this is the only "rendering call you should make in project 1"
            draw_pix(i,j);
        }
    }*/

    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
	check();
}
/*
Bresenham’s Line-Drawing Algorithm for |m| < 1.0
1. Input the two line endpoints and store the left endpoint in (x 0 , y 0 ).
2. Set the color for frame-buffer position (x 0 , y 0 ); i.e., plot the first point.
3. Calculate the constants dx, dy, 2dy, and 2dy − 2dx, and obtain the
starting value for the decision parameter as
p 0 = 2dy − dx
4. At each x k along the line, starting at k = 0, perform the following test:
If p k < 0, the next point to plot is (x k + 1, y k ) and
p k+1 = p k + 2dy
Otherwise, the next point to plot is (x k + 1, y k + 1) and
p k+1 = p k + 2*y − 2*x
5. Repeat step 4 dx − 1 more times.
*/

void do_cal(float x1,float y1,float x2,float y2)
{
  //calculate the constants.
  float dx,dy;
  float p0;
  dx=abs(x2-x1);
  dy=abs(y2-y1);
  p0=2*dy-dx;
  float p=p0;
//works only when |m|<1
  if(x1>x2)
  {
    float temp,temp1;
    temp=x1;
    x1=x2;
    x2=temp;
    
    temp1=y1;
    y1=y2;
    y2=temp1;
  }
 
  x=x1;
  y=y1;
  
  printf("x1 is %f and y1 is %f\n",x1,y1);
  printf("x2 is %f and y2 is %f\n",x2,y2);
  if(y1<y2)
  {
  while(x<x2)
  {
     if(p<0)
     {
      x=x+1;
      p=p+2*dy;
     }
     else
     {
      x=x+1;
        y=y+1;
      p=p+2*dy-2*dx;
     }
     draw_pix(x,y);
  }
  }
  if(y1>y2)
  {
     printf("Y1 greater than Y2");
   while(x<x2)
  {
     if(p<0)
     {
      x=x+1;
      p=p+2*dy;
     }
     else
     {
      x=x+1;
      y=y-1;
      p=p+2*dy-2*dx;
     }
     draw_pix(x,y);
  }
      
  }
}
//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(.2,.2,1.0);
    glVertex3f(x+.5,y+.5,0);
    glEnd();
}

/*Gets called when display size changes, including initial creation of the display*/
void reshape(int width, int height)
{
	/*set up projection matrix to define the view port*/
    //update the ne window width and height
	win_width = width;
	win_height = height;
    
    //creates a rendering area across the window
	glViewport(0,0,width,height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0,grid_width,0,grid_height,-10,10);
	
    //clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //set pixel size based on width, if the aspect ratio
    //changes this hack won't work as well
    pixel_size = width/(float)grid_width;
    
    //set pixel size relative to the grid cell size
    glPointSize(pixel_size);
    //check for opengl errors
	check();
}

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
	switch(ch)
	{
		default:
            //prints out which key the user hit
            printf("User hit the \"%c\" key\n",ch);
			break;
	}
    //redraw the scene after keyboard input
	glutPostRedisplay();
}
//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    //print the pixel location, and the grid location
    printf ("MOUSE AT PIXEL: %d %d, GRID: %d %d\n",x,y,(int)(x/pixel_size),(int)(y/pixel_size));
	switch(button)
	{
		case GLUT_LEFT_BUTTON: //left button
            printf("LEFT ");
            break;
		case GLUT_RIGHT_BUTTON: //right button
            printf("RIGHT ");
		default:
            printf("UNKNOWN "); //any other mouse button
			break;
	}
    if(state !=GLUT_DOWN)  //button released
        printf("BUTTON UP\n");
    else
        printf("BUTTON DOWN\n");  //button clicked
    
    //redraw the scene after mouse click
    glutPostRedisplay();
}
//gets called when the curser moves accross the scene
void motion(int x, int y)
{
    //redraw the scene after mouse movement
	glutPostRedisplay();
}
//checks for any opengl errors in the previous calls and
//outputs if they are present
void check()
{
	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		printf("GLERROR: There was an error %s\n",gluErrorString(err) );
		exit(1);
	}
}
