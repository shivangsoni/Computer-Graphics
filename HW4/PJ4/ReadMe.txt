Name:Shivang Soni 
Student ID:915623718

This projects implements Biezer curve and B spline curve.


:::::::::::::::::::::Important Note::::::::::::::::::::::::::::::::::
Due to some keyboard interrupt user need to click at the display window to see the operation working in some cases after entering them into terminal.
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
The Biezer curve is functioning properly if any changes to be made in the output datafile that will be stored back.
The function contains the functionality like:
1. Add vertices to the polygon.
2. Delete vertices from the polygon.
3. Add new polygon
4. Replace the value at the particular vertex in the polygon.
5. Display B-Spline etc..

The B-Spline function is too implemented. 

Default it will display Biezer Curve and operations on the Curve.

For proper termination of the program so that the points will properly be stored in the input file press -1.

This is an console based application.

Functioning through terminal only.
We need to specify the position where do we need to add the points too.




Que1.How to execute the program?
Sol1.There is a input file named "input.txt" from which the program takes all information required to draw a polygon.

There is a output file named "input.txt" where all the modified coordinate information is to be stored.

Compilation run make command
and then ./Final7
Alternatively:
For Compiling the code I use: g++  demo.cpp -lX11 -lGL -lGLU -lglut -g -Wall -O2 -o Final7
Then running the compiled file through command prompt by: ./Final7

Que2.How the program works?
Sol2.This is a console based interactive application.




::::::::::::::::::::::::Example of the working of the program:::::::::::::::::::::
On running the executable file program ask:

Enter the resolution value for which the curve is to be drawn on the screen::
100 //we get a smooth curve and for less resolution value we get curve somewhat like polygons.

Then console ask's 
Enter 1 to add the number of vertices to polygon
Enter 2 to delete vertices from the polygon
Enter 3 to Add new polygon
Enter 4 to Replace the value at the particular vertices in polygon
Enter 5 to display Bspline
Enter 6 to display Beizer curve
Enter -1 if you want to exit

Initially the input file contains 1 polygon in this we can also add other polygons based on our convienence.
user print as 
1
Console:Enter the polygon id to whom vertex needed to be added
User:0 //0 is the polygon id for the first polygon.
Console:Enter the location where you wish to insert an element
User:1// so the value will be inserted at 2 vertix of polygon as consider position 0 as 1st vertex
Console:Enter the x value for that vertex
User:20
Console:Enter the y value for that vertex
User:20


//Now the polygon is reshaped and has 6 vertices.

//Deleting vertices from the polygon
Console:
Enter 1 to add the number of vertices to polygon
Enter 2 to delete vertices from the polygon
Enter 3 to Add new polygon
Enter 4 to Replace the value at the particular vertices in polygon
Enter 5 to display Bspline
Enter 6 to display Beizer curve
Enter -1 if you want to exit

User:2
Console:Enter the polygon id to whom vertex needed to be deleted
User:0
Console:Enter the location where you wish to delete an element
User:1
//Due to some keyboard inturupt user need to click at the display window to see the 2nd operation working.

Adding new polygon will be done in the same way like above where the new incomming polygon will be provided with the last polygon id.

Replacing the value at a particular vertex is also implemented in the same way as this.

Moving onto the B-spline curve on pressing 5 on the console 
there is an keyboard interrupt so click on the display screen.


On clicking 5 :
We can able be enter the order of the B-Spline curve we wanted and can able to see displayed on the screen.
Console: Enter order of the curve you want
User: order value
Console:Select the polygon value to apply B-Spline
User: polygon id
NOTE::




Polygon Id of 1st polygon is 0
               2nd is 1 and so on.......................




Only the curve having the specific polygon ID be displayed on the screen


:::::::::::::::::::::Important Note::::::::::::::::::::::::::::::::::
Due to some keyboard interrupt user need to click at the display window to see the operation working in some cases after entering them into terminal.
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Whole Implementation is done Console Based through Terminal.





















::::::::::::::::::::::::::::::INPUT FILE::::::::::::::::::::::::::::::

1           //Number of Polygon to be drawn on the Screen
5           //Vertices in polygon

10 20       //Values at Vertex
10 30       //
20 50       //
30 30       //
30 10       //















