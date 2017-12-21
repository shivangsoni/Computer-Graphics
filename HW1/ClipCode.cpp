 //display view port on the screen
 //assign code for clipping
 
 /*for(int i=0;i<numpoly;i++)
 {
  for(int j=0;j<v[i]-1;j++)
  {
    int codes[4],codes1[4];
    for(int k=0;k<4;k++)
    {
      codes[k]=0;
      codes1[k]=0;
    }
    float polyxnew[100]=0;
    float polyynew[100]=0;
    int newver=0;
    for(int h=0;h<100;h++)
    {
     polyxnew[h]=0;
     polyynew[h]=0;
    }
    //check the conditions if (a,b,r,l)
    float xcord=polyx[i][j];
    float ycord=polyy[i][j];
    float xcordnext=polyx[i][j+1];
    float ycordnext=polyy[i][j+1];
    if(xcord<xmin && xcord<ymin)
    {
      codes[0]=0; codes[1]=1; codes[2]=0; codes[3]=1;
    }
    else if(xcord<xmin && ycord>ymin && ycord<ymax)
    {
     codes[0]=0; codes[1]=0; codes[2]=0; codes[3]=1;
    }
    else if(xcord<xmin && ycord>ymax)
    {
     codes[0]=1; codes[1]=0; codes[2]=0; codes[3]=1;
    }
    else if(xcord>xmax && ycord<ymin)
    {
    codes[0]=0; codes[1]=1; codes[2]=1; codes[3]=0;
    }
    else if(xcord>xmax && ycord>ymin && ycord<ymax)
    {
     codes[0]=0; codes[1]=0; codes[2]=1; codes[3]=0;
    }
    else if(xcord>xmax && ycord>ymax)
    {
     codes[0]=1; codes[1]=0; codes[2]=1; codes[3]=0;
    }
    else if(xcord<xmax && xcord>xmin && ycord<ymin)
    {
     codes[0]=0; codes[1]=1; codes[2]=0; codes[3]=0;
    }
    else if(xcord<xmax && xcord>xmin && ycord>ymax)
    {
     codes[0]=1; codes[1]=0; codes[2]=0; codes[3]=0;
    }
    else
    {
     codes[0]=0; codes[1]=0; codes[2]=0; codes[3]=0;
    }
    
    //2nd point codes
     if(xcordnext<xmin && xcordnext<ymin)
    {
      codes1[0]=0; codes1[1]=1; codes1[2]=0; codes1[3]=1;
    }
    else if(xcordnext<xmin && ycordnext>ymin && ycordnext<ymax)
    {
     codes1[0]=0; codes1[1]=0; codes1[2]=0; codes1[3]=1;
    }
    else if(xcordnext<xmin && ycordnext>ymax)
    {
     codes1[0]=1; codes1[1]=0; codes1[2]=0; codes1[3]=1;
    }
    else if(xcordnext>xmax && ycordnext<ymin)
    {
    codes1[0]=0; codes1[1]=1; codes1[2]=1; codes1[3]=0;
    }
    else if(xcordnext>xmax && ycordnext>ymin && ycordnext<ymax)
    {
     codes1[0]=0; codes1[1]=0; codes1[2]=1; codes1[3]=0;
    }
    else if(xcordnext>xmax && ycordnext>ymax)
    {
     codes1[0]=1; codes1[1]=0; codes1[2]=1; codes1[3]=0;
    }
    else if(xcordnext<xmax && xcordnext>xmin && ycordnext<ymin)
    {
     codes1[0]=0; codes1[1]=1; codes1[2]=0; codes1[3]=0;
    }
    else if(xcordnext<xmax && xcordnext>xmin && ycordnext>ymax)
    {
     codes1[0]=1; codes1[1]=0; codes1[2]=0; codes1[3]=0;
    }
    else
    {
     codes1[0]=0; codes1[1]=0; codes1[2]=0; codes1[3]=0;
    }  
    //codes set  
    
    if(codes[0]==0 && codes[1]==0 && codes[2]==0 && codes[3]==0 && codes1[0]==0 && codes1[1]==0 && codes1[2]==0 && codes1[3]==0)
    {
     //Accept line 
     polyxnew[newver]=polyx[i][j];
     polyynew[newver]=polyy[i][j];
     newver++;
     polyxnew[newver]=polyx[i][j+1];
     polyynew[newver]=polyy[i][j+1];
     newver++;   
    }
    else if((codes[0]&codes1[0])||(codes[1]&codes1[1])||(codes[2]&codes1[2])||(codes[3]&codes1[3]))
    {
      //Line Rejected
    }
    else
    {
      //Line is +i.v slope
      
    }
  }
 }*/
 /*
  float polyxnew[100][100];
  float polyynew[100][100];
  int ver=0;
  float y2,y1,x2,x1;
  for(int i=0;i<100;i++)
  {
    for(int j=0;j<100;j++)
    {
      polyxnew[i][j]=0;
      polyynew[i][j]=0;
    }
  }
 
 for(int i=0;i<numpoly;i++)
 {
  for(int j=0;j<v[i]-1;j++)
  {
    x1=polyx[i][j];
    y1=polyy[i][j];
    x2=polyx[i][j+1];
    y2=polyx[i][j+1];
   int c1=codes(polyx[i][j],polyy[i][j]);
    int c2=codes(polyx[i][j+1],polyy[i][j+1]);
    float m=(y2-y1)/(x2-x1);
    while((c1|c2)>0)
    {
        if((c1 & c2)>0)
        {
           printf("c1 is %d and c2 is %d\n",c1,c2);
          // exit(0);
        }

    float xi=x1;float yi=y1;
    int c=c1;
    if(c==0)
    {
         c=c2;
         xi=x2;
         yi=y2;
    }
    float x,y;
    if((c & 8)>0)
    {
       y=ymax;
       x=xi+ 1.0/m*(ymax-yi);
    }
    else
      if((c & 4)>0)
      {
          y=ymin;
          x=xi+1.0/m*(ymin-yi);
      }
      else
       if((c & 2)>0)
       {
           x=xmax;
           y=yi+m*(xmax-xi);
       }
       else
       if((c & 1)>0)
       {
           x=xmin;
           y=yi+m*(xmin-xi);
       }

       if(c==c1)
       {
           xd1=x;
           yd1=y;
           c1=codes(xd1,yd1);
       }

       if(c==c2)
       {
           xd2=x;
           yd2=y;
           c2=codes(xd2,yd2);
       }
}
polyxnew[i][ver]=polyx[i][j];
polyynew[i][ver]=polyy[i][j];
ver++;
polyxnew[i][ver]=polyx[i][j+1];
polyynew[i][ver]=polyy[i][j+1];
ver++;
  }
  
//display() 2 points;
x1=polyx[i][v[i]-1];
    y1=polyy[i][v[i]-1];
    x2=polyx[i][0];
    y2=polyx[i][0];
   int c1=codes(x1,y1);
   int c2=codes(x2,y2);
    float m=(y2-y1)/(x2-x1);
    while((c1|c2)>0)
    {
        if((c1 & c2)>0)
        {
        printf("2 c1 is %d and c2 is %d\n",c1,c2);
          // exit(0);
        }

    float xi=x1;float yi=y1;
    int c=c1;
    if(c==0)
    {
         c=c2;
         xi=x2;
         yi=y2;
    }
    float x,y;
    if((c & 8)>0)
    {
       y=ymax;
       x=xi+ 1.0/m*(ymax-yi);
    }
    else
      if((c & 4)>0)
      {
          y=ymin;
          x=xi+1.0/m*(ymin-yi);
      }
      else
       if((c & 2)>0)
       {
           x=xmax;
           y=yi+m*(xmax-xi);
       }
       else
       if((c & 1)>0)
       {
           x=xmin;
           y=yi+m*(xmin-xi);
       }

       if(c==c1)
       {
           xd1=x;
           yd1=y;
           c1=codes(xd1,yd1);
       }

       if(c==c2)
       {
           xd2=x;
           yd2=y;
           c2=codes(xd2,yd2);
       }
//last point 
  //do_cal(xd1,yd1,xd2,yd2);
polyxnew[i][ver]=polyx[i][v[i]-1];
polyynew[i][ver]=polyy[i][v[i]-1];
ver++;
polyxnew[i][ver]=polyx[i][0];
polyynew[i][ver]=polyy[i][0];
ver++;  
  }
  v[i]=ver;
  for(int k=0;k<ver;k++)
  {
   polyx[i][k]=polyxnew[i][k];
   polyy[i][k]=polyynew[i][k];
  }
 }*/
 //waitFor(5);
