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

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
int countt=0;
int f=0;
/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;

void reset_knots();
void draw_liness1();
void addvertices();
void deletevertices();
void addpoly();
float resolution;
void replace();
float polyxnew[100][100];
float polyynew[100][100];
float smaller=0;
float greater=0;
int vernew=0;
float polyx[10][100];
float polyy[10][100];
int numpoly;
FILE *fp1;

int v[100];
void outputfile();
float u1[100][100];
void BB_Spline(int order,int qq);
int segment[100];
 float bx[10][100][100];
 float by[10][100][100];
//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;
//void resize(int ,int);
/*Window information*/
int win_height;
int win_width;
float u[100];
float ax[5];
float ay[5];
void init();
void idle();
void display();
void draw_lines();
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void biezer_curve(int i,float t);
void biezer_curve1(int numb,int i,int order,float t);
void B_curve(int a,int order);
static int selectedKnot = 0; // Selected knot number.
static int splineOrder = 1; // Order of spline.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Knot values scaled by a factor of 10 to avoid floating point error when comparing knot values.
static float knots[9] = {0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0}; 	
// End globals.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Function to increase value of a knot.
void increaseKnot(int i)
{
   if ( (i < 8) )
   {
      if (knots[i] < knots[i+1]) knots[i] += 1.0;
	  else 
	  {
	     increaseKnot(i+1);
		 if (knots[i] < knots[i+1]) knots[i] += 1.0;
	  }
   }
   if ( (i == 8 )  && ( knots[i] < 80) ) knots[i] += 1.0;
}

// Function to decrease value of a knot.
void decreaseKnot(int i)
{
   if ( (i > 0) )
   {
      if (knots[i] > knots[i-1]) knots[i] -= 1.0;
	  else 
	  {
	     decreaseKnot(i-1);
		 if (knots[i] > knots[i-1]) knots[i] -= 1.0;
	  }
   }
   if ( (i == 0 )  && ( knots[i] > 0) ) knots[i] -= 1.0;
}

// Reset knot values.
void resetKnots(void)
{
   int i;
   for (i = 0; i < 9; i++) knots[i] = 10.0*i;
   selectedKnot = 0;
}

// Recursive computation of B-spline functions.
float Bspline(int index, int order, float u)
{
   float coef1, coef2;
   if ( order == 1 )
   {
	  if ( index == 0 ) if ( ( knots[index] <= u ) && ( u <= knots[index+1] ) ) return 1.0;
      if ( ( knots[index] < u ) && ( u <= knots[index+1] ) ) return 1.0;
	  else return 0.0;
   }
   else
   {
      if ( knots[index + order - 1] == knots[index] ) 
	  {
	     if ( u == knots[index] ) coef1 = 1;
		 else coef1 = 0;
	  }
	  else coef1 = (u - knots[index])/(knots[index + order - 1] - knots[index]);

      if ( knots[index + order] == knots[index+1] )
	  {
		 if ( u == knots[index + order] ) coef2 = 1;
		 else coef2 = 0;
	  }
	  else coef2 = (knots[index + order] - u)/(knots[index + order] - knots[index+1]);
		
      return ( coef1 * Bspline(index, order-1, u) + coef2 * Bspline(index+1,order-1 ,u) );
   }
}


