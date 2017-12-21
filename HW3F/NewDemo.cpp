/*
 * Simple glut demo that can be used as a template for
 * other projects by Garrett Aldrich
 */
 
 
#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include<GL/glm.h>
#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>
#include<cmath>
void rot();
	
void XYortho();
void XZortho();
void YZortho();
/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;
float tx,ty,tz;
//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
//Mat.txt
int no_of_mat;
int material[10000];
float ka[10000][3];
float kd[10000][3];
float ks[10000][3];
float nmat[10000];

//light.txt
float Lx=0,Ly=0,Lz=0;
float Iax=0,Iay=0,Iaz=0;
float Ilx=0,Ily=0,Ilz=0;
float F_dist=0;
float K_dist=0;
float pixel_size;
float degree;
void rotationX();
void rotationY();
void rotationZ();
float ax,ay,az;
float bx,by,bz;
void Translation();
float l;
/*Window information*/
int win_height;
int win_width;
int numpoly;
float polyx[10000][10000];
float polyy[10000][10000];
float polyz[10000][10000];
int polyvertices[10000];
int vertex[10000];
void translate_arb(float tx, float ty, float tz, int tid);
  void rot1_arb(int rotid);
  void rot2_arb(int rotid);
  void rot3_arb(int rotid,float degree);
  void rot4_arb(int rotid);
  void rot5_arb(int rotid);
  //translate_arb(+ax,+ay,+az,rotid);
float xcentroid,ycentroid,zcentroid;
float scalex,scaley,scalez;
void Scaling();
int vertexpoint1[10000][10000];
int vertexpoint2[10000][10000];
void init();
void idle();
void display();
void draw_pix(int x, int y);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void linee();
float dx,dy,dz,d,vx,vy,vz;
void rotationArbitary();
void outputVertices();
void flow();
FILE *fp1;
void flow()
{
   FILE * fp2 = fopen("light.txt", "r"); // Correct statement to open a file
    if(fp2 == NULL) {
        fprintf(stderr, "Can't open input file testFile\n");
        exit(1);
    }
    	
    	GLfloat nump11,nump21,nump31;
    	fscanf(fp2,"%f",&nump11);
    	fscanf(fp2,"%f",&nump21);
    	fscanf(fp2,"%f",&nump31);
    	
	Lx=nump11;Ly=nump21;Lz=nump31;
	printf("%f,%f,%f",Lx,Ly,Lz);
	fscanf(fp2,"%f",&nump11);
    	fscanf(fp2,"%f",&nump21);
    	fscanf(fp2,"%f",&nump31);
	
           Iax=nump11;Iay=nump21;Iaz=nump31;
           
           	fscanf(fp2,"%f",&nump11);
    	fscanf(fp2,"%f",&nump21);
    	fscanf(fp2,"%f",&nump31);
	
           Ilx=nump11;Ily=nump21;Ilz=nump31;
                      
           	fscanf(fp2,"%f",&nump11);
    	fscanf(fp2,"%f",&nump21);
            F_dist=nump11;
            K_dist=nump21;
	fclose(fp2);

   FILE * fp3 = fopen("materials.txt", "r"); // Correct statement to open a file
    if(fp3 == NULL) {
        fprintf(stderr, "Can't open input file testFile\n");
        exit(1);
    }
            GLint nma;
            fscanf(fp3,"%d",&nma);
            no_of_mat=nma;
            
            for(int i=0;i<no_of_mat;i++)
            {
             GLfloat nump1,nump2,nump3;
    	fscanf(fp3,"%f",&nump1);
    	fscanf(fp3,"%f",&nump2);
    	fscanf(fp3,"%f",&nump3);
    	ka[i][0]=nump1;
    	ka[i][1]=nump2;
    	ka[i][2]=nump3;
    	fscanf(fp3,"%f",&nump1);
    	fscanf(fp3,"%f",&nump2);
    	fscanf(fp3,"%f",&nump3);
    	kd[i][0]=nump1;
    	kd[i][1]=nump2;
    	kd[i][2]=nump3;
            fscanf(fp3,"%f",&nump1);
    	fscanf(fp3,"%f",&nump2);
    	fscanf(fp3,"%f",&nump3);
    	ks[i][0]=nump1;
    	ks[i][1]=nump2;
    	ks[i][2]=nump3;
    	GLint n_m;
    	fscanf(fp3,"%d",&n_m);
    	nmat[i]=n_m;
            }
	fclose(fp3);
}
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
	flow();
    FILE * fp = fopen("inp.txt", "r"); // Correct statement to open a file
    if(fp == NULL) {
        fprintf(stderr, "Can't open input file testFile\n");
        exit(1);
    }
    	
    	 GLint nump;
    	fscanf(fp,"%d",&nump);
	numpoly=nump;
	for(int i=0;i<numpoly;i++)
	{
	//printf("Enter the number of vertices in a %d polygon\n",i+1);
	GLint verte;
	fscanf(fp,"%d",&verte);
	polyvertices[i]=verte;
	//scanf("%d",&v[i]);
	  for(int j=0;j<polyvertices[i];j++)
	 {
	    // printf("Enter the %d point x and y coordinate respectively\n",j+1);
	     GLfloat xcord,ycord,zcord;
	     fscanf(fp,"%f",&xcord);
	     fscanf(fp,"%f",&ycord);
	     fscanf(fp,"%f",&zcord);
	     polyx[i][j]=xcord;
	      polyy[i][j]=ycord;
	      polyz[i][j]=zcord;
	     printf("\n");
	 }
	 fscanf(fp,"%d",&verte);
	 vertex[i]=verte;
	 for(int j=0;j<verte;j++)
	 {
	  GLint x1,y1;
	  fscanf(fp,"%d",&x1);
	  vertexpoint1[i][j]=x1;
	  fscanf(fp,"%d",&y1);
	  vertexpoint2[i][j]=y1;
	 }
	}
	fclose(fp);
