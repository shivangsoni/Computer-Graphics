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
void call_Intensity();
void call_Intensity1();
void call_Intensity2();
float ymax=-1,ymin=1000,xmax=-1,xmin=1000;
int c=0;
float zmin,zmax;
float x;
int cc=0;
float power(float ,int);
void do_cal(float,float,float,float);
float inter[100][100];
float inter1[100][100];
float inter2[100][100];
void calcs(int f,int i);
void bresenham (float x0, float y0, float x1, float y1);
void sort(int ras,int z);

void painter_algo();
void ints(int ras,float z);
void ints1(int ras,float z);
void ints2(int ras,float z);
void rasterize(int ras);
void display1(int w);
float norm[1000][1000][3][3];
float Inetnetcal[1000][1000][3][3];
void normal_calc();
void XYortho();
void XZortho();
void YZortho();
float zpainter[1000][1000];
float ypainter[1000][1000];
float xpainter[1000][1000];

/****set in main()****/
//the number of pixels in the grid

float xvertex1[1000];
float yvertex1[1000];
float zvertex1[1000];


float xvertex2[1000];
float yvertex2[1000];
float zvertex2[1000];

float xvertex3[1000];
float yvertex3[1000];
float zvertex3[1000];

float pvertex[10000];
int grid_width;
int grid_height;
float tx,ty,tz;
float Ianet[10000][3];
float Idnet[10000][3];
float Isnet[10000][3];
 void IntensityAmbient(int mat);
 void IntensityDiffuse(float a,float b,float c,float d,float e,float f,int g);
 void IntensitySpecular(float a,float b,float c,float d,float e, float f,int g); 
//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
void cross(float a1,float a2,float a3,float b1,float b2,float b3);
float vcrossx,vcrossy,vcrossz;
//light.txt
float Lx=0,Ly=0,Lz=0;
float Iax=0,Iay=0,Iaz=0;
float Ilx=0,Ily=0,Ilz=0;
float F_dist=0;
float K_dist=0;
float pixel_size;
float degree;
float ax,ay,az;
float bx,by,bz;
float l;
//Mat.txt
int no_of_mat;
int material[100];
float ka[100][3];
float kd[100][3];
float ks[100][3];
float nmat[100];
/*Window information*/
int win_height;
int win_width;
int numpoly;
float polyx[1000][1000];
float polyy[1000][1000];
float polyz[1000][1000];
int polyvertices[10000];
int vertex[10000];
float Inet[100][3];
void Inetcal(int a);
  //translate_arb(+ax,+ay,+az,rotid);
float xcentroid,ycentroid,zcentroid;
float scalex,scaley,scalez;
int vertexpoint1[100][100];
int vertexpoint2[100][100];
int vertexpoint3[100][100];
void init();
int zvp1[100][100];
int zvp2[100][100];
int zvp3[100][100];


int yvp1[100][100];
int yvp2[100][100];
int yvp3[100][100];


int xvp1[100][100];
int xvp2[100][100];
int xvp3[100][100];

void idle();
void display();
void draw_pix(float x, float y);
void draw_pix1(float x,float y,float p,float q, float r);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

void check();
void linee();
float dx,dy,dz,d,vx,vy,vz;
void outputVertices();

void XY_painter();
void XZ_painter();
void YZ_painter();


void flow();
void painter_algorithm()
{
 //While projecting in XY plane sort through Z distance 
 
 //The polygon which is far away from axis must be rasterze first
 XY_painter();
 XZ_painter();
 YZ_painter();
}


