1.inp.txt:to store the inputted vertices of the polygon and after all transformations and operations store the updated vertices.
2.Information Manual:Contain information how to execute the programs.
3.ReadMe.txt:All files present.
4.Makefile:In order to avoid compilation
5.cppfile:demo.cpp
6.light.txt: read light parameters.
7.material.txt: read material related values


Implementation done for Phonge, Gouraud shading and Painter's algorithm and all of them are functioning properly.

Tried implemented Half-toning but not functional.

For painter's algorithm: sorting all the triangle based on the plane perpendicular to it and drawing in on screen. The sorting used is bubble sort for the following.


Tried taking input for bunny.txt and testobj.txt

Que1.How to execute the program?
Sol1.There is a input file named "inp.txt" from which the program takes all information required to draw a polygon.

There is a output file named "inp.txt" where all the modified coordinate information is to be stored.

Compilation run make command
and then ./Final7
Alternatively:
For Compiling the code I use: g++  demo.cpp -lX11 -lGL -lGLU -lglut -g -Wall -O2 -o Final7
Then running the compiled file through command prompt by: ./Final7

Que2.How the program works?
Sol2.This is a console based interactive application.


The input will be given in form like:
2                           #number of objects

1                           #material id (starts at 1)
12                          #number of vertices
0.75 0.1974269 0.5850651    #x y z of vertices, 1 per line
0.8350651 0.25 0.5525731
0.8350651 0.25 0.4474269
0.6649349 0.25 0.4474269
0.6649349 0.25 0.5525731
0.6974269 0.3350651 0.5
0.8025731 0.3350651 0.5
0.8025731 0.1649349 0.5
0.6974269 0.1649349 0.5
0.75 0.1974269 0.4149349
0.75 0.3025731 0.4149349
0.75 0.3025731 0.5850651
20                          #number of triangles 
2 3 7                       #index for 3 triangle vertices (starts at 1)
2 8 3                       
4 5 6
5 4 9
7 6 12
6 7 11
10 11 3
11 10 4
8 9 10
9 8 1
12 1 2
1 12 5
7 3 11
2 7 12
4 6 11
6 5 12
3 8 10
8 2 1
4 10 9
5 9 1

2                            #material id for object 2
8                            #number of vertices for object 2
0.375 0.375 0.625
0.625 0.375 0.625
0.375 0.625 0.625
0.625 0.625 0.625
0.375 0.625 0.375
0.625 0.625 0.375
0.375 0.375 0.375
0.625 0.375 0.375
12                           #tri index for project 2
1 2 3
3 2 4
3 4 5
5 4 6
5 6 7
7 6 8
7 8 1
1 8 2
2 8 4
4 8 6
7 1 5
5 1 3