void BB_Spline(int k,int qq){
            glLineWidth(2.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0);
     
      
       for(int j=0;j<v[qq]-1;j++)
       {
   //    printf("point on the line %d,%d,%d,%d",polyx[i],polyy[i],polyx[i+1],polyy[i+1]);
         glVertex2f(polyx[qq][j],polyy[qq][j]);
         glVertex2f(polyx[qq][j+1],polyy[qq][j+1]);
       }
     // printf("point on the line %d,%d,%d,%d",polyx[v-1],polyy[v-1],polyx[0],polyy[0]);
       //bresenham(polyx[i][v[i]-1],polyy[i][v[i]-1],polyx[i][0],polyy[i][0]); 
  glEnd();
 for(int i=0;i<100;i++)
 {
  segment[i]=1;
 }
 
 int im=0;
  for(int j=0;j<v[qq];j++)
  {
    im=j+k-1;
    if(im<v[qq]-1)
    {
     segment[qq]=segment[qq]+1;
    }
    else
    break;
  } 
 
/* for(int i=0;i<numpoly;i++)
 {
   printf("%d\n",segment[i]);
 }*/
 
 //Calculate u

   for(int j=0;j<100;j++)
   {
    u1[qq][j]=j;
    //printf("%d\n",u1[i][j]);
   }
 
    glLineWidth(2.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0); 
 //Calculate ubar
 float dx[10][100][100],dy[10][100][100];
 
  for(float ubar=u1[qq][k-1];ubar<=u1[qq][k+segment[qq]];ubar=ubar+(1/resolution))
  {
    int indexI;
    indexI=floor(ubar);
    //printf("This is indexI value %d\n",indexI);
    for(int p=0;p<v[qq];p++)
     {
       dx[qq][0][p]=polyx[qq][p];
       dy[qq][0][p]=polyy[qq][p];
       //glVertex2f(dx[q][0][p],dy[q][0][p]);
     }
    for(int j=1;j<=k-1;j++)
    {
     for(int i=indexI-(k-1);i<=indexI-j;i++)
     {
      dx[qq][j][i]=(u1[qq][i+k]-ubar)/(u1[qq][i+k]-u1[qq][i+j])*dx[qq][j-1][i]+(ubar-u1[qq][i+j])/(u1[qq][i+k]-u1[qq][i+j])*dx[qq][j-1][i+1];
      dy[qq][j][i]=(u1[qq][i+k]-ubar)/(u1[qq][i+k]-u1[qq][i+j])*dy[qq][j-1][i]+(ubar-u1[qq][i+j])/(u1[qq][i+k]-u1[qq][i+j])*dy[qq][j-1][i+1];
     //printf("%f %f\n",dx[q][i][j],dy[q][i][j]);
     }
    }
//       printf("%f %f\n",dx[q][k-1][indexI-(k-1)],dy[q][k-1][indexI-(k-1)]);
       glVertex2f(dx[qq][k-1][indexI-(k-1)],dy[qq][k-1][indexI-(k-1)]);
  }
          
    glEnd();
    //glLineWidth(2.0); 
}