void XY_painter()
{
  //Sort the vertices of the polygon according to their z axis.
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {  float zminn=1;
      if(polyz[i][vertexpoint1[i][j]-1]<zminn)
      {
         zminn= polyz[i][vertexpoint1[i][j]-1];      
      }
      if(polyz[i][vertexpoint2[i][j]-1]<zminn)
      {
       zminn= polyz[i][vertexpoint2[i][j]-1];
      }
      if(polyz[i][vertexpoint3[i][j]-1]<zminn)
      {
       zminn= polyz[i][vertexpoint3[i][j]-1];
      }
      //printf("%f   %f  %f %f\n",polyz[i][vertexpoint1[i][j]-1],polyz[i][vertexpoint2[i][j]-1],polyz[i][vertexpoint3[i][j]-1],zminn);
      //printf("%f\n",zminn);
      zvp1[i][j]=vertexpoint1[i][j];
      zvp2[i][j]=vertexpoint2[i][j];
      zvp3[i][j]=vertexpoint3[i][j];
     
      zpainter[i][j]=zminn;   
   }
   printf("\n\n");
  }
  int g=0;
  //store the sorted z values
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     pvertex[g++]=zpainter[i][j];  
   }
  }
  // p vertx stores the min z value.
  int h=0,k=0,q=0;
  
  //vertex1x do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex1[h++]=polyx[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2x do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex2[h++]=polyx[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3x do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex3[h++]=polyx[i][vertexpoint3[i][j]-1];  
   }
  }
  h=0;
    //vertex1y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex1[h++]=polyy[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2y do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex2[h++]=polyy[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex3[h++]=polyy[i][vertexpoint3[i][j]-1];  
   }
  }
  h=0;
     for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex1[h++]=polyz[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2y do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex2[h++]=polyz[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex3[h++]=polyz[i][vertexpoint3[i][j]-1];  
   }
  }
  
  
  // sort with respect to z
  for(int i=0;i<h;i++)
  {
    for(int j=0;j<h-i-1;j++)
    {
      if(pvertex[j] < pvertex[j+1])
      {
        float swap=pvertex[j];
        pvertex[j]=pvertex[j+1];
        pvertex[j+1]=swap;
        
        swap=xvertex1[j];
        xvertex1[j]=xvertex1[j+1];
        xvertex1[j+1]=swap;
        
        swap=xvertex2[j];
        xvertex2[j]=xvertex2[j+1];
        xvertex2[j+1]=swap;
        
        swap=xvertex3[j];
        xvertex3[j]=xvertex3[j+1];
        xvertex3[j+1]=swap;
        
        
        
        
        
        swap=yvertex1[j];
        yvertex1[j]=yvertex1[j+1];
        yvertex1[j+1]=swap;
        
        swap=yvertex2[j];
        yvertex2[j]=yvertex2[j+1];
        yvertex2[j+1]=swap;
        
        swap=yvertex3[j];
        yvertex3[j]=yvertex3[j+1];
        yvertex3[j+1]=swap;
        
        
        swap=zvertex1[j];
        zvertex1[j]=zvertex1[j+1];
        zvertex1[j+1]=swap;
        
        swap=zvertex2[j];
        zvertex2[j]=zvertex2[j+1];
        zvertex2[j+1]=swap;
        
        swap=zvertex3[j];
        zvertex3[j]=zvertex3[j+1];
        zvertex3[j+1]=swap;
        
      }
    }
  }
  
  
  
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);
   
  float a=1;
  float b=0;
  float c=0;   
  for(int i=0;i<h;i++)
  {
      glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(a, b, c); 

      glVertex3f(xvertex1[i],yvertex1[i],0);

      glVertex3f(xvertex2[i],yvertex2[i],0);
      glVertex3f(xvertex3[i],yvertex3[i],0);
      glVertex3f(xvertex3[i],yvertex3[i],0);
      glEnd();
      glFlush();            
       a=a-0.010;
       b=b+0.010;
       c=c+0.015;
  }
  
  //sort the array
 /* for (int i=0; i<numpoly; i++)
  {
    for (int j= 0 ; j<vertex[i]; j++)
    {
      for(int k=0;k<vertex[i]-j-1;k++)
      {  if (zpainter[i][k] < zpainter[i][k+1]) 
          {
           float swap       = zpainter[i][k];
           zpainter[i][k] = zpainter[i][k+1];
           zpainter[i][k+1] = swap;
           
           
           swap=zvp1[i][k];
           zvp1[i][k]=zvp1[i][k+1];
           zvp1[i][k+1]=swap;
           
           swap=zvp2[i][k];
           zvp2[i][k]=zvp2[i][k+1];
           zvp2[i][k+1]=swap;
           
           swap=zvp3[i][k];
           zvp3[i][k]=zvp3[i][k+1];
           zvp3[i][k+1]=swap;     
          }
      }
    }
  }
  */
 /* for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     printf("%f  %d  %d  %d\n",zpainter[i][j],zvp1[i][j],zvp2[i][j],zvp3[i][j]);
   }
   //printf("\nNew Vertex\n");
  }
  
  
     glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);
   
   
  for(int i=0;i<numpoly;i++)
  {
  float a=1;
  float b=0;
  float c=0;
    for(int j=0;j<vertex[i];j++)
    {
      glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(a, b, c); 

      glVertex3f(polyx[i][zvp1[i][j]-1],polyy[i][zvp1[i][j]-1],0);

      glVertex3f(polyx[i][zvp2[i][j]-1],polyy[i][zvp2[i][j]-1],0);
      glVertex3f(polyx[i][zvp3[i][j]-1],polyy[i][zvp3[i][j]-1],0);
      glVertex3f(polyx[i][zvp3[i][j]-1],polyy[i][zvp3[i][j]-1],0);
      glEnd();
      glFlush();            
       a=a-0.015;
       b=b+0.015;
       c=c+0.015;
  }
  }*/
}

void XZ_painter()
{

for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {  float zminn=1;
      if(polyy[i][vertexpoint1[i][j]-1]<zminn)
      {
         zminn= polyy[i][vertexpoint1[i][j]-1];      
      }
      if(polyy[i][vertexpoint2[i][j]-1]<zminn)
      {
       zminn= polyy[i][vertexpoint2[i][j]-1];
      }
      if(polyy[i][vertexpoint3[i][j]-1]<zminn)
      {
       zminn= polyy[i][vertexpoint3[i][j]-1];
      }
  //printf("%f   %f  %f %f\n",polyz[i][vertexpoint1[i][j]-1],polyz[i][vertexpoint2[i][j]-1],polyz[i][vertexpoint3[i][j]-1],zminn);
      //printf("%f\n",zminn);
      yvp1[i][j]=vertexpoint1[i][j];
      yvp2[i][j]=vertexpoint2[i][j];
      yvp3[i][j]=vertexpoint3[i][j];
      
      ypainter[i][j]=zminn;   
   }
   printf("\n\n");
  }
 

  


  int g=0;
  //store the sorted y values
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     pvertex[g++]=ypainter[i][j];  
   }
  }
  
  // p vertx stores the min y value.
  
  int h=0,k=0,q=0;
  
  //vertex1x do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex1[h++]=polyx[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2x do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex2[h++]=polyx[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3x do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex3[h++]=polyx[i][vertexpoint3[i][j]-1];  
   }
  }
  h=0;
    //vertex1y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex1[h++]=polyy[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2y do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex2[h++]=polyy[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex3[h++]=polyy[i][vertexpoint3[i][j]-1];  
   }
  }
  h=0;
     for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex1[h++]=polyz[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2y do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex2[h++]=polyz[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex3[h++]=polyz[i][vertexpoint3[i][j]-1];  
   }
  }
  
  
  // sort with respect to z
  for(int i=0;i<h;i++)
  {
    for(int j=0;j<h-i-1;j++)
    {
      if(pvertex[j] < pvertex[j+1])
      {
        float swap=pvertex[j];
        pvertex[j]=pvertex[j+1];
        pvertex[j+1]=swap;
        
        swap=xvertex1[j];
        xvertex1[j]=xvertex1[j+1];
        xvertex1[j+1]=swap;
        
        swap=xvertex2[j];
        xvertex2[j]=xvertex2[j+1];
        xvertex2[j+1]=swap;
        
        swap=xvertex3[j];
        xvertex3[j]=xvertex3[j+1];
        xvertex3[j+1]=swap;
        
        
        
        
        
        swap=yvertex1[j];
        yvertex1[j]=yvertex1[j+1];
        yvertex1[j+1]=swap;
        
        swap=yvertex2[j];
        yvertex2[j]=yvertex2[j+1];
        yvertex2[j+1]=swap;
        
        swap=yvertex3[j];
        yvertex3[j]=yvertex3[j+1];
        yvertex3[j+1]=swap;
        
        
        swap=zvertex1[j];
        zvertex1[j]=zvertex1[j+1];
        zvertex1[j+1]=swap;
        
        swap=zvertex2[j];
        zvertex2[j]=zvertex2[j+1];
        zvertex2[j+1]=swap;
        
        swap=zvertex3[j];
        zvertex3[j]=zvertex3[j+1];
        zvertex3[j+1]=swap;
        
      }
    }
  }
  
  
  
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
 //  glClear(GL_COLOR_BUFFER_BIT);
   
  float a=1;
  float b=0;
  float c=0;   
  for(int i=0;i<h;i++)
  {
      glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(a, b, c); 

      glVertex3f(xvertex1[i],zvertex1[i]+1,0);

      glVertex3f(xvertex2[i],zvertex2[i]+1,0);
      glVertex3f(xvertex3[i],zvertex3[i]+1,0);
      glVertex3f(xvertex3[i],zvertex3[i]+1,0);
      glEnd();
      glFlush();            
       a=a-0.008;
       b=b+0.008;
       c=c+0.008;
  }

}


