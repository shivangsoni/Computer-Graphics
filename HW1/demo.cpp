#ifdef WIN32
#include <windows.h>
#endif
#include<iostream>
#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <GL/glut.h>
#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include<time.h>
//other includes
#include <stdio.h>
#include <stdlib.h>
#include<cmath>

/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;
float polyxnew[100][100];
float polyynew[100][100];
float smaller=0;
float greater=0;
int vernew=0;
//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;
int codes(float x,float y);
/*Window information*/
int win_height;
int win_width;
float scalex;
float scaley;
int v[10];
float xcentroid;
float ycentroid;
void bresenham(float,float,float,float);
typedef unsigned int outcode;
enum{TOP=0x1,BOTTOM=0x2,RIGHT=0x4,LEFT=0x8};
void outputfile();
int inter[100][100];
float polyx[10][100];
float polyy[10][100];
int numpoly;
void init();
void Translation();
void idle();
int rastid;
int rast[10];
void display();
void draw_pix(int x, int y);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void Scaling();
void CSLCAD(float x0,float y0,float x1,float y1,float xmin,float xmax,float ymin,float ymax, int id);
void rasterize(int a);
outcode CompOutCode(float ,float ,float ,float ,float ,float );
void do_cal(float,float,float,float);
void do_cal2(float,float,float,float);
void rotation();
float degree;
float x,y;
float x1,x2;
//float y1,y2;
float x3,x4;
float y3,y4;
float dx,dy;
float step;
float m;
float xin,yin;
int q=0;
int c;
char algo;
void sort(int,int);
void display1(int);
void ints(int,float);
void calcs(int);
void clipping();
int xmin,xmax,ymin,ymax;
float tx,ty;
char cc;
int count=0;
FILE *fp1;
void waitFor(unsigned int);
int grid_matrix[100][100];
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
    for(int i=0;i<10;i++)
    {
     rast[i]=0;
    }
    //initialize opengl variables
    init();
    //start glut event loop
	glutMainLoop();
	return 0;
}
//do_cal2
void calcs(int ras)
{

 for(int i=0;i<v[ras-1];i++)
 {
  if(xmin>polyx[ras-1][i])
  {
   xmin=polyx[ras-1][i];
  }
    if(xmax<polyx[ras-1][i])
  {
   xmax=polyx[ras-1][i];
  }
   if(ymin>polyy[ras-1][i])
   {
   ymin=polyy[ras-1][i];
   }
   if(ymax<polyy[ras-1][i])
   {
   ymax=polyy[ras-1][i];
   }
  } 
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

int codes(float x,float y)
{
    int c=0;
    if(y>ymax)c=8;
    if(y<ymin)c=4;
    if(x>xmax)c=c|2;
    if(x<xmin)c=c|1;
    return c;
}

void clipping()
{
 printf("Enter the view port coordinates values\n");
 printf("Enter xmin and ymin respectively\n");
 scanf("%d",&xmin);
 scanf("%d",&ymin);
 printf("Enter xmin and ymax respectively\n");
 scanf("%d",&xmax);
 scanf("%d",&ymax);
 //float xd1,xd2,yd1,yd2;
 
 float polyxnewnew[100][100];
 float polyynewnew[100][100];
 for(int i=0;i<100;i++)
 {
  for(int j=0;j<100;j++)
  {
   polyxnew[i][j]=0;
   polyynew[i][j]=0;
   polyxnewnew[i][j]=0;
   polyynewnew[i][j]=0;
  }
 }
 for(int i=0;i<numpoly;i++)
 {
  for(int j=0;j<v[i]-1;j++)
  {
   CSLCAD(polyx[i][j],polyy[i][j],polyx[i][j+1],polyy[i][j+1],xmin,xmax,ymin,xmax,i);
  }
   CSLCAD(polyx[i][v[i]-1],polyy[i][v[i]-1],polyx[i][0],polyy[i][0],xmin,xmax,ymin,xmax,i);
  //v[i]=vernew;
  //vernew=0;
  int poo=0;
  for(int k=0;k<vernew;k++)
  {
   if(polyxnew[i][k] == 0 || polyynew[i][k] == 0)
   {
    
   }
   else
   {
      polyxnewnew[i][poo]=polyxnew[i][k];
      polyynewnew[i][poo]=polyynew[i][k];
      poo++;
   }
  }
  v[i]=poo;
  if(v[i]==0)
  {
   continue;
  }
  for(int p=0;p<poo;p++)
  {
   polyx[i][p]=polyxnewnew[i][p];
   polyy[i][p]=polyynewnew[i][p];
  }
 }
}

void CSLCAD(float x0,float y0,float x1,float y1,float xmin,float xmax,float ymin,float ymax,int id)
{
    outcode outcode0,outcode1,outcodeout;
    bool accept=false, done=false;
    outcode0=CompOutCode(x0,y0,xmin,xmax,ymin,ymax);
    outcode1=CompOutCode(x1,y1,xmin,xmax,ymin,ymax);
    //cout<<"outcode0="<<outcode0<<endl;
    //cout<<"outcode1="<<outcode1<<endl;
    printf("outcode0=%d",outcode0);
    printf("outcode1=%d",outcode1);
    do
    {
        if(outcode0==0 && outcode1==0)
        {
            accept=true;
            done=true;
        }
        else if(outcode0 & outcode1)
            {
                done=true;
            }
            else
            {
                float x,y;
                int ocd=outcode0 ? outcode0:outcode1;
                if(ocd & TOP)
                {
                    x=x0+(x1-x0)*(ymax-y0)/(y1-y0);
                    y=ymax;
                }
                else if(ocd & BOTTOM)
                {
                    x=x0+(x1-x0)*(ymin-y0)/(y1-y0);
                    y=ymin;
                }
                else if(ocd & LEFT)
                {
                    y=y0+(y1-y0)*(xmin-x0)/(x1-x0);
                    x=xmin;
                }
                else
                {
                    y=y0+(y1-y0)*(xmax-x0)/(x1-x0);
                    x=xmax;
                }
                if(ocd==outcode0)
                {
                    x0=x;
                    y0=y;
                    outcode0=CompOutCode(x0,y0,xmin,xmax,ymin,ymax);
                }
                else
                {
                    x1=x;
                    y1=y;
                    outcode1=CompOutCode(x1,y1,xmin,xmax,ymin,ymax);
                }
            }
    }while(done==false);

    if(accept==true)
    {
         //line(x0,y0,x1,y1);
         if(x0==0)
         {

         }
         else if(y0==0)
         {
         
         }
         else
         {
          polyxnew[id][vernew]=x0;
          polyynew[id][vernew]=y0;
          vernew++;
         // printf("\nx0 is %d and y0 is %d\n",x0,y0);
         }
         
         
         if(x1==0)
         {

         }
         else if(y1==0)
         {
         
         }
         else
         {
          polyxnew[id][vernew]=x1;
          polyxnew[id][vernew]=y1;
          vernew++;
          //printf("\nx1 is %d and y1 is %d\n",x1,y1);
         }
         // bresenham(x0,y0,x1,y1);                
    }
}

outcode CompOutCode(float x,float y,float xmin,float xmax,float ymin,float ymax)
{
    outcode code=0;
    if(y>ymax)
        code|=TOP;
    if(y<ymin)
        code|=BOTTOM;
    if(x>xmax)
        code|=RIGHT;
    if(x<xmin)
        code|=LEFT;
    return code;
}

void do_cal2(float x1,float y1,float x2,float y2)
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
      grid_matrix[(int)x][(int)y]=1;
      y=y-1;
      //printf("This is x %d",x);
      //printf("This is y %d",y);
    }
    }
    else if(dx == 0 && (y2-y1)>0)
    {
     for(int k=0;k<=step;k++)
    {
      grid_matrix[(int)x][(int)y]=1;
      y=y+1;
      //printf("This is x %d",x);
      //printf("This is y %d",y);
    }
    }
    else if(m>0 && m<1)
    {
    for(int k=0;k<=step;k++)
    {
      grid_matrix[(int)x][(int)y]=1;
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
      grid_matrix[(int)x][(int)y]=1;
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
      grid_matrix[(int)x][(int)y]=1;
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
      grid_matrix[(int)x][(int)y]=1;
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
      grid_matrix[(int)x][(int)y]=1;
      x=x+1;
      y=y;
     }
    }
    else if(dx==0 && dy==0)
    {
     grid_matrix[(int)x][(int)y]=1;
    }
    else if(m==1)
    {
     for(int k=0;k<=step;k++)
     {
     grid_matrix[(int)x][(int)y]=1;
      x=x+1;
      y=y+1;
     }
    }
    else if(m == -1)
    {
     for(int k=0;k<=step;k++)
     {
     grid_matrix[(int)x][(int)y]=1;
      x=x+1;
      y=y-1;
     }
    }
   else
    {
      for(int k=0;k<=step;k++)
     {
      grid_matrix[(int)x][(int)y]=1;
      x=x;
      y=y+1;
     }
    }
    
    
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
	

    //draws every other pixel on the screen
  /*  for (int j = 0; j < grid_height; j+=2){
        for (int i = 0; i < grid_width; i+=2){
            //this is the only "rendering call you should make in project 1"
            draw_pix(i,j);
        }
    }*/
       for(int i=0;i<numpoly;i++)
      {
       for(int j=0;j<v[i]-1;j++)
       {
   //    printf("point on the line %d,%d,%d,%d",polyx[i],polyy[i],polyx[i+1],polyy[i+1]);
         bresenham(polyx[i][j],polyy[i][j],polyx[i][j+1],polyy[i][j+1]);
       }
     // printf("point on the line %d,%d,%d,%d",polyx[v-1],polyy[v-1],polyx[0],polyy[0]);
       bresenham(polyx[i][v[i]-1],polyy[i][v[i]-1],polyx[i][0],polyy[i][0]);
       }
       rastid=0;
        for(int p=0;p<q;p++)
         rasterize(rast[p]); 
      // draw_pix(polyx[0],polyy[0]);

     //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
     // do_cal(x4,y4,x1,y1);
      //rasterize();
      int choice;
      printf("Please enter the operation you want to perform\n");
       printf("Enter 1 if you want to perform Rotation\n");
       printf("Enter 2 if you want to perform Scaling\n");
       printf("Enter 3 if you want to perform Translation\n");
       printf("Enter 4 if you want to rasterize particular polygon\n");
       printf("Enter 5 for display polygon through dda as default is Bresenham\n");
       printf("Enter 6 if you want to do clipping polygon\n");
       printf("Enter any other key to exit\n");
      scanf("%d",&choice);
      switch(choice)
      {
       case 1:
        rotation();
        break;
       case 2:
        Scaling();
        break;
       case 3:
        Translation();
        break;
       case 4:
        rastid=0;
        printf("Enter rast id\n");
        scanf("%d",&rastid);
        rast[q++]=rastid;
        rasterize(rastid);
        break;
       case 5:
       for(int i=0;i<numpoly;i++)
      {
      for(int j=0;j<v[i]-1;j++)
      {
   //    printf("point on the line %d,%d,%d,%d",polyx[i],polyy[i],polyx[i+1],polyy[i+1]);
       bresenham(polyx[i][j],polyy[i][j],polyx[i][j+1],polyy[i][j+1]);
      }
     // printf("point on the line %d,%d,%d,%d",polyx[v-1],polyy[v-1],polyx[0],polyy[0]);
       bresenham(polyx[i][v[i]-1],polyy[i][v[i]-1],polyx[i][0],polyy[i][0]);
      }
       waitFor(5);      
       break;
       case 6:
       clipping();
       break;
       default: 
       printf("Your choice is inappropiate\n");
       outputfile();
       exit(0);
      }
    //checks for opengl errors
	check();
}

