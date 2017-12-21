/*
 * Simple glut demo that can be used as a template for
 * other projects by Garrett Aldrich
 *
 * No guarantees or warantee, any mistakes are intentional 
 * and meant as an exercise for the user :-)
 */


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
#include <string.h>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;


/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;

/*Window information*/
int win_height;
int win_width;


//mouse stuff
int rendpoly = 0;
int curpoly = 0;
int mousex = 0;
int mousey = 0;
int mouseb = 0;

void init();
void idle();
void display();
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();


//floating point for world and object space
struct vpt{
    float x;
    float y;
    float z;
};

//index of triangles
struct tri{
    int i;
    int j;
    int k;
};

//edges for doing rasterization
struct edge{
    vpt p1;
    vpt p2;
    vpt v1;
    vpt v2;
    vpt n1;
    vpt n2;
};

//type of orthogonal projection
enum PROJ_T {P_XY, P_XZ, P_YZ};
PROJ_T PROJ = P_XY;

//map this square to the framebuffer when projecting
vpt min_vbox = {-1.1,-1.1,0.0};
vpt max_vbox = { 1.1, 1.1,0.0};

//read from file
vector< vector<vpt> > vert_lists;
vector< vector<tri> > tri_lists;

//framebuffer representation of screen
float *fbuffer;

//draw framebuffer to screen one pixel at a time
void rend_fb();
//draws a pixel to the screen at location x,y with intensity I
void draw_pix(int x, int y, float I);
//draws pixel to framebuffer
void draw_fbpix(int x, int y, float I= 1.0);
//tests if point is in framebuffer
bool clip_test(int x, int y);
//rasterizes a single triangle
void rasterize_tri(const tri &t, const vector<vpt> &verts, const vector<vpt> &norms);
//runs the scanline algorithm between two edges of the triangle
void scanline_edges(const edge &e1, const edge &e2);
//intersect an edge with a scanline
bool intersect_edge(float scany, const edge& e, float &x_int);
//draws the scanline to framebuffer 
void render_scanline(float y, float x1, float x2);
//projects triangle into pixel/framebuffer space
void project(const tri &t, const vector<vpt> &verts, vpt &v0, vpt &v1, vpt &v2);

int main(int argc, char **argv)
{

    //read triangles in here
    //for this example we will have just 1
    //simple triangle
    vpt v0 = {-1.0,-1.0,-1.0};
    vpt v1 = { 1.0,-1.0, 0.0};
    vpt v2 = { 0.0, 1.0, 1.0};
    vector<vpt> verts({v0,v1,v2});
    vert_lists.push_back(verts);

    vector<tri> tris(0,1,2);
    tri_lists.push_back({{tris}});

    win_width = 1000;
    win_height = 1000;
    grid_width = win_width;
    grid_height = win_height;

    /*Set up glut functions*/
    /** See https://www.opengl.org/resources/libraries/glut/spec3/spec3.html ***/
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
    //set clear color
    glClearColor(0.0,0.0,0.0,0.0);
    //checks for OpenGL errors
	check();
}

//called repeatedly when glut isn't doing anything else
void idle()
{
    ;
    //redraw the scene over and over again
	//glutPostRedisplay();
}