fp1 = fopen("inp.txt", "w"); // Correct statement to open a file
    if(fp1 == NULL) {
        fprintf(stderr, "Can't open input file testFile\n");
        exit(1);
    }
    /*printf("Number of polygon on screen\n");
    scanf("%d",&numpoly);
    for(int i=0;i<numpoly;i++)
    {
      printf("Enter the %d polygon vertices\n",i);
      scanf("%d",&polyvertices[i]);
      for(int j=0;j<polyvertices[i];j++)
      {
      printf("Enter the %d  vertices x coordinates\n",j+1);
      scanf("%f",&polyx[i][j]);
      printf("Enter the %d  vertices y coordinates\n",j+1);
      scanf("%f",&polyy[i][j]);
      printf("Enter the %d  vertices z coordinates\n",j+1);
      scanf("%f",&polyz[i][j]);
      }
          
    printf("\n Enter the number of edges of object %d",i+1);
    scanf("%d",&vertex);
    for(int j=0;j<vertex;j++)
    {
          printf("Enter the point where edge must start\n");
           scanf("%d",&vertexpoint1[j]);
           printf("Enter the  point where edge must end\n");
           scanf("%d",&vertexpoint2[j]);
    }
    }*/
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(win_width,win_height);
    //windown title is "glut demo"
	glutCreateWindow("glut demo");
    
	/*defined glut callback functions*/
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
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
void XYortho()
{
 glLineWidth(2.5);
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_LINES);
//glVertex3f(0.0, 10.0, 4.0);
//glVertex3f(10.0, 0, 5);

  for(int i=0;i<numpoly;i++)
  {
    for(int j=0;j<vertex[i];j++)
    {
     //     printf("Coordinates are \n");
     //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
     //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
      glVertex3f(polyx[i][vertexpoint1[i][j]],polyy[i][vertexpoint1[i][j]],0);
      glVertex3f(polyx[i][vertexpoint2[i][j]], polyy[i][vertexpoint2[i][j]],0);
    }
  }
 glEnd();
}

void XZortho()
{
  glLineWidth(2.5);
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_LINES);
//glVertex3f(0.0, 10.0, 4.0);
//glVertex3f(10.0, 0, 5);

  for(int i=0;i<numpoly;i++)
  {
    for(int j=0;j<vertex[i];j++)
    {
     //     printf("Coordinates are \n");
     //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
     //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
      glVertex3f(polyx[i][vertexpoint1[i][j]],polyz[i][vertexpoint1[i][j]]+1,0);
      glVertex3f(polyx[i][vertexpoint2[i][j]], polyz[i][vertexpoint2[i][j]]+1,0);
    }
  }
 glEnd();
}