void Translation()
{
     //clears the screen
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();
       int tid;
            printf("Which polygon do you want to do Translation to\n");
            scanf("%d",&tid);
     
	printf("Enter the number to be translated in x direction\n");
	scanf("%f",&tx);
	printf("Enter the number to be translated in y direction\n");
	scanf("%f",&ty);

     //draw_pix(polyx[0]+tx,polyy[0]+ty);
       for(int i=0;i<v[tid-1]-1;i++)
      {
        
       bresenham(polyx[tid-1][i]+tx,polyy[tid-1][i]+ty,polyx[tid-1][i+1]+tx,polyy[tid-1][i+1]+ty);
      }
      bresenham(polyx[tid-1][v[tid-1]-1]+tx,polyy[tid-1][v[tid-1]-1]+ty,polyx[tid-1][0]+tx,polyy[tid-1][0]+ty);
      for(int i=0;i<v[tid-1];i++)
      {
       polyx[tid-1][i]=polyx[tid-1][i]+tx;
       polyy[tid-1][i]=polyy[tid-1][i]+ty;
      }
}
//DDA
void do_cal(float x1,float y1,float x2,float y2)
{
    float dX,dY,iSteps;
    float xInc,yInc,iCount,x,y;

    dX = x1 - x2;
    dY = y1 - y2;

    if (fabs(dX) > fabs(dY))
    {
        iSteps = fabs(dX);
    }
    else
    {
        iSteps = fabs(dY);
    }

    xInc = dX/iSteps;
    yInc = dY/iSteps;

    x = x1;
    y = y1;
    for (iCount=1; iCount<=iSteps; iCount++)
    {
        draw_pix(x,y);
        x -= xInc;
        y -= yInc;
    }
    //circle(x,y,1);
}