// Draw a B-spline function graph as line strip and joints as points.
void drawSpline(int index, int order)
{
   float x;   
   int j;

   // Drawing are scaled by factor of 3 in the y-direction.
   // Special case to handle order 1 to avoid vertical edges.
   if (order == 1)
   {
	  // Spline curve.
      glBegin(GL_LINE_STRIP);
	  for ( x = knots[index]; x < knots[index+1]; x+=0.05 )
         glVertex3f( -40.0 + x, 10.0, 0.0 );	
	  glEnd();
	  glPointSize(3.0);

	  // Joints.
	  glBegin(GL_POINTS);
	  glVertex3f( -40.0 + knots[index], 10.0, 0.0);
	  glVertex3f( -40.0 + knots[index+1], 10.0, 0.0);
	  glEnd();
   }
   else
   {
	  // Spline curve.
	  glBegin(GL_LINE_STRIP);
	  for ( x = knots[index]; x <= knots[index + order]; x += 0.005 )
	     glVertex3f( -40.0 + x, 30*Bspline(index, order, x) - 20.0, 0.0 );	
      glEnd();

	  // Joints.
	  glColor3f(0.0, 0.0, 0.0);
	  glBegin(GL_POINTS);
	  for (j = index; j <= index + order; j++)
	     glVertex3f( -40.0 + knots[j], 30*Bspline(index, order, knots[j]) - 20.0, 0.0 );
	  glEnd();
   }
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();

   glTranslatef(0.0, 0.5, 0.0);

   // Write spline order.
   glColor3f(0.0, 0.0, 0.0);
   switch (splineOrder) 
   {
      case 1:
          glRasterPos3f(-10.5, 35.0, 0.0);
          writeBitmapString((void*)font, "First-order B-splines");
	  break;
      case 2:
          glRasterPos3f(-10.5, 35.0, 0.0);
          writeBitmapString((void*)font, "Linear B-splines");
	  break;
      case 3:
          glRasterPos3f(-10.5, 35.0, 0.0);
          writeBitmapString((void*)font, "Quadratic B-splines");
	  break;
      case 4:
          glRasterPos3f(-10.5, 35.0, 0.0);
          writeBitmapString((void*)font, "Cubic B-splines");
	  break;
      default:
      break;
   }

   // Draw successive B-spline functions for the chosen order.
   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
   for (i = 0; i < 9 - splineOrder; i++ )
   {
	  switch (i) 
	  {     
         case 0:
	       glColor3f(1.0, 0.0, 0.0);
	     break;
         case 1:
	       glColor3f(0.0, 1.0, 0.0);
	     break;
         case 2:
	       glColor3f(0.0, 0.0, 1.0);
	     break;
         case 3:
	       glColor3f(1.0, 0.0, 1.0);
	     break;
         case 4:
	       glColor3f(0.0, 1.0, 1.0);
	     break;
         case 5:
	       glColor3f(1.0, 1.0, 0.0);
	     break;
         case 6:
	       glColor3f(0.0, 0.0, 0.0);
	     break;
         case 7:
	       glColor3f(1.0, 0.0, 0.0);
	     break;
         default:
         break;
	  }
       drawSpline(i, splineOrder);
   }

   // Draw the x-axis.
   glColor3f(0.0, 0.0, 0.0);
   glBegin(GL_LINES);
      glVertex3f(-40.0, -20.0, 0.0);
      glVertex3f( 40.0, -20.0, 0.0);  
   glEnd();

   // Draw points on the x-axis.
   glPointSize(5.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 9; i++) 
         glVertex3f(-40.0 + (float)i*10.0, -20.0, 0.0);
   glEnd();

   // Label the points on the x-axis.
   glRasterPos3f(-40.5, -23.0, 0.0);
   writeBitmapString((void*)font, "0");
   glRasterPos3f(-30.5, -23.0, 0.0);
   writeBitmapString((void*)font, "1");
   glRasterPos3f(-20.5, -23.0, 0.0);
   writeBitmapString((void*)font, "2");
   glRasterPos3f(-10.5, -23.0, 0.0);
   writeBitmapString((void*)font, "3");
   glRasterPos3f(-0.5, -23.0, 0.0);
   writeBitmapString((void*)font, "4");
   glRasterPos3f(9.5, -23.0, 0.0);
   writeBitmapString((void*)font, "5");
   glRasterPos3f(19.5, -23.0, 0.0);
   writeBitmapString((void*)font, "6");
   glRasterPos3f(29.5, -23.0, 0.0);
   writeBitmapString((void*)font, "7");
   glRasterPos3f(39.5, -23.0, 0.0);
   writeBitmapString((void*)font, "8");

   // Draw the y-axis.
   glBegin(GL_LINES);
      glVertex3f(-40.0, -20.0, 0.0);
      glVertex3f(-40.0, 40.0, 0.0);  
   glEnd();

   // Draw points on the y-axis.
   glBegin(GL_POINTS);
         glVertex3f(-40.0, 10.0, 0.0);
         glVertex3f(-40.0, 40.0, 0.0);
   glEnd();

   // Label the points on the y-axis.
   glRasterPos3f(-42.5, -20.5, 0.0);
   writeBitmapString((void*)font, "0");
   glRasterPos3f(-42.5, 9.5, 0.0);
   writeBitmapString((void*)font, "1");
   glRasterPos3f(-42.5, 39.5, 0.0);
   writeBitmapString((void*)font, "2");

   // Draw horizontal bars corresponding to knot points.
   glBegin(GL_LINES);
      for (i = 0; i < 9; i++) 
	  {
         glVertex3f(-40.0, -35.0 + (float)i, 0.0);
         glVertex3f( 40.0, -35.0 + (float)i, 0.0);
	  }
   glEnd();

   // Draw the knot points as dots on their respective bars.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 9; i++) 
         glVertex3f( -40.0 + knots[i], -35.0 + (float)i, 0.0);
   glEnd();

   // Highlight the selected knot point.
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POINTS);
      glVertex3f( -40.0 + knots[selectedKnot], 
		          -35.0 + (float)selectedKnot, 0.0); 
   glEnd();

   // Label the knot bars.
   glColor3f(0.0, 0.0, 0.0);
   glRasterPos3f(-7.0, -40.0, 0.0);
   writeBitmapString((void*)font, "Knot Values");
   
   glPopMatrix();
   glutSwapBuffers();
}


// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_LEFT) decreaseKnot(selectedKnot);
   if(key == GLUT_KEY_RIGHT) increaseKnot(selectedKnot);
   if(key == GLUT_KEY_UP) 
   {
	   if (splineOrder < 4) splineOrder++; else splineOrder = 1;
   }
   if(key == GLUT_KEY_DOWN) 
   {
	   if (splineOrder > 1) splineOrder--; else splineOrder = 4;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   printf("Interaction:\n");
   printf("Press the up/down arrow keys to cycle between order 1 through 4.\n");
   printf("Press space to select a knot points.");
   printf("Press the left/right arrow keys to move the selected knot point.\n");
   printf("Press delete to reset.\n");
}










int main(int argc, char **argv)
{
 FILE * fp = fopen("input.txt", "r"); // Correct statement to open a file
    if(fp == NULL) {
        fprintf(stderr, "Can't open input file testFile\n");
        exit(1);
    }
   
    //the number of pixels in the grid
    grid_width = 100;
    grid_height = 100;
    
    /*printf("Enter the vertices of the array object x and y respectively\n");
    scanf("%f",&ax[0]);
    scanf("%f",&ay[0]);
    scanf("%f",&ax[1]);
    scanf("%f",&ay[1]);
    scanf("%f",&ax[2]);
    scanf("%f",&ay[2]);
    scanf("%f",&ax[3]);
    scanf("%f",&ay[3]);
    scanf("%f",&ax[4]);
    scanf("%f",&ay[4]);*/  
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
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    
    GLint nump;	
	/*printf("Enter the polygon you want to display on the screen\n");
	scanf("%d",&numpoly);*/
	fscanf(fp,"%d",&nump);
	numpoly=nump;
	for(int i=0;i<numpoly;i++)
	{
	//printf("Enter the number of vertices in a %d polygon\n",i+1);
	GLint vertex;
	fscanf(fp,"%d",&vertex);
	v[i]=vertex;
	//scanf("%d",&v[i]);
	  for(int j=0;j<v[i];j++)
	 {
	    // printf("Enter the %d point x and y coordinate respectively\n",j+1);
	    GLfloat xcord,ycord;
	     fscanf(fp,"%f",&xcord);
	     fscanf(fp,"%f",&ycord);
	      polyx[i][j]=xcord;
	      polyy[i][j]=ycord;
	     printf("\n");
	 }
	}
	fclose(fp);
	fp1 =fopen("input.txt", "w");    
	    
    if(fp1==NULL)
    {
        fprintf(stderr, "Can't open input file testFile\n");
        exit(1);
    }    
    
   printf("Enter the resolution you want\n");
   scanf("%f",&resolution); 
    
    
    
    
    
     //  printInteraction();
   //glutInit(&argc, argv);
   //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(600, 600);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("glut demo");
   setup(); 
   glutMouseFunc(mouse);     //mouse button events
   glutMotionFunc(motion);
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);  
   glutKeyboardFunc(key);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop(); 
    
    
    
    
    
    /*glutInitWindowSize(win_width,win_height);
    //windown title is "glut demo"
	glutCreateWindow("glut demo");
    
	glutDisplayFunc(display); //rendering calls here
	glutReshapeFunc(reshape); //update GL on window size change
	glutMouseFunc(mouse);     //mouse button events
	glutMotionFunc(motion);   //mouse movement events
	glutKeyboardFunc(key);    //Keyboard events
	glutIdleFunc(idle);
	setup();
	//glutReshapeFunc(resize);       //Function called while program is sitting "idle"
  // glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
    //initialize opengl variables
    init();
    //start glut event loop
	glutMainLoop();*/
	return 0;
}