void YZ_painter()
{

for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {  float zminn=1;
      if(polyx[i][vertexpoint1[i][j]-1]<zminn)
      {
         zminn= polyx[i][vertexpoint1[i][j]-1];      
      }
      if(polyx[i][vertexpoint2[i][j]-1]<zminn)
      {
       zminn= polyx[i][vertexpoint2[i][j]-1];
      }
      if(polyx[i][vertexpoint3[i][j]-1]<zminn)
      {
       zminn= polyx[i][vertexpoint3[i][j]-1];
      }
  //printf("%f   %f  %f %f\n",polyz[i][vertexpoint1[i][j]-1],polyz[i][vertexpoint2[i][j]-1],polyz[i][vertexpoint3[i][j]-1],zminn);
      //printf("%f\n",zminn);
      xvp1[i][j]=vertexpoint1[i][j];
      xvp2[i][j]=vertexpoint2[i][j];
      xvp3[i][j]=vertexpoint3[i][j];
      
      xpainter[i][j]=zminn;   
   }
   printf("\n\n");
  }
 
  //sort the array
  /*for (int i=0; i<numpoly; i++)
  {
    for (int j= 0 ; j<vertex[i]; j++)
    {
      for(int k=0;k<vertex[i]-j-1;k++)
      {  if (xpainter[i][k] < xpainter[i][k+1]) 
          {
           float swap       = xpainter[i][k];
           xpainter[i][k] = xpainter[i][k+1];
           xpainter[i][k+1] = swap;
           
           
           swap=xvp1[i][k];
           xvp1[i][k]=xvp1[i][k+1];
           xvp1[i][k+1]=swap;
           
           swap=xvp2[i][k];
           xvp2[i][k]=xvp2[i][k+1];
           xvp2[i][k+1]=swap;
           
           swap=xvp3[i][k];
           xvp3[i][k]=xvp3[i][k+1];
           xvp3[i][k+1]=swap;     
          }
      }
    }
  }*/
  
  
  
  
  
   int g=0;
  //store the sorted y values
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     pvertex[g++]=xpainter[i][j];  
   }
  }
  
  // p vertx stores the min y value.
  
  int h=0,k=0,q=0;
  
  //vertex1x do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex1[h++]=polyx[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2x do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex2[h++]=polyx[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3x do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     xvertex3[h++]=polyx[i][vertexpoint3[i][j]-1];  
   }
  }
  h=0;
    //vertex1y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex1[h++]=polyy[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2y do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex2[h++]=polyy[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     yvertex3[h++]=polyy[i][vertexpoint3[i][j]-1];  
   }
  }
  h=0;
     for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex1[h++]=polyz[i][vertexpoint1[i][j]-1];  
   }
  }
  h=0;
  //vertex2y do
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex2[h++]=polyz[i][vertexpoint2[i][j]-1];  
   }
  }
  h=0;
  //vertex3y do
   for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
     zvertex3[h++]=polyz[i][vertexpoint3[i][j]-1];  
   }
  }
  
  
  // sort with respect to z
  for(int i=0;i<h;i++)
  {
    for(int j=0;j<h-i-1;j++)
    {
      if(pvertex[j] < pvertex[j+1])
      {
        float swap=pvertex[j];
        pvertex[j]=pvertex[j+1];
        pvertex[j+1]=swap;
        
        swap=xvertex1[j];
        xvertex1[j]=xvertex1[j+1];
        xvertex1[j+1]=swap;
        
        swap=xvertex2[j];
        xvertex2[j]=xvertex2[j+1];
        xvertex2[j+1]=swap;
        
        swap=xvertex3[j];
        xvertex3[j]=xvertex3[j+1];
        xvertex3[j+1]=swap;
        
        
        
        
        
        swap=yvertex1[j];
        yvertex1[j]=yvertex1[j+1];
        yvertex1[j+1]=swap;
        
        swap=yvertex2[j];
        yvertex2[j]=yvertex2[j+1];
        yvertex2[j+1]=swap;
        
        swap=yvertex3[j];
        yvertex3[j]=yvertex3[j+1];
        yvertex3[j+1]=swap;
        
        
        swap=zvertex1[j];
        zvertex1[j]=zvertex1[j+1];
        zvertex1[j+1]=swap;
        
        swap=zvertex2[j];
        zvertex2[j]=zvertex2[j+1];
        zvertex2[j+1]=swap;
        
        swap=zvertex3[j];
        zvertex3[j]=zvertex3[j+1];
        zvertex3[j+1]=swap;
        
      }
    }
  }
  
  
  
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
 //  glClear(GL_COLOR_BUFFER_BIT);
   
  float a=1;
  float b=0;
  float c=0;   
  for(int i=0;i<h;i++)
  {
      glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(a, b, c); 

      glVertex3f(yvertex1[i]+1,zvertex1[i]+1,0);

      glVertex3f(yvertex2[i]+1,zvertex2[i]+1,0);
      glVertex3f(yvertex3[i]+1,zvertex3[i]+1,0);
      glVertex3f(yvertex3[i]+1,zvertex3[i]+1,0);
      glEnd();
      glFlush();            
       a=a-0.008;
       b=b+0.008;
       c=c+0.008;
  }
  
  
  
  
  
  
  
  
  
  
  

  