void YZortho()
{
   glLineWidth(2.5);
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_LINES);
//glVertex3f(0.0, 10.0, 4.0);
//glVertex3f(10.0, 0, 5);

  for(int i=0;i<numpoly;i++)
  {
    for(int j=0;j<vertex[i];j++)
    {
     //     printf("Coordinates are \n");
     //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
     //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
      glVertex3f(polyy[i][vertexpoint1[i][j]]+1,polyz[i][vertexpoint1[i][j]]+1,0);
      glVertex3f(polyy[i][vertexpoint2[i][j]]+1, polyz[i][vertexpoint2[i][j]]+1,0);
    }
  }
 glEnd();
}
/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
	glClearColor(1.0,1.0,1.0,1.0);
    //checks for OpenGL errors
	check();
}
void linee()
{
glLineWidth(2.5);
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_LINES);
      glVertex3f(1,0,0);
      glVertex3f(1,2,0);
      glVertex3f(0,1,0);
      glVertex3f(2,1,0);
 glEnd();
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
	linee();
	XYortho();
	XZortho();
	YZortho();
	//blits the current opengl framebuffer on the screen
    glutSwapBuffers();
  printf("Enter the operation you want to perform\n");
  printf("For translation press 1\n");
  printf("For Scaling press 2\n");
  printf("For Rotation press 3\n");
  printf("For exit press press -1\n");
  int d;
  scanf("%d",&d);
  switch(d)
  {
   case 1:
    Translation();
     break;
   case 2:
    Scaling();
     break;
   case 3:
      rot();
     break;
   case -1:
    printf("Exiting from the system\n");
    outputVertices();
    exit(1); 
  }
 
    //checks for opengl errors
	check();
}
void outputVertices()
{
  GLint a=numpoly;
  fprintf(fp1,"%d\n",a);
  for(int i=0;i<a;i++)
  {
    GLint b=polyvertices[i];
    fprintf(fp1,"%d\n",b);
    for(int j=0;j<b;j++)
    {
     GLfloat p,w,z;
     p=polyx[i][j];
     w=polyy[i][j];
     z=polyz[i][j];
     fprintf(fp1,"%f\t",p);
     fprintf(fp1,"%f\t",w);
     fprintf(fp1,"%f\n",z);
    }
    GLint v=vertex[i];
    fprintf(fp1,"%d\n",v);
    for(int j=0;j<v;j++)
    {
     GLint p,w;
     p=vertexpoint1[i][j];
     w=vertexpoint2[i][j];
     fprintf(fp1,"%d\t",p);
     fprintf(fp1,"%d\n",w);
    }
  }
}
void rot()
{
   int xs;
   printf("For rotation along x press 1\n");
   printf("For rotation along y press 2\n");
   printf("For rotation along z press 3\n");
   printf("For rotation along any arbitary axis press 4\n");
   scanf("%d",&xs);
    if(xs==1)
    {
     rotationX();
    }
    else if(xs==2)
    {
     rotationY();
    }
    else if(xs==3)
    {
     rotationZ();
    }
    else if(xs==4)
    {
     rotationArbitary();
    }
}
void rotationX()
{
	int rotid;
	printf("Enter the polygon id what you want to rotate\n");
	scanf("%d",&rotid);
	printf("Enter the degree of rotation\n");
	scanf("%f",&degree);
	//degree=(3.14)/180*degree;
           
    degree=degree*(M_PI)/180;
    xcentroid=1;
    ycentroid=0;
    zcentroid=0;
    for(int i=0;i<polyvertices[rotid-1];i++)
    {
      float a,b;
      a=polyy[rotid-1][i];
      b=polyz[rotid-1][i];
      polyy[rotid-1][i]=a*cos(degree)-b*(sin(degree));
      polyz[rotid-1][i]=a*sin(degree)-b*(cos(degree));
    }
    	
	XYortho();
	XZortho();
	YZortho();
}
void rotationY()
{
 	int rotid;
	printf("Enter the polygon id what you want to rotate\n");
	scanf("%d",&rotid);
	printf("Enter the degree of rotation\n");
	scanf("%f",&degree);
	//degree=(3.14)/180*degree;
           
    degree=degree*(M_PI)/180;
    xcentroid=0;
    ycentroid=1;
    zcentroid=0;
    for(int i=0;i<polyvertices[rotid-1];i++)
    {
      float a,b;
      a=polyx[rotid-1][i];
      b=polyz[rotid-1][i];
      polyx[rotid-1][i]=a*cos(degree)+b*(sin(degree));
      polyz[rotid-1][i]=-a*sin(degree)+b*(cos(degree));
    }
	
	XYortho();
	XZortho();
	YZortho();  
}
void rotationZ()
{
 	int rotid;
	printf("Enter the polygon id what you want to rotate\n");
	scanf("%d",&rotid);
	printf("Enter the degree of rotation\n");
	scanf("%f",&degree);
	//degree=(3.14)/180*degree;
           
    degree=degree*(M_PI)/180;
    xcentroid=1;
    ycentroid=0;
    zcentroid=0;
    for(int i=0;i<polyvertices[rotid-1];i++)
    {
      float a,b;
      a=polyx[rotid-1][i];
      b=polyy[rotid-1][i];
      polyx[rotid-1][i]=a*cos(degree)-b*(sin(degree));
      polyy[rotid-1][i]=a*sin(degree)+b*(cos(degree));
    }
    	
	XYortho();
	XZortho();
	YZortho();
}
void rotationArbitary()
{
	int rotid;
	printf("Enter the polygon id what you want to rotate\n");
	scanf("%d",&rotid);
	printf("Enter the degree of rotation\n");
	scanf("%f",&degree);
	//degree=(3.14)/180*degree;           
            degree=degree*(M_PI)/180;
            getchar();
  printf("Enter the points on the arbitary axis along which you want to rotate\n");
  printf("Enter the x coordinate of point 1\n");
  
  scanf("%f\n",&ax);
  printf("Enter the y coordinate of point 1\n");
  scanf("%f\n",&ay);
  printf("Enter the z coordinate of point 1\n");
  scanf("%f\n",&az);
  printf("Enter the x coordinate of point 2\n");
  scanf("%f\n",&bx);
  printf("Enter the y coordinate of point 2\n");
  scanf("%f\n",&by);
  printf("Enter the z coordinate of point 2\n");
  scanf("%f\n",&bz);
  //1st translate by -a
  translate_arb(-ax,-ay,-az,rotid);
  vx=bx-ax;
  vy=by-ay;
  vz=bz-az;
  d=sqrt(vx*vx+vy*vy+vz*vz);
  dx=vx/d;
  dy=vy/d;
  dz=vz/d;
  //2nd rot into xz plane
  rot1_arb(rotid);
  rot2_arb(rotid);
  rot3_arb(rotid, degree);
  rot4_arb(rotid);
  rot5_arb(rotid);
  translate_arb(+ax,+ay,+az,rotid);
}
void rot3_arb(int rotid,float degree)
{
     for(int i=0;i<polyvertices[rotid-1];i++)
    {
      float a,b;
      a=polyx[rotid-1][i];
      b=polyy[rotid-1][i];
      polyx[rotid-1][i]=a*cos(degree)-b*(sin(degree));
      polyy[rotid-1][i]=a*sin(degree)+b*(cos(degree));
    }
    linee();
}
  void rot4_arb(int rotid)
  {
          for(int j=0;j<polyvertices[rotid-1];j++)
    {
//     printf("Coordinates are \n");
  //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
   //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
       float a,b;
       l=sqrt(dy*dy+dz*dz);
       a=polyy[rotid-1][j];
       b=polyz[rotid-1][j];
       polyy[rotid-1][j]=a*(dz/l)+b*(dy/l);
       polyz[rotid-1][j]=-a*(dy/l)+b*(dz/l);
    }
    linee();
  }
  void rot5_arb(int rotid)
  {
     for(int j=0;j<polyvertices[rotid-1];j++)
    {
//     printf("Coordinates are \n");
  //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
   //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
       float a,b;
       a=polyx[rotid-1][j];
       b=polyz[rotid-1][j];
       polyx[rotid-1][j]=a*(l)-b*(dx);
       polyz[rotid-1][j]=a*(dx)+b*(l);
    }
    	
	XYortho();
	XZortho();
	YZortho();
  }
