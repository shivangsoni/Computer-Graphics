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
#include<time.h>
//other includes
#include <stdio.h>
#include <stdlib.h>
#include<cmath>
float dx,dy,m,xin,yin;
  int v;
  int c;  
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  float z;
  float inter[100];
  float step;
struct poly
{
  float x;
  float y;
  void read();
  void display();
  void ints(float);
  void sort(int);
}p[100];

void poly:: read()
{
 
 int i;
 printf("Enter the vertices of polygon\n");
 scanf("%d",&v);
 if(v>2)
 {
   for(int i=0;i<v;i++)
   {
    printf("Enter the x coordinate\n");
     scanf("%f",&p[i].x);
    //cin>>p[i].x;
      printf("Enter the y coordinate\n");
      scanf("%f",&p[i].x);
   // cin>>p[i].y;
   }
   xmin=xmax=p[0].x;
   ymin=ymax=p[0].y;
 }
}
void draw_pix(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(.2,.2,1.0);
    glVertex3f(x+.5,y+.5,0);
    glEnd();
}
void poly::calcs()
{
 for(int i=0;i<v;i++)
 {
  if(xmin>p[i].x)
  {
   xmin=p[i].x;
  }
    if(xmax<p[i].x)
  {
   xmax=p[i].x;
  }
  if(ymin>p[i].y)
  {
   ymin=p[i].y;
  }
    if(ymax<p[i].y)
  {
   ymax=p[i].y;
  }
 } 
}
void poly::display()
{
 float s,s2;
 s=ymin+0.5;
  while(s<=ymax)
  {
    ints(s);
    sort(s);
    s++;
  }
}

void do_cal(float x1,float y1,float x2,float y2)
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
}//Draws a single "pixel" given the current grid 
void ints(float a)
{
  int x1,x2,y1,y2,temp;
  c=0;
  for(int i=0;i<100;i++)
  {
   inter[i]=0;
  }
  for(int i=0;i<v;i++)
  {
   x1=p[i].x;
   y1=p[i].y;
   x2=p[i+1].x;
   y2=p[i+1].y;
   
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
      x=(x2-x1)/(z-y1)*(y2-y1);
      x=x+x1;
     }
     if(x<=xmax && x>=xmin)
     {
      inter[c++]=x;
     }
   }  
  } 
}
void sort()
{
 int temp,i,j;
  for(i=0;i<v;i++)
  {
   do_cal(p[i].x,p[i].y,p[i+1].x,p[i+1].y);
  }
  
  for(i=0;i<c;i+=2)
  {
   do_cal(inter[i],z,inter[i+1],z);
  }
}
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

int main()
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
 int c1;
 poly x;
 x.read();
 x.calc();
 //cleardevice();
 //setcolor(1);
 x.display();
     glutInitWindowSize(win_width,win_height);
    //windown title is "glut demo"
	glutCreateWindow("glut demo");
	    
    //initialize opengl variables
    init();
    //start glut event loop
	glutMainLoop();
	return 0;
}