/*     glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   //glClear(GL_COLOR_BUFFER_BIT);
   
   
  for(int i=0;i<numpoly;i++)
  {
  float a=1;
  float b=0;
  float c=0;
    for(int j=0;j<vertex[i];j++)
    {          
      glBegin(GL_QUADS);              
      glColor3f(a, b, c); 
      glVertex3f(polyy[i][xvp1[i][j]-1]+1,polyz[i][xvp1[i][j]-1]+1,0);
      glVertex3f(polyy[i][xvp2[i][j]-1]+1,polyz[i][xvp2[i][j]-1]+1,0);
      glVertex3f(polyy[i][xvp3[i][j]-1]+1, polyz[i][xvp3[i][j]-1]+1,0);
      glVertex3f(polyy[i][xvp3[i][j]-1]+1, polyz[i][xvp3[i][j]-1]+1,0);
      glEnd();
      glFlush();
                  
       a=a-0.015;
       b=b+0.015;
       c=c+0.015;
  }
  }

 */
}

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
	 GLint q;
	 fscanf(fp,"%d",&q);
	 material[i]=q;
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
	     polyx[i][j]=xcord*100;
	      polyy[i][j]=ycord*100;
	      polyz[i][j]=zcord*100;
	     printf("\n");
	 }
	 fscanf(fp,"%d",&verte);
	 vertex[i]=verte;
	 for(int j=0;j<verte;j++)
	 {
	  GLint x1,y1,pi;
	  fscanf(fp,"%d",&x1);
	  vertexpoint1[i][j]=x1;
	  fscanf(fp,"%d",&y1);
	  vertexpoint2[i][j]=y1;
	  fscanf(fp,"%d",&pi);
	  vertexpoint3[i][j]=pi;
	 }
	}
	fclose(fp);
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
void rasterize(int ras)
{  if(ras!=0)
  {
   for(int i=0;i<vertex[ras-1];i++)
   {
    calcs(ras,i);
   //printf("%f,%f,%f,%f",xmin,xmax,ymin,ymax);
    display1(ras);
   }
  }
}

void IntensityAmbient(int mat)
{
      Ianet[mat-1][0]=Iax*ka[mat-1][0];
      Ianet[mat-1][1]=Iay*ka[mat-1][1];
      Ianet[mat-1][2]=Iaz*ka[mat-1][2];
}

void IntensityDiffuse(float x1,float y1,float z1, float nx,float ny,float nz,int mat)
{
  float calc=(1-x1)*(1-x1) +(1-y1)*(1-y1)+(F_dist-z1)*(F_dist-z1);
  float llx;
  float lly;
  float llz;
  llx=(Lx-x1)/(sqrt((Lx-x1)*(Lx-x1)+(Ly-y1)*(Ly-y1)+(Lz-z1)*(Lz-z1)));
  lly=(Ly-y1)/(sqrt((Lx-x1)*(Lx-x1)+(Ly-y1)*(Ly-y1)+(Lz-z1)*(Lz-z1)));
  llz=(Lz-z1)/(sqrt((Lx-x1)*(Lx-x1)+(Ly-y1)*(Ly-y1)+(Lz-z1)*(Lz-z1)));
  
  Idnet[mat-1][0]=(Ilx/(sqrt(calc)+K_dist)) + kd[mat-1][0]*(llx*nx+lly*ny+llz*nz);
  Idnet[mat-1][1]=(Ily/(sqrt(calc)+K_dist)) + kd[mat-1][1]*(llx*nx+lly*ny+llz*nz);
  Idnet[mat-1][2]=(Ilz/(sqrt(calc)+K_dist)) + kd[mat-1][2]*(llx*nx+lly*ny+llz*nz); 
}

float power(float y,int f)
{
  float q=1;
 for(int i=0;i<f;i++)
 {
   q=q*y;
 }
 return q;
}

