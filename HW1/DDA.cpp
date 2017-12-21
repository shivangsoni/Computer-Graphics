#define _USE_MATH_DEFINES
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <cmath>

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

void do_cal();
float x,y;
float x1,x2;
float y1,y2;
float dx,dy;
float step;
float m;
float xin,yin;
char ss[100];
int main(int argc, char **argv)
{
    //the number of pixels in the grid
    grid_width = 100;
    grid_height = 100;
    GLint x11,y11,x21,y21;
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
	/*
	printf("Enter the value of x1 and y1\n");
	scanf("%f",&x1);
	scanf("%f",&y1);
	
	printf("Enter the value of x2 and y2\n");
	scanf("%f",&x2);
	scanf("%f",&y2);*/
	
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

// your framework of choice here

class SolidSphere
{
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

public:
    SolidSphere(float radius, unsigned int rings, unsigned int sectors)
    {
        float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                float const y = sin( -M_PI_2 + M_PI * r * R );
                float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

                *t++ = s*S;
                *t++ = r*R;

                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

                *n++ = x;
                *n++ = y;
                *n++ = z;
        }

        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
        }
    }

    void draw(GLfloat x, GLfloat y, GLfloat z)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(x,y,z);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
        glPopMatrix();
    }
};

SolidSphere sphere(1, 12, 24);

void display()
{
    int const win_width  = 100; // retrieve window dimensions from
    int const win_height = 100; // framework of choice here
    float const win_aspect = (float)win_width / (float)win_height;

    glViewport(0, 0, win_width, win_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, win_aspect, 1, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

#ifdef DRAW_WIREFRAME
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    sphere.draw(0, 0, -5);
    glutSwapBuffers();
    //swapBuffers();
}


void do_cal()
{
  dx=abs(x2-x1);
    dy=abs(y2-y1);
    if(x2<x1)
    {
     int temp1;
     int temp2;
     temp1=x2;
     x2=x1;
     x1=temp1;
     temp2=y2;
     y2=y1;
     y1=temp2;
    }
    m=(y2-y1)/(x2-x1);
    if(dx>=dy)
    {
     step=dx;
    }
    else
    {
     step=dy;
    }
    x=x1;
    y=y1;
    
    xin=float(dx)/float(step);
    yin=float(dy)/float(step);
    
    //draw_pix(x,y);
    if(dx == 0 && (y2-y1)<0)
    {
     for(int k=0;k<=step;k++)
    {
      draw_pix(x,y);
      y=y-1;
      //printf("This is x %d",x);
      //printf("This is y %d",y);
    }
    }
    else if(dx == 0 && (y2-y1)>0)
    {
     for(int k=0;k<=step;k++)
    {
      draw_pix(x,y);
      y=y+1;
      //printf("This is x %d",x);
      //printf("This is y %d",y);
    }
    }
    else if(m>0 && m<1)
    {
    for(int k=0;k<=step;k++)
    {
      draw_pix(x,y);
      x=x+1;
      y=y+m;
      //printf("This is x %d",x);
      //printf("This is y %d",y);
    }
    }
    else if(m>1)
    {
    for(int k=0;k<=step;k++)
    {
      draw_pix(x,y);
      x=x+1/m;
      y=y+1;
      //draw_pix(x,y);
      //printf("This is x %d",x);
      //printf("This is y %d",y);
    }
    }
     else if(-1<m && m<0)
    {    
    for(int k=0;k<=step;k++)
    {
      draw_pix(x,y);
      x=x+1;
      y=y+m;
      
      //printf("This is x %d",x);
      //printf("This is y %d",y);
    }
    }
     else if(m<-1)
    {      
    for(int k=0;k<=step;k++)
    {
      draw_pix(x,y);
      x=x-m;
      y=y-1;
      
      //printf("This is x %d",x);
      //printf("This is y %d",y);
    }
    }
    else if(m==0)
    {
     for(int k=0;k<=step;k++)
     {
      draw_pix(x,y);
      x=x+1;
      y=y;
     }
    }
    else if(dx==0 && dy==0)
    {
     draw_pix(x,y);
    }
    else if(m==1)
    {
     for(int k=0;k<=step;k++)
     {
     draw_pix(x,y);
      x=x+1;
      y=y+1;
     }
    }
    else if(m == -1)
    {
     for(int k=0;k<=step;k++)
     {
     draw_pix(x,y);
      x=x+1;
      y=y-1;
     }
    }
   else
    {
      for(int k=0;k<=step;k++)
     {
     draw_pix(x,y);
      x=x;
      y=y+1;
      
     }
    }
}
//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(int x, int y){
    glBegin(GL_POINTS);
    //printf("Inside Draw\n %d,%d",x,y);
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