void bresenham (float x0, float y0, float x1, float y1)  
  {  
  /* glLineWidth(2.5);
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_LINES);
//glVertex3f(0.0, 10.0, 4.0);
//glVertex3f(10.0, 0, 5);

     //     printf("Coordinates are \n");
     //   printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint1[j]-1],polyy[i][vertexpoint1[j]-1],polyz[i][vertexpoint1[j]-1]);
     //  printf("Point %d has %f%f%f\n",j,polyx[i][vertexpoint2[j]-1], polyy[i][vertexpoint2[j]-1], polyz[i][vertexpoint2[j]-1]);
      glVertex2f(x0,y0);
      glVertex2f(x1,y1);

 glEnd();*/
  
  float dx = abs (x1 - x0);  
  float dy = abs (y1 - y0);  
  float x, y;  
  if (dx >= dy)  
  {  
   float d = 2*dy-dx;  
   float ds = 2*dy;  
   float dt = 2*(dy-dx);  
       if (x0 < x1)   
       {  
            x = x0;  
            y = y0;  
       }  
        else  
        {   
             x = x1;  
             y = y1;  
             x1 = x0;  
             y1 = y0;  
        }  
        draw_pix(x,y);  
   while (x < x1)  
   {  
        if (d < 0)  
        d += ds;  
        else {  
             if (y < y1) {  
              y++;  
              d += dt;  
             }   
             else {  
              y--;  
             d += dt;  
             }  
            }  
   x++;  
      draw_pix(x,y);  
       }  
       }  
       else {   
             int d = 2*dx-dy;  
             int ds = 2*dx;  
             int dt = 2*(dx-dy);  
            if (y0 < y1) {  
             x = x0;  
            y = y0;  
             }  
             else {   
             x = x1;  
             y = y1;  
             y1 = y0;  
            x1 = x0;  
             }  
           draw_pix(x,y);    
        while (y < y1)  
        {  
              if (d < 0)  
                 d += ds;  
              else {  
                      if (x > x1){  
                      x--;  
                      d += dt;  
                   } else {  
                      x++;  
                      d += dt;  
                   }  
              }  
              y++;  
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
void display1(int w)
{
 float s,s2;
 s=ymin+0.001;
  while(s<=ymax)
  {
    ints(w,s);
    sort(w,s);
    s++;
  }
}

void ints(int ras,float z)
{
  float x1,x2,y1,y2,temp;
  c=0;
  for(int i=0;i<100;i++)
  {
   for(int j=0;j<100;j++)
   {
    inter[i][j]=0;
   }
  }
  for(int i=0;i<v[ras-1]-1;i++)
  {
   x1=polyx[ras-1][i];
   y1=polyy[ras-1][i];
   x2=polyx[ras-1][i+1];
   y2=polyy[ras-1][i+1];
   
   if(y2<y1)
   {
    temp=y1;
    y1=y2;
    y2=temp;
    
    temp=x1;
    x1=x2;
    x2=temp;
   }
   
   if(z<=y2 && z>=y1)
   {
     if(y1==y2)
     {
       x=x1; 
     }
     else
     {
      x=(x2-x1)/(y2-y1)*(z-y1);
      x=x+x1;
      //printf("x after %f\n",x);
     }
     if(x<=xmax && x>=xmin)
     {
      inter[ras-1][c++]=x;
     }
   }  
  }
   x1=polyx[ras-1][v[ras-1]-1];
   y1=polyy[ras-1][v[ras-1]-1];
   x2=polyx[ras-1][0];
   y2=polyy[ras-1][0];
   if(y2<y1)
   {
    temp=y1;
    y1=y2;
    y2=temp;
    
    temp=x1;
    x1=x2;
    x2=temp;
   }
   if(z<=y2 && z>=y1)
   {
     if(y1==y2)
     {
        x=x1;
     }
     else
     {
      x=(x2-x1)/(y2-y1)*(z-y1);
      x=x+x1;
      //printf("Condition 2\n");
      //printf("%f\n",x);
     }
     if(x<=xmax && x>=xmin)
     {
      inter[ras-1][c++]=x;
     }
   }
}

void sort(int ras,int z)
{
 int temp,i,j;
  for(i=0;i<v[ras-1]-1;i++)
  {
   bresenham(polyx[ras-1][i],polyy[ras-1][i],polyx[ras-1][i+1],polyy[ras-1][i+1]);
  }
   bresenham(polyx[ras-1][v[ras-1]-1],polyy[ras-1][v[ras-1]-1],polyx[ras-1][0],polyy[ras-1][0]);
  for(i=0;i<c;i+=2)
  {
   bresenham(inter[ras-1][i],z,inter[ras-1][i+1],z);
  }
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
void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}
void rasterize(int ras)
{  if(ras!=0)
  {
   calcs(ras);
   display1(ras);
  }
}
void rotation()
{
 
 	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            //clears the opengl Modelview transformation matrix
	glLoadIdentity();
	int rotid;
	printf("Enter the polygon id what you want to rotate\n");
	scanf("%d",&rotid);
	printf("Enter the degree of rotation\n");
	scanf("%f",&degree);
	//degree=(3.14)/180*degree;
           
    degree=degree*(M_PI)/180;
    xcentroid=0;
    ycentroid=0;
    for(int i=0;i<v[rotid-1];i++)
    {
     xcentroid=xcentroid+polyx[rotid-1][i];
     ycentroid=ycentroid+polyy[rotid-1][i];
    }
    xcentroid=xcentroid/float(v[rotid-1]);
    ycentroid=ycentroid/float(v[rotid-1]);
    //degree=degree*(M_PI)/180;
    
   // printf("%f\n",sin(90*(M_PI)/180));
    //printf("%f\n",degree);//radian
   /*  draw_pix((cos(degree)*(polyx[0]-xcentroid))+xcentroid-((polyy[0]-ycentroid)*sin(degree)),(sin(degree)*(polyx[0]-xcentroid))+ycentroid+((polyy[0]-ycentroid)*cos(degree)));*/
    /*if(degree<0)
     {
      degree=degree+2*M_PI;
     }
    if(0<=degree && degree<=.785)
    {
     degree=2*M_PI-degree;
    }
    if(1.57<=degree && degree<=2.355)
    {
     degree=2*M_PI-degree;
    }
     printf("%f\n",degree);*/
      for(int i=0;i<v[rotid-1]-1;i++)
      {
      /* do_cal((cos(degree)*(polyx[i]-xcentroid))+xcentroid-((polyy[i]-ycentroid)*sin(degree)),(sin(degree)*(polyx[i]-xcentroid))+ycentroid+((polyy[i]-ycentroid)*cos(degree)),(cos(degree)*(polyx[i+1]-xcentroid))+xcentroid-((polyy[i+1]-ycentroid)*sin(degree)),(sin(degree)*(polyx[i+1]-xcentroid))+ycentroid+((polyy[i+1]-ycentroid)*cos(degree)));*/
      bresenham(cos(degree)*(polyx[rotid-1][i]-xcentroid)-sin(degree)*(polyy[rotid-1][i]-ycentroid)+xcentroid ,sin(degree)*(polyx[rotid-1][i]-xcentroid)+cos(degree)*(polyy[rotid-1][i]-ycentroid)+ycentroid,cos(degree)*(polyx[rotid-1][i+1]-xcentroid)-sin(degree)*(polyy[rotid-1][i+1]-ycentroid)+xcentroid ,sin(degree)*(polyx[rotid-1][i+1]-xcentroid)+cos(degree)*(polyy[rotid-1][i+1]-ycentroid)+ycentroid);
     // bresenham
      }
   /*do_cal((cos(degree)*(polyx[v-1]-xcentroid))+xcentroid-((polyy[v-1]-ycentroid)*sin(degree)),(sin(degree)*(polyx[v-1]-xcentroid))+ycentroid+((polyy[v-1]-ycentroid)*cos(degree)),(cos(degree)*(polyx[0]-xcentroid))+xcentroid-((polyy[0]-ycentroid)*sin(degree)),(sin(degree)*(polyx[0]-xcentroid))+ycentroid+((polyy[0]-ycentroid)*cos(degree)));*/
    bresenham(cos(degree)*(polyx[rotid-1][v[rotid-1]-1]-xcentroid)-sin(degree)*(polyy[rotid-1][v[rotid-1]-1]-ycentroid)+xcentroid ,sin(degree)*(polyx[rotid-1][v[rotid-1]-1]-xcentroid)+cos(degree)*(polyy[rotid-1][v[rotid-1]-1]-ycentroid)+ycentroid,cos(degree)*(polyx[rotid-1][0]-xcentroid)-sin(degree)*(polyy[rotid-1][0]-ycentroid)+xcentroid ,sin(degree)*(polyx[rotid-1][0]-xcentroid)+cos(degree)*(polyy[rotid-1][0]-ycentroid)+ycentroid);
      
   for(int i=0;i<v[rotid-1];i++)
   {
    float a=polyx[rotid-1][i];
    float b=polyy[rotid-1][i];
    polyx[rotid-1][i]=cos(degree)*(a-xcentroid)-sin(degree)*(b-ycentroid)+xcentroid;
    polyy[rotid-1][i]=sin(degree)*(a-xcentroid)+cos(degree)*(b-ycentroid)+ycentroid;
   }
}
void Scaling()
{
     //clears the screen
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();
	int scaleid;
	printf("Enter the polygon id you want to scale\n");
	scanf("%d",&scaleid);
	printf("Enter the factor to be scaled in x direction\n");
	scanf("%f",&scalex);
	printf("Enter the factor to be scaled in y direction\n");
	scanf("%f",&scaley);
    
    xcentroid=0;
    ycentroid=0;
    for(int i=0;i<v[scaleid-1];i++)
    {
     xcentroid=xcentroid+polyx[scaleid-1][i];
     ycentroid=ycentroid+polyy[scaleid-1][i];
    }
    xcentroid=xcentroid/float(v[scaleid-1]);
    ycentroid=ycentroid/float(v[scaleid-1]);
     //draw_pix(scalex*(polyx[scaleid-1][0]-xcentroid)+xcentroid,scaley*(polyy[scaleid-1][0]-ycentroid)+ycentroid);
       for(int i=0;i<v[scaleid-1]-1;i++)
      {
      bresenham(scalex*(polyx[scaleid-1][i]-xcentroid)+xcentroid,scaley*(polyy[scaleid-1][i]-ycentroid)+ycentroid,scalex*(polyx[scaleid-1][i+1]-xcentroid)+xcentroid,scaley*(polyy[scaleid-1][i+1]-ycentroid)+ycentroid);
      }
      bresenham(scalex*(polyx[scaleid-1][v[scaleid-1]-1]-xcentroid)+xcentroid,scaley*(polyy[scaleid-1][v[scaleid-1]-1]-ycentroid)+ycentroid,scalex*(polyx[scaleid-1][0]-xcentroid)+xcentroid,scaley*(polyy[scaleid-1][0]-ycentroid)+ycentroid);
      
      for(int i=0;i<v[scaleid-1];i++)
      {
       polyx[scaleid-1][i]=scalex*(polyx[scaleid-1][i]-xcentroid)+xcentroid;
       polyy[scaleid-1][i]=scaley*(polyy[scaleid-1][i]-ycentroid)+ycentroid;
      }
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