void IntensitySpecular(float x1,float y1,float z1,float nx,float ny, float nz,int mat)
{
  float calc=(1-x1)*(1-x1) + (1-y1)*(1-y1)+(F_dist-z1)*(F_dist-z1);
  float llx;
  float lly;
  float llz;
  llx=(1-x1)/(sqrt((1-x1)*(1-x1)+(1-y1)*(1-y1)+(F_dist-z1)*(Lz-z1)));
  lly=(1-y1)/(sqrt((1-x1)*(1-x1)+(1-y1)*(1-y1)+(F_dist-z1)*(Lz-z1)));
  llz=(F_dist-z1)/(sqrt((1-x1)*(1-x1)+(1-y1)*(1-y1)+(F_dist-z1)*(Lz-z1)));
  
  float llx1;
  float lly1;
  float llz1;
  llx1=(Lx-x1)/(sqrt((Lx-x1)*(Lx-x1)+(Ly-y1)*(Ly-y1)+(Lz-z1)*(Lz-z1)));
  lly1=(Ly-y1)/(sqrt((Lx-x1)*(Lx-x1)+(Ly-y1)*(Ly-y1)+(Lz-z1)*(Lz-z1)));
  llz1=(Lz-z1)/(sqrt((Lx-x1)*(Lx-x1)+(Ly-y1)*(Ly-y1)+(Lz-z1)*(Lz-z1)));
  
  float rx,ry,rz;
  rx= -llx1 + 2*(nx*llx1+ny*lly1+nz*llz1)*nx;
  ry= -lly1 + 2*(nx*llx1+ny*lly1+nz*llz1)*ny;
  rz= -llz1 + 2*(nx*llx1+ny*lly1+nz*llz1)*nz;
  Isnet[mat-1][0]=(Ilx/(sqrt(calc)+K_dist)) + ks[mat-1][0]*power((rx*llx+ry*lly+rz*llz),nmat[mat-1]);
  Isnet[mat-1][1]=(Ily/(sqrt(calc)+K_dist)) + ks[mat-1][1]*power((rx*llx+ry*lly+rz*llz),nmat[mat-1]);
  Isnet[mat-1][2]=(Ilz/(sqrt(calc)+K_dist)) + ks[mat-1][2]*power((rx*llx+ry*lly+rz*llz),nmat[mat-1]); 
}

void cross(float a1,float a2,float a3,float b1,float b2,float b3)
{
//   a2b3-a3b2,a3b1-a1b3,a1b2-a2b1
   vcrossx=a2*b3-a3*b2;
   vcrossy=a3*b1-a1*b3;
   vcrossz=a1*b2-a2*b1;
}

void Inetcal(int mat)
{
 Inet[mat-1][0]=Ianet[mat-1][0]+Isnet[mat-1][0]+Idnet[mat-1][0];
 Inet[mat-1][1]=Ianet[mat-1][1]+Isnet[mat-1][1]+Idnet[mat-1][1];
 Inet[mat-1][2]=Ianet[mat-1][2]+Isnet[mat-1][2]+Idnet[mat-1][2];
}

void normal_calc()
{
   for(int i=0;i<numpoly;i++)
   {
     for(int j=0;j<vertex[i];j++)
     {
       float a11,a12,a13,b11,b12,b13,c11,c12,c13;
       a11=polyx[i][vertexpoint1[i][j]-1];
       a12=polyy[i][vertexpoint1[i][j]-1];
       a13=polyz[i][vertexpoint1[i][j]-1];
       
       b11=polyx[i][vertexpoint2[i][j]-1];
       b12=polyy[i][vertexpoint2[i][j]-1];
       b13=polyz[i][vertexpoint2[i][j]-1];
       
       
       c11=polyx[i][vertexpoint3[i][j]-1];
       c12=polyy[i][vertexpoint3[i][j]-1];
       c13=polyz[i][vertexpoint3[i][j]-1];
       
       float Vx,Vy,Vz,Vx1,Vy1,Vz1;
       Vx=b11-a11;
       Vy=b12-a12;
       Vz=b13-a13;
       
       Vx1=c11-a11;
       Vy1=c12-a12;
       Vz1=c13-a13;
       
       cross(Vx,Vy,Vz,Vx1,Vy1,Vz1);
       norm[i][j][0][0]=vcrossx/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));
       norm[i][j][0][1]=vcrossy/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));
       norm[i][j][0][2]=vcrossz/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));
       
       Vx=c11-b11;
       Vy=c12-b12;
       Vz=c13-b13;
       
       Vx1=a11-b11;
       Vy1=a12-b12;
       Vz1=a13-b13;
       
       cross(Vx,Vy,Vz,Vx1,Vy1,Vz1);
       norm[i][j][1][0]=vcrossx/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));
       norm[i][j][1][1]=vcrossy/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));
       norm[i][j][1][2]=vcrossz/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));
       
       Vx=a11-c11;
       Vy=a12-c12;
       Vz=a13-c13;
       
       Vx1=b11-c11;
       Vy1=b12-c12;
       Vz1=b13-c13;
       
       cross(Vx,Vy,Vz,Vx1,Vy1,Vz1);
       norm[i][j][2][0]=vcrossx/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));
       norm[i][j][2][1]=vcrossy/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));
       norm[i][j][2][2]=vcrossz/(sqrt(vcrossx*vcrossx+vcrossy*vcrossy+vcrossz*vcrossz));    
     }
   }
}

void call_Intensity()
{

  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
    IntensityAmbient(material[i]);
    IntensitySpecular(polyx[i][vertexpoint1[i][j]-1],polyy[i][vertexpoint1[i][j]-1],polyz[i][vertexpoint1[i][j]-1],norm[i][j][0][0],norm[i][j][0][1],norm[i][j][0][2],material[i]);
    IntensityDiffuse(polyx[i][vertexpoint1[i][j]-1],polyy[i][vertexpoint1[i][j]-1],polyz[i][vertexpoint1[i][j]-1],norm[i][j][0][0],norm[i][j][0][1],norm[i][j][0][2],material[i]);
    Inetcal(material[i]);
    Inetnetcal[i][j][0][0]=Inet[material[i]][0];
    Inetnetcal[i][j][0][1]=Inet[material[i]][1];
    Inetnetcal[i][j][0][2]=Inet[material[i]][2];
   // printf("%f,%f,%f\n",Inet[material[i]][0],Inet[material[i]][1],Inet[material[i]][2]);
   }
  }
}