void outputfile()
{
  GLint a=numpoly;
  fprintf(fp1,"%d\n",a);
  for(int i=0;i<a;i++)
  {
    GLint b=v[i];
    fprintf(fp1,"%d\n",b);
    for(int j=0;j<b;j++)
    {
     GLfloat p,w;
     p=polyx[i][j];
     w=polyy[i][j];
     fprintf(fp1,"%f\t",p);
     fprintf(fp1,"%f\n",w);
    }
  }
}

void biezer_curve(int i,float t)
{
  for (int j=0;j<v[i];j++)
  {
   bx[i][0][j]=polyx[i][j];by[i][0][j]=polyy[i][j];
  }

 for(int j=1;j<v[i];j++)
 {
  for(int k=0;k<v[i]-j;k++)
  {
   bx[i][j][k]=(1-t)*bx[i][j-1][k]+t*bx[i][j-1][k+1];
   by[i][j][k]=(1-t)*by[i][j-1][k]+t*by[i][j-1][k+1];
  }
 }
 
 }
 
void addvertices(){
int pid,position,valuex,valuey;
printf("Enter the polygon id to whom vertex needed to be added\n");
scanf("%d",&pid);
   printf("Enter the location where you wish to insert an element\n");
   scanf("%d", &position);
 
   printf("Enter the value in x to be inserted\n");
   scanf("%d", &valuex);
   printf("Enter the value in y to be inserted\n");
   scanf("%d", &valuey);
 int c;
   for (c = v[pid] - 1; c >= position - 1; c--)
      polyx[pid][c+1] = polyx[pid][c];
 
   polyx[pid][position-1] = valuex;
  
     for (c = v[pid] - 1; c >= position - 1; c--)
      polyy[pid][c+1] = polyy[pid][c];
 
   polyy[pid][position-1] = valuey;
   v[pid]++; 
}
void deletevertices(){
int pid,position,valuex,valuey;
printf("Enter the polygon id to whom vertex needed to be added\n");
scanf("%d",&pid);
   printf("Enter the location where you wish to delete an element\n");
   scanf("%d", &position);
 for(int i=position-1;i<v[pid]-1;i++)
 {
  polyx[pid][i]=polyx[pid][i+1];
  polyy[pid][i]=polyy[pid][i+1];
 }
 v[pid]--;
}
void addpoly(){
printf("Your polygon id for this polygon will be %d\n",numpoly);
numpoly++;
printf("Enter the number of vertex point for this newpolygon\n");
int c;
scanf("%d",&c);
v[numpoly-1]=c;
printf("Enter the corrosponding vertex values\n");
for(int i=0;i<c;i++)
{
 printf("Enter the value of %d vertex x coordinate\n",i+1);
 scanf("%f",&polyx[numpoly-1][i]);
 printf("Enter the value of %d vertex y coordinate\n",i+1);
 scanf("%f",&polyy[numpoly-1][i]);
}
}
void replace(){
int pid,position,valuex,valuey;
 printf("Enter the polygon id which you need to replace the value\n");
 scanf("%d",&pid);
 printf("Enter the position for which the value to be replaced\n");
    scanf("%d", &position);
 
   printf("Enter the value in x to be inserted\n");
   scanf("%d", &valuex);
   printf("Enter the value in y to be inserted\n");
   scanf("%d", &valuey);
 polyx[pid][position-1]=valuex;
 polyy[pid][position-1]=valuey;
}
void biezer_curve1(int numb,int i,int order,float t)
{
  for (int j=i;j<i+order;j++)
  {
   bx[numb][0][j]=polyx[numb][j];
   by[numb][0][j]=polyy[numb][j];
  }
  
 for(int j=1;j<order;j++)
 {
  for(int k=0;k<order-j;k++)
  {
   bx[numb][j][k]=(1-t)*bx[numb][j-1][k]+t*bx[numb][j-1][k+1];
   by[numb][j][k]=(1-t)*by[numb][j-1][k]+t*by[numb][j-1][k+1];
  }
 }
 
 
}
void B_curve(int numb,int order){
 for(int i=0;i<v[numb]-order;i=i++)
 {
     //printf("%f,%f    %f,%f\n",)

         glVertex2f(polyx[numb][i],polyy[numb][i]);
         glVertex2f(polyx[numb][i+1],polyy[numb][i+1]);
   
   
 
    float initialx=polyx[numb][i],initialy=polyy[numb][i];
    for(float t=0;t<=1;t=t+1/resolution)
    {
     biezer_curve1(numb,i,order,t);
     glVertex2f(initialx,initialy);
     glVertex2f(bx[numb][order-1][0],by[numb][order-1][0]);
     
     //countt=1; 
     printf("\n\n%f,%f   %f,%f\n\n",initialx,initialy,bx[i][v[i]-1][0],by[i][v[i]-1][0]);
     
     initialx=bx[numb][order-1][0];
     initialy=by[numb][order-1][0]; 
    }
   
     
 }

 //return 0;
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
void display()
{
    //clears the screen
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();

    //draw_liness1();
    if(f==0)
    {
      //printf("Enter the resolution for the B-Spline curve you want\n");
      //scanf("%d",&resolution);
     draw_lines();
    }
    if(f==1)
    {int gh;
      //printf("Enter the resolution for the B-Spline curve you want\n");
      //scanf("%d",&resolution);
      printf("Enter the order of B-Spline curve you want\n");
      scanf("%d",&gh);
      int pid;
      printf("Enter the polygon Id you want to apply B-Spline\n");
      scanf("%d",&pid);
      BB_Spline(gh,pid);
    }
    //drawScene();
    //blits the current opengl framebuffer on the screen
   glutSwapBuffers();
   printf("Enter 1 to add the number of vertices to polygon\n");
   printf("Enter 2 to delete vertices from the polygon\n");
   printf("Enter 3 to Add new polygon\n");
   printf("Enter 4 to Replace the value at the particular vertices in polygon\n");
   printf("Enter 5 to display Bspline\n");
   printf("Enter 6 to display Beizer curve\n"); 
   printf("Enter -1 if you want to exit\n");
   int p;
   scanf("%d",&p);
   switch(p)
   {
   case 1:
    addvertices();
   break;
   case 2:
    deletevertices();
   break;
   case 3:
   addpoly();
   break;
   case 4:
   replace();
   break;
   case 5:
   f=1;
   break;
   case 6:
   f=0;
   break;
   case -1:
   outputfile();
   exit(0);
   
   }
    //checks for opengl errors
	check();
}/*
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}*/

void draw_lines(){
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();
  glLineWidth(2.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0);
     
      for(int i=0;i<numpoly;i++)
      {
       for(int j=0;j<v[i]-1;j++)
       {
   //    printf("point on the line %d,%d,%d,%d",polyx[i],polyy[i],polyx[i+1],polyy[i+1]);
         glVertex2f(polyx[i][j],polyy[i][j]);
         glVertex2f(polyx[i][j+1],polyy[i][j+1]);
       }
     // printf("point on the line %d,%d,%d,%d",polyx[v-1],polyy[v-1],polyx[0],polyy[0]);
       //bresenham(polyx[i][v[i]-1],polyy[i][v[i]-1],polyx[i][0],polyy[i][0]);
      } 
  
   
  for(int i=0;i<numpoly;i++)
   {
    float initialx=polyx[i][0],initialy=polyy[i][0];
    for(float t=0;t<=1;t=t+1/resolution)
    {
     biezer_curve(i,t);
     glVertex2f(initialx,initialy);
     glVertex2f(bx[i][v[i]-1][0],by[i][v[i]-1][0]);
     
     //countt=1; 
     //printf("\n\n%f,%f   %f,%f\n\n",initialx,initialy,bx[i][v[i]-1][0],by[i][v[i]-1][0]);
     
     initialx=bx[i][v[i]-1][0];
     initialy=by[i][v[i]-1][0]; 
    }
   }
        
    glEnd();
    glLineWidth(2.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
   /* glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0);
   for(int i=0;i<v[0]-1;i=i++)
 {
         glVertex2f(polyx[0][i],polyy[0][i]);
         glVertex2f(polyx[0][i+1],polyy[0][i+1]);
     
 }  
    glEnd();
   */
 
}

void draw_liness1()
{
   glLineWidth(2.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0);
     int numb=0;
     int order=3;     
  /*     for(int j=0;j<v[i]-1;j++)
       {
   //    printf("point on the line %d,%d,%d,%d",polyx[i],polyy[i],polyx[i+1],polyy[i+1]);
         glVertex2f(polyx[i][j],polyy[i][j]);
         glVertex2f(polyx[i][j+1],polyy[i][j+1]);
       }*/
  int v1=2;     
 for(int i=0;i<v[numb]-order-1;i=i++)
 {
     //printf("%f,%f    %f,%f\n",)

         glVertex2f(polyx[numb][i],polyy[numb][i]);
         glVertex2f(polyx[numb][i+1],polyy[numb][i+1]);
   
    float initialx=polyx[numb][i],initialy=polyy[numb][i];
    for(float t=0;t<=1;t=t+1/resolution)
    {
     biezer_curve1(numb,i,order,t);
     glVertex2f(initialx,initialy);
     glVertex2f(bx[numb][order-1][0],by[numb][order-1][0]);
     
     //countt=1; 
     printf("\n\n%f,%f   %f,%f\n\n",initialx,initialy,bx[numb][order-1][0],by[numb][order-1][0]);
     
     initialx=bx[numb][order-1][0];
     initialy=by[numb][order-1][0]; 
    }
   if(v1!=0)
   {
    v1--;
   }
   else
   {
    break;
   }
 }
 glEnd();
}

void reshape(int width, int height)
{
	/*set up projection matrix to define the view port*/
  
    //update the ne window width and height
/*	win_width = width;
	win_height = height;
    
    //creates a rendering area across the window
	glViewport(0,0,width,height);

    // uses an orthogonal projection matrix so that
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //you guys should look up this function
	glOrtho(0,100,0.0,100,-10,10);
    
    //clear the modelview matrix
    //the ModelView Matrix can be used in this project, to change the view on the projection
    //but you can also leave it alone and deal with changing the projection to a different view
    //for project 2, do not use the modelview matrix to transform the actual geometry, as you won't
    //be able to save the results
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //check for opengl errors
    check();*/    
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, 150.0, 0, 150.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
      
}

//gets called when a key is pressed on the keyboard
void key(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:	
         exit(0);
         break;
      case ' ':
         if (selectedKnot < 8) selectedKnot++; 
		 else selectedKnot = 0;
         glutPostRedisplay();
		 break;
      case 127:	
         resetKnots();
		 glutPostRedisplay();
         break;
      default:
         break;
  }
}
	/*switch(ch)
	{
		default:
            //prints out which key the user hit
            printf("User hit the \"%c\" key\n",ch);
			break;
	}
    //redraw the scene after keyboard input
	glutPostRedisplay();*/



//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    //print the pixel location, and the grid location
    printf ("MOUSE AT PIXEL: %d %d, GRID: %d %d\n",x,y,(int)(x/pixel_size),(int)((win_height-y)/pixel_size));
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