void rot1_arb(int rotid)
{
    
       for(int j=0;j<polyvertices[rotid-1];j++)
    {
//     printf("Coordinates are \n");
  //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
   //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
       float a,b;
       l=sqrt(dy*dy+dz*dz);
       a=polyy[rotid-1][j];
       b=polyz[rotid-1][j];
       polyy[rotid-1][j]=a*(dz/l)-b*(dy/l);
       polyz[rotid-1][j]=a*(dy/l)+b*(dz/l);
    }
    	
	XYortho();
	XZortho();
	YZortho();
}
void rot2_arb(int rotid)
{
        for(int j=0;j<polyvertices[rotid-1];j++)
    {
//     printf("Coordinates are \n");
  //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
   //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
       float a,b;
       a=polyx[rotid-1][j];
       b=polyz[rotid-1][j];
       polyx[rotid-1][j]=a*(l)+b*(dx);
       polyz[rotid-1][j]=a*(-dx)+b*(l);
    }
    	
	XYortho();
	XZortho();
	YZortho();
}
void translate_arb(float tx, float ty, float tz, int tid)
{
      for(int j=0;j<polyvertices[tid-1];j++)
    {
//     printf("Coordinates are \n");
  //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
   //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
       polyx[tid-1][j]=polyx[tid-1][j]+tx;
       polyy[tid-1][j]=polyy[tid-1][j]+ty;
       polyz[tid-1][j]=polyz[tid-1][j]+tz;
    }
    	
	XYortho();
	XZortho();
	YZortho();
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
	glOrtho(0,2,0,2,-10,10);
    
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
void Translation()
{
            int tid;
            printf("Which polygon do you want to do Translation to\n");
            scanf("%d",&tid);
	printf("Enter the number to be translated in x direction\n");
	scanf("%f",&tx);
	printf("Enter the number to be translated in y direction\n");
	scanf("%f",&ty);
            printf("Enter the number to be translated in z direction\n");
            scanf("%f",&tz);
    for(int j=0;j<polyvertices[tid-1];j++)
    {
//     printf("Coordinates are \n");
  //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
   //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
       polyx[tid-1][j]=polyx[tid-1][j]+tx;
       polyy[tid-1][j]=polyy[tid-1][j]+ty;
       polyz[tid-1][j]=polyz[tid-1][j]+tz;
    }
    	
	XYortho();
	XZortho();
	YZortho();
}

void Scaling()
{
	int scaleid;
	printf("Enter the polygon id you want to scale\n");
	scanf("%d",&scaleid);
	printf("Enter the factor to be scaled in x direction\n");
	scanf("%f",&scalex);
	printf("Enter the factor to be scaled in y direction\n");
	scanf("%f",&scaley);
	printf("Enter the factor to be scaled in z direction\n");
	scanf("%f",&scalez);
	
    xcentroid=0;
    ycentroid=0;
    zcentroid=0;
    for(int i=0;i<polyvertices[scaleid-1];i++)
    {
     xcentroid=xcentroid+polyx[scaleid-1][i];
     ycentroid=ycentroid+polyy[scaleid-1][i];
     zcentroid=zcentroid+polyy[scaleid-1][i];
    }
    xcentroid=xcentroid/float(polyvertices[scaleid-1]);
    ycentroid=ycentroid/float(polyvertices[scaleid-1]);
    zcentroid=zcentroid/float(polyvertices[scaleid-1]);
      for(int i=0;i<polyvertices[scaleid-1];i++)
      {
       polyx[scaleid-1][i]=scalex*(polyx[scaleid-1][i]-xcentroid)+xcentroid;
       polyy[scaleid-1][i]=scaley*(polyy[scaleid-1][i]-ycentroid)+ycentroid;
       polyz[scaleid-1][i]=scalez*(polyz[scaleid-1][i]-zcentroid)+zcentroid;
      }
       	
	XYortho();
	XZortho();
	YZortho();
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