void call_Intensity1()
{
 
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
    IntensityAmbient(material[i]);
    IntensitySpecular(polyx[i][vertexpoint2[i][j]-1],polyy[i][vertexpoint2[i][j]-1],polyz[i][vertexpoint2[i][j]-1],norm[i][j][1][0],norm[i][j][1][1],norm[i][j][1][2],material[i]);
    IntensityDiffuse(polyx[i][vertexpoint2[i][j]-1],polyy[i][vertexpoint2[i][j]-1],polyz[i][vertexpoint2[i][j]-1],norm[i][j][1][0],norm[i][j][1][1],norm[i][j][1][2],material[i]);
    Inetcal(material[i]);
    Inetnetcal[i][j][1][0]=Inet[material[i]][0];
    Inetnetcal[i][j][1][1]=Inet[material[i]][1];
    Inetnetcal[i][j][1][2]=Inet[material[i]][2];
   }
  }
}

void call_Intensity2()
{
  
  for(int i=0;i<numpoly;i++)
  {
   for(int j=0;j<vertex[i];j++)
   {
    IntensityAmbient(material[i]);
    IntensitySpecular(polyx[i][vertexpoint3[i][j]-1],polyy[i][vertexpoint3[i][j]-1],polyz[i][vertexpoint3[i][j]-1],norm[i][j][2][0],norm[i][j][2][1],norm[i][j][2][2],material[i]);
    IntensityDiffuse(polyx[i][vertexpoint3[i][j]-1],polyy[i][vertexpoint3[i][j]-1],polyz[i][vertexpoint3[i][j]-1],norm[i][j][2][0],norm[i][j][2][1],norm[i][j][2][2],material[i]);
    Inetcal(material[i]);
    Inetnetcal[i][j][2][0]=Inet[material[i]][0];
    Inetnetcal[i][j][2][1]=Inet[material[i]][1];
    Inetnetcal[i][j][2][2]=Inet[material[i]][2];
   }
  }
}

void sort(int ras,float z)
{
  int temp,i,j;
  for(i=0;i<vertex[ras-1];i++)
  {
bresenham(polyx[ras-1][vertexpoint1[ras-1][i]-1],polyy[ras-1][vertexpoint1[ras-1][i]-1],polyx[ras-1][vertexpoint2[ras-1][i]-1],polyy[ras-1][vertexpoint2[ras-1][i]-1]);
bresenham(polyx[ras-1][vertexpoint2[ras-1][i]-1],polyy[ras-1][vertexpoint2[ras-1][i]-1],polyx[ras-1][vertexpoint3[ras-1][i]-1],polyy[ras-1][vertexpoint3[ras-1][i]-1]);
bresenham(polyx[ras-1][vertexpoint3[ras-1][i]-1],polyy[ras-1][vertexpoint3[ras-1][i]-1],polyx[ras-1][vertexpoint1[ras-1][i]-1],polyy[ras-1][vertexpoint1[ras-1][i]-1]);
  }
  // printf("HHHHHHHHHHHHHHHHHHIIIIIIIIIIIIIIIIIIIIIIIII\n");
  //bresenham(polyx[ras-1][vertex[ras-1]-1],polyy[ras-1][vertex[ras-1]-1],polyx[ras-1][0],polyy[ras-1][0]);
  for(i=0;i<c;i+=2)
  {
    bresenham(inter1[ras-1][i],z,inter1[ras-1][i+1],z);
    bresenham(inter[ras-1][i],z,inter[ras-1][i+1],z);
    bresenham(inter2[ras-1][i],z,inter2[ras-1][i+1],z);
    
    //printf("\nZZZZZZZZZZZZZZZZZZZZZZZ\n");
    //printf("%f , %f\t,  %f , %f\n",inter[ras-1][i],z,inter[ras-1][i+1],z);
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
  //printf("%d\n",vertex[ras-1]);
  int i;
  for(i=0;i<vertex[ras-1];i++)
  {
   x1=polyx[ras-1][vertexpoint1[ras-1][i]-1];
   y1=polyy[ras-1][vertexpoint1[ras-1][i]-1];
   x2=polyx[ras-1][vertexpoint2[ras-1][i]-1];
   y2=polyy[ras-1][vertexpoint2[ras-1][i]-1];
   
   if(y2<=y1)
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
       //printf("Hi\n");
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
     // printf("%f",x);
     }
   }  
  }
  
}


void ints1(int ras,float z)
{
  float x1,x2,y1,y2,temp;
  c=0;
  for(int i=0;i<100;i++)
  {
   for(int j=0;j<100;j++)
   {
    inter1[i][j]=0;
   }
  }
  //printf("%d\n",vertex[ras-1]);
  int i;
  for(i=0;i<vertex[ras-1];i++)
  {
   x1=polyx[ras-1][vertexpoint1[ras-1][i]-1];
   y1=polyy[ras-1][vertexpoint1[ras-1][i]-1];
   x2=polyx[ras-1][vertexpoint3[ras-1][i]-1];
   y2=polyy[ras-1][vertexpoint3[ras-1][i]-1];
   // printf("\n Vertex Point\n");
  /* if(cc<=vertex[ras-1])
   {
    printf("\n%f     %f      %f        %f\n",x1,y1,x2,y2);
    cc++; 
   }*/
   if(y2<=y1)
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
      inter1[ras-1][c++]=x;
     // printf("%d",)
     
     }
   }  
  }   
}


void ints2(int ras,float z)
{
  float x1,x2,y1,y2,temp;
  c=0;
  for(int i=0;i<100;i++)
  {
   for(int j=0;j<100;j++)
   {
    inter2[i][j]=0;
   }
  }
  //printf("%d\n",vertex[ras-1]);
  for(int i=0;i<vertex[ras-1];i++)
  {
   x1=polyx[ras-1][vertexpoint2[ras-1][i]-1];
   y1=polyy[ras-1][vertexpoint2[ras-1][i]-1];
   x2=polyx[ras-1][vertexpoint3[ras-1][i]-1];
   y2=polyy[ras-1][vertexpoint3[ras-1][i]-1];
   // printf("\n Vertex Point\n");
  /* if(cc<=vertex[ras-1])
   {
    printf("\n%f     %f      %f        %f\n",x1,y1,x2,y2);
    cc++; 
   }*/
   if(y2<=y1)
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
      inter2[ras-1][c++]=x;
     // printf("%d",)
     }
   }  
  }
  
}