//this is where we render the screen
void display()
{
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
    glLoadIdentity();

    /*****DRAW GRID ON SCREEN*******/
    //creates a rendering area across the window
    glViewport(0,0,win_width,win_height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //map gl ortho so that each pixel is on an integer boundary
    //we will handle the screen space projection ourselves
    glOrtho(0,win_width,0,win_height,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //draw grid
    glBegin(GL_LINES);
        glColor3f(.4,.4,.8);
        glVertex3f(win_width/2,0,0);
        glVertex3f(win_width/2,win_height,0);
        glVertex3f(0,win_height/2,0);
        glVertex3f(win_width,win_height/2,0);
    glEnd();
    

    /******RENDER XY PROJECTION IN TOP LEFT*********/
    PROJ = P_XY;
    //creates a rendering area across the top quad of the screen
    glViewport(0,win_height/2+1,win_width/2-1,win_height/2-1);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //map gl ortho so that each pixel is on an integer boundary
    //we will handle the screen space projection ourselves
    glOrtho(0,grid_width,0,grid_height,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //clear frame buffer
    memset(fbuffer,0,sizeof(float)*grid_width*grid_height);

    //draw triangles to screen buffer
    //iterate through objects
    for(int i = 0; i < tri_lists.size(); i++){
        const vector<tri> &tris = tri_lists[i];
        const vector<vpt> &verts = vert_lists[i];

        //NOTE THIS IS NOT CORRECT
        //YOU NEED TO CALCULATE NORMALS HERE
        //SINCE I'M NOT USING THEM I JUST SET
        //THEM ALL TO 0
        vector<vpt> norms(verts.size());
        //for project 3 need to use the painters algorithm
        //to render the triangles in the correct order,
        //but for this example we just render them in order
        for(int j = 0; j < tris.size(); j++){
            const tri &t = tris[j];
            rasterize_tri(t,verts,norms);
        }
    }
    //copy fromebuffer to screen
    rend_fb();

    /******RENDER XZ PROJECTION IN TOP LEFT*********/
    PROJ = P_XZ;
    //creates a rendering area across the top quad of the screen
    glViewport(win_width/2+1,win_height/2+1,win_width/2-1,win_height/2-1);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //map gl ortho so that each pixel is on an integer boundary
    //we will handle the screen space projection ourselves
    glOrtho(0,grid_width,0,grid_height,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //clear frame buffer
    memset(fbuffer,0,sizeof(float)*grid_width*grid_height);

    //draw triangles to screen buffer
    //iterate through objects
    for(int i = 0; i < tri_lists.size(); i++){
        const vector<tri> &tris = tri_lists[i];
        const vector<vpt> &verts = vert_lists[i];

        //NOTE THIS IS NOT CORRECT
        //YOU NEED TO CALCULATE NORMALS HERE
        //SINCE I'M NOT USING THEM I JUST SET
        //THEM ALL TO 0
        vector<vpt> norms(verts.size());
        //for project 3 need to use the painters algorithm
        //to render the triangles in the correct order,
        //but for this example we just render them in order
        for(int j = 0; j < tris.size(); j++){
            const tri &t = tris[j];
            rasterize_tri(t,verts,norms);
        }
    }
    //copy fromebuffer to screen
    rend_fb();

    /******RENDER YZ PROJECTION IN TOP LEFT*********/
    PROJ = P_YZ;
    //creates a rendering area across the top quad of the screen
    glViewport(0,0,win_width/2-1,win_height/2-1);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //map gl ortho so that each pixel is on an integer boundary
    //we will handle the screen space projection ourselves
    glOrtho(0,grid_width,0,grid_height,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //clear frame buffer
    memset(fbuffer,0,sizeof(float)*grid_width*grid_height);

    //draw triangles to screen buffer
    //iterate through objects
    for(int i = 0; i < tri_lists.size(); i++){
        const vector<tri> &tris = tri_lists[i];
        const vector<vpt> &verts = vert_lists[i];

        //NOTE THIS IS NOT CORRECT
        //YOU NEED TO CALCULATE NORMALS HERE
        //SINCE I'M NOT USING THEM I JUST SET
        //THEM ALL TO 0
        vector<vpt> norms(verts.size());
        //for project 3 need to use the painters algorithm
        //to render the triangles in the correct order,
        //but for this example we just render them in order
        for(int j = 0; j < tris.size(); j++){
            const tri &t = tris[j];
            rasterize_tri(t,verts,norms);
        }
    }
    //copy fromebuffer to screen
    rend_fb();

    //blits to the screen
    glutSwapBuffers();
    //checks for opengl errors
	check();
}

void rend_fb(){
    //could normalize the values between 0 and 1.0 (or 0 and 9) here.

    for(int i = 0; i< grid_height; i++)
        for(int j = 0; j < grid_width; j++)
            draw_pix(j,i,fbuffer[j+i*grid_width]);
}


//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(int x, int y, float intensity){
    glBegin(GL_POINTS);
    //clamp intensity between 0.0 and 1.0
    intensity = intensity<=1.0?(intensity>= 0.0?intensity:0.0): 1.0;
    glColor3f(intensity,intensity,intensity);
    glVertex3f(x+.5,y+.5,0);
    glEnd();
}

/*Gets called when display size changes, including initial craetion of the display*/
void reshape(int width, int height)
{
	/*set up projection matrix to define the view port*/
    //update the ne window width and height
	win_width = width;
    win_height = height;
    //since we are only drawing to 1/4 the screen
    grid_width = width/2;
    grid_height = height/2;

    //make and clear framebuffer;
    delete fbuffer;
    fbuffer = new float[grid_width*grid_height];
    memset(fbuffer,0,sizeof(float)*grid_height*grid_width);

    //viewport and orthographic projection in display function

    check();
}

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
	switch(ch)
    {
    default:
        //prints out which key the user hit   if(x2 < x1)
    {
        cout << "ERROR: This shouldn't happen, set a breakpoint and see where we went wrong\n";
        return;
    }
        printf("User hit the \"%c\" key\n",ch);
        break;
	}
    //redraw the scene after keyboard input
	glutPostRedisplay();
}


//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    mousex = x;
    mousey = y;

	switch(button)
	{
		case GLUT_LEFT_BUTTON: //left button
            printf("LEFT ");
            break;
		case GLUT_RIGHT_BUTTON: //right button
            if(state==GLUT_DOWN){
            }
            printf("MINVP (%d %d) MAXVP (%d %d)");
		default:
            printf("UNKNOWN "); //any other mouse button
			break;
	}
    if(state ==GLUT_DOWN)  //button clicked
    {
        mouseb = button;
    }
    else{
        mouseb = -1;
    }

    
    //redraw the scene after mouse click
    glutPostRedisplay();
}

//gets called when the curser moves accross the scene
void motion(int x, int y)
{
    mousex = x;
    mousey = y;
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


bool clip_test(int x, int y){
    return x >= 0 && x < grid_width && y >= 0 && y < grid_height;
}

void draw_fbpix(int x, int y, float I){
    if(!clip_test(x,y))return;
    fbuffer[x+y*grid_width] = I;
}

//projects into pixel/framebuffer space
void project(const tri &t, const vector<vpt> &verts, vpt &v0, vpt &v1, vpt &v2){
    v0 = verts[t.i];
    v1 = verts[t.j];
    v2 = verts[t.k];

    switch(PROJ){
    case P_XY:
        v0 = {v0.x,v0.y,0.0};
        v1 = {v1.x,v1.y,0.0};
        v2 = {v2.x,v2.y,0.0};
        break;
    case P_XZ:
        v0 = {v0.x,v0.z,0.0};
        v1 = {v1.x,v1.z,0.0};
        v2 = {v2.x,v2.z,0.0};
        break;
    case P_YZ:
        v0 = {v0.y,v0.z,0.0};
        v1 = {v1.y,v1.z,0.0};
        v2 = {v2.y,v2.z,0.0};
        break;
    default:
        cout << "This shouldn't happen\n";
        exit(0);
    }

    //scale/translate into pixel/framebuffer space
    v0.x = (v0.x-min_vbox.x)/(max_vbox.x-min_vbox.x)*grid_width;
    v0.y = (v0.y-min_vbox.y)/(max_vbox.y-min_vbox.y)*grid_height;
    v1.x = (v1.x-min_vbox.x)/(max_vbox.x-min_vbox.x)*grid_width;
    v1.y = (v1.y-min_vbox.y)/(max_vbox.y-min_vbox.y)*grid_height;
    v2.x = (v2.x-min_vbox.x)/(max_vbox.x-min_vbox.x)*grid_width;
    v2.y = (v2.y-min_vbox.y)/(max_vbox.y-min_vbox.y)*grid_height;

}

void rasterize_tri(const tri &t, const vector<vpt> &verts, const vector<vpt> &norms){
    //project triangle into pixel space
    vpt v1,v2,v3;
    project(t,verts,v1,v2,v3);

    //create edges, make sure that the points are sorted
    //by there y values
    edge e1;
    if(v1.y < v2.y )
        e1 = {v1,v2,verts[t.i],verts[t.j],norms[t.i],norms[t.j]};
    else
        e1 = {v2,v1,verts[t.j],verts[t.i],norms[t.j],norms[t.i]};

    edge e2;
    if(v1.y < v3.y)
        e2 = {v1,v3,verts[t.i],verts[t.k],norms[t.i],norms[t.k]};
    else
        e2 = {v3,v1,verts[t.k],verts[t.i],norms[t.k],norms[t.i]};

    edge e3;
    if(v2.y < v3.y)
        e3 = {v2,v3,verts[t.j],verts[t.k],norms[t.j],norms[t.k]};
    else
        e3 = {v3,v2,verts[t.k],verts[t.j],norms[t.k],norms[t.j]};


    //intersect edges
    scanline_edges(e1,e2);
    scanline_edges(e1,e3);
    scanline_edges(e2,e3);

}


//assumes e*.p1.y <= e*.p2.y
void scanline_edges(const edge &e1, const edge &e2){
    if(e1.p1.y == e1.p2.y){//parallel line test
        render_scanline(e1.p1.y,e1.p1.x,e1.p2.x);
        return;
    }
    if(e2.p1.y == e2.p2.y){//parallel line test
        render_scanline(e2.p2.y,e2.p1.x,e2.p2.x);
        return;
    }

    float ymin = max(e1.p1.y,e2.p1.y);
    float ymax = min(e1.p2.y,e2.p2.y);
    //scan from bottom to top of edges
    for(float y = ymin; y <= ymax; y+=1.0){
        float x1,x2; //intersection points
        intersect_edge(y,e1,x1);
        intersect_edge(y,e2,x2);
        if(x1 < x2)
            render_scanline(y,x1,x2);
        else
           render_scanline(y,x2,x1);
    }

}

//assumes e1 isn't parallel to the x axis
//could do interpolation on the edge here as well and return the color intensity
bool intersect_edge(float scany, const edge& e, float &x_int){
    if(e.p2.y==e.p1.y) return false;//parallel lines (0 or inf intersection points)
     x_int = (scany-e.p1.y)*(e.p2.x-e.p1.x)/(e.p2.y-e.p1.y)+e.p1.x;
     return true;
}

//could include intensity values and use those to interpolate here
void render_scanline(float y, float x1, float x2){
    int py=(int)floor(y);
    int px=(int)floor(x1);
    for(px; px < x2; px++)
        draw_fbpix(px,py,1.0);
}