void display1(int w)
{
 float s,s2;
   s=ymin+0.001;
  while(s<=ymax)
  {
    ints(w,s);
    ints2(w,s);
    ints1(w,s);
    sort(w,s);
    s++;
  }
}

void do_cal(float x0, float y0, float x1, float y1)  
  {   
  float dx = abs (x1 - x0);  
  float dy = abs (y1 - y0);  
  float x, y;  
  if (dx >= dy)  
  {  
   float d = 2*dy-dx;  
   float ds = 2*dy;  
   float dt = 2*(dy-dx);  
       if (x0 <= x1)   
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
             float d = 2*dx-dy;  
             float ds = 2*dx;  
             float dt = 2*(dx-dy);  
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

void bresenham(float x1, float y1, float x2, float y2) {
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
        draw_pix(x/100,y/100);
        x -= xInc;
        y -= yInc;
    }

 }

void calcs(int ras,int i)
{
  if(xmin>polyx[ras-1][vertexpoint1[ras-1][i]-1])
  {
   xmin=polyx[ras-1][vertexpoint1[ras-1][i]-1];
  }
    if(xmax<polyx[ras-1][vertexpoint1[ras-1][i]-1])
  {
   xmax=polyx[ras-1][vertexpoint1[ras-1][i]-1];
  }
  
   if(xmin>polyx[ras-1][vertexpoint2[ras-1][i]-1])
  {
   xmin=polyx[ras-1][vertexpoint2[ras-1][i]-1];
  }
    if(xmax<polyx[ras-1][vertexpoint2[ras-1][i]-1])
  {
   xmax=polyx[ras-1][vertexpoint2[ras-1][i]-1];
  }
    if(xmin>polyx[ras-1][vertexpoint3[ras-1][i]-1])
  {
   xmin=polyx[ras-1][vertexpoint3[ras-1][i]-1];
  }
    if(xmax<polyx[ras-1][vertexpoint3[ras-1][i]-1])
  {
   xmax=polyx[ras-1][vertexpoint3[ras-1][i]-1];
  }
   if(ymin>polyy[ras-1][vertexpoint1[ras-1][i]-1])
   {
   ymin=polyy[ras-1][vertexpoint1[ras-1][i]-1];
   }
   if(ymax<polyy[ras-1][vertexpoint1[ras-1][i]-1])
   {
   ymax=polyy[ras-1][vertexpoint1[ras-1][i]-1];
   }
  if(ymin>polyy[ras-1][vertexpoint2[ras-1][i]-1])
  {
   ymin=polyy[ras-1][vertexpoint2[ras-1][i]-1];
  }
  if(ymax<polyy[ras-1][vertexpoint2[ras-1][i]-1])
  {
   ymax=polyy[ras-1][vertexpoint2[ras-1][i]-1];
  }
   if(ymin>polyy[ras-1][vertexpoint3[ras-1][i]-1])
  {
   ymin=polyy[ras-1][vertexpoint3[ras-1][i]-1];
  }
  if(ymax<polyy[ras-1][vertexpoint3[ras-1][i]-1])
  {
   ymax=polyy[ras-1][vertexpoint3[ras-1][i]-1];
  }
  
  //printf("%f , \t%d\n",xmin,i);
}

void XYortho()
{

//glLineWidth(2.5);
//glColor3f(1.0, 1.0, 0.0);
//glBegin(GL_LINES);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
 
   // Draw a Red 1x1 Square centered at origin
/*   glBegin(GL_TRI);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      //glVertex2f(-0.5f, -0.5f);    // x, y
     // glVertex2f( 0.5f, -0.5f);
      //glVertex2f( 0.5f,  0.5f);
       glVertex3f(polyx[i][vertexpoint1[i][j]-1],polyy[i][vertexpoint1[i][j]-1],0);
       glVertex3f(polyx[i][vertexpoint2[i][j]-1], polyy[i][vertexpoint2[i][j]-1],0);
       glVertex3f(polyx[i][vertexpoint3[i][j]-1], polyy[i][vertexpoint3[i][j]-1],0);
   glEnd();
 
   glFlush();*/ 

  for(int i=0;i<numpoly;i++)
  {
  float a=1;
  float b=0;
  float c=0;
    for(int j=0;j<vertex[i];j++)
    {
 /*     glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(a, b, c); 

      glVertex3f(polyx[i][vertexpoint1[i][j]-1],polyy[i][vertexpoint1[i][j]-1],0);

      glVertex3f(polyx[i][vertexpoint2[i][j]-1],polyy[i][vertexpoint2[i][j]-1],0);
      glVertex3f(polyx[i][vertexpoint3[i][j]-1],polyy[i][vertexpoint3[i][j]-1],0);
      glVertex3f(polyx[i][vertexpoint1[i][j]-1],polyy[i][vertexpoint1[i][j]-1],0);
      glEnd();
      glFlush();            
       a=a-0.015;
       b=b+0.015;
       c=c+0.015;*/
      /*glVertex3f(polyx[i][vertexpoint2[i][j]-1],polyy[i][vertexpoint2[i][j]-1],0);
      glVertex3f(polyx[i][vertexpoint3[i][j]-1],polyy[i][vertexpoint3[i][j]-1],0);
      glVertex3f(polyx[i][vertexpoint1[i][j]-1],polyy[i][vertexpoint1[i][j]-1],0);*/ 
     
 bresenham(polyx[i][vertexpoint1[i][j]-1],polyy[i][vertexpoint1[i][j]-1],polyx[i][vertexpoint2[i][j]-1], polyy[i][vertexpoint2[i][j]-1]);
 bresenham(polyx[i][vertexpoint2[i][j]-1],polyy[i][vertexpoint2[i][j]-1],polyx[i][vertexpoint3[i][j]-1], polyy[i][vertexpoint3[i][j]-1]);
 bresenham(polyx[i][vertexpoint3[i][j]-1],polyy[i][vertexpoint3[i][j]-1],polyx[i][vertexpoint1[i][j]-1], polyy[i][vertexpoint1[i][j]-1]);
  }
 }

 rasterize(1);
 rasterize(2);
}

void XZortho()
{
//glLineWidth(2.5);
//glColor3f(1.0, 1.0, 0.0);
//glBegin(GL_LINES);
//glVertex3f(0.0, 10.0, 4.0);
//glVertex3f(10.0, 0, 5);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
 

  for(int i=0;i<numpoly;i++)
  {
  float a=1;
  float b=0;
  float c=0;
    for(int j=0;j<vertex[i];j++)
    {
      glBegin(GL_QUADS);              
      glColor3f(a, b, c); 
      glVertex3f(polyx[i][vertexpoint1[i][j]-1],polyz[i][vertexpoint1[i][j]-1]+1,0);

      glVertex3f(polyx[i][vertexpoint2[i][j]-1], polyz[i][vertexpoint2[i][j]-1]+1,0);
      glVertex3f(polyx[i][vertexpoint3[i][j]-1], polyz[i][vertexpoint3[i][j]-1]+1,0);
      glVertex3f(polyx[i][vertexpoint3[i][j]-1], polyz[i][vertexpoint3[i][j]-1]+1,0);
      glEnd();
      glFlush();
       a=a-0.015;
       b=b+0.015;
       c=c+0.015;
    
    }
  }
 glEnd();
}

void YZortho()
{

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   
 for(int i=0;i<numpoly;i++)
  {
   
  float a=1;
  float b=0;
  float c=0;
    for(int j=0;j<vertex[i];j++)
    {
    /*  bresenham(polyy[i][vertexpoint1[i][j]-1]+1,polyz[i][vertexpoint1[i][j]-1]+1,polyy[i][vertexpoint2[i][j]-1]+1,polyz[i][vertexpoint2[i][j]-1]+1);
      bresenham(polyy[i][vertexpoint2[i][j]-1]+1,polyz[i][vertexpoint2[i][j]-1]+1,polyy[i][vertexpoint3[i][j]-1]+1, polyz[i][vertexpoint3[i][j]-1]+1);
      bresenham(polyy[i][vertexpoint3[i][j]-1]+1,polyz[i][vertexpoint3[i][j]-1]+1,polyy[i][vertexpoint1[i][j]-1]+1, polyz[i][vertexpoint1[i][j]-1]+1);
      */       
      glBegin(GL_QUADS);              
      glColor3f(a, b, c); 
      glVertex3f(polyy[i][vertexpoint1[i][j]-1]+1,polyz[i][vertexpoint1[i][j]-1]+1,0);
      glVertex3f(polyy[i][vertexpoint2[i][j]-1]+1,polyz[i][vertexpoint2[i][j]-1]+1,0);
      glVertex3f(polyy[i][vertexpoint3[i][j]-1]+1, polyz[i][vertexpoint3[i][j]-1]+1,0);
      glVertex3f(polyy[i][vertexpoint3[i][j]-1]+1, polyz[i][vertexpoint3[i][j]-1]+1,0);
      glEnd();
      glFlush();
       a=a-0.015;
       b=b+0.015;
       c=c+0.015;
      
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
	XY_painter();
	XZ_painter();
	YZ_painter();
	//blits the current opengl framebuffer on the screen
	//XYortho();
    glutSwapBuffers();
  printf("Enter the operation you want to perform\n");
  printf("For exit press press -1\n");
  int d;
  scanf("%d",&d);
  switch(d)
  {
   case 1:
    
   break;
   case -1:
    printf("Exiting from the system\n");
    outputVertices();
    exit(1); 
   
  }
     //outputVertices();
    //checks for opengl errors
	check();
}
void outputVertices()
{
FILE *fp5;
fp5 = fopen("inp.txt", "w"); // Correct statement to open a file
    if(fp5 == NULL) {
        fprintf(stderr, "Can't open input file testFile\n");
        exit(1);
    }
  GLint a=numpoly;
  fprintf(fp5,"%d\n",a);
  for(int i=0;i<a;i++)
  {
    GLint ab=material[i];
    fprintf(fp5,"%d\n",ab);
    GLint b=polyvertices[i];
    fprintf(fp5,"%d\n",b);
    for(int j=0;j<b;j++)
    {
     GLfloat p,w,z;
     p=polyx[i][j]/100;
     w=polyy[i][j]/100;
     z=polyz[i][j]/100;
     fprintf(fp5,"%f\t",p);
     fprintf(fp5,"%f\t",w);
     fprintf(fp5,"%f\n",z);
    }
    GLint v=vertex[i];
    fprintf(fp5,"%d\n",v);
    for(int j=0;j<v;j++)
    {
     GLint p,w,r;
     p=vertexpoint1[i][j];
     w=vertexpoint2[i][j];
     r=vertexpoint3[i][j];
     fprintf(fp5,"%d\t",p);
     fprintf(fp5,"%d\t",w);
     fprintf(fp5,"%d\n",r);
    }
  }
  fclose(fp5);
}
//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(float x, float y){
    glBegin(GL_POINTS);
    glColor3f(.4,.4,.4);
//glLineWidth(0.3);
//glColor3f(0.2, 0.2, 0.0);
//glBegin(GL_LINES);
    glVertex3d(x,y,0);
    glEnd();
}


void draw_pix1(float x1,float y1,float p,float q,float r)
{
  if(p!=0 || q!=0 || r!=0)
  {
    glBegin(GL_POINTS);
    glColor3f(1.0*p/(p+q+r),1.0*q/(p+q+r),1.0*r/(p+q+r));
//glLineWidth(0.3);
//glColor3f(0.2, 0.2, 0.0);
//glBegin(GL_LINES);
    glVertex3d(x1,y1,0);
    glEnd();
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
