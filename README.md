# project-cs201
TEAM MEMBERS:
Maddineni Sreehitha(2023MCB1300)
S.Keerthi Priya(2023CSB1129)

Course Instructor:Anil Shukla
MENTOR NAME: Seema Barda
PROJECT NAME:Collision Sphere:Advanced Quadtree Collision and Range Detection

Summary: This project aims at using Quadtrees both to make range querying
much more efficient and to accelerate collision detection. In relation to the range
querying module, a quadtree has the effect of arranging cars in a grid where each
zone of the space makes it possible for the system to query a certain space to get
vehicles around a region. This will hasten the search because the number of cars to
be checked around a particular zone is drastically reduced. In the collision-detection
part, tracked bubbles have positions, speeds, and sizes. Every node contains focus
points of nearby bubbles so that this system can find potential collisions efficiently
and actually update their positions according to velocity or bounce them back once
into a wall. This dramatically reduced the checks one had to perform throughout
the process.

The Spatial Querying Code initially asks for the center of region we want of conduct the query on.
Then it asks us to input the dimension of the square which forms our region next we'll be asked to 
input the no. of points we want to input if you give 4, it asks you for input 4 times and inserts 
into the region only those points which fall within the region.Then you have to input the point 
around which you want to perform spatial querying and the length of the square, the points  within
which are to found.It then outputs the points that are  in the region and within the given range.


Collision detection code takes input from file named case.txt which contains bubbles coordinates theeir velocities ,radius and colour which will be given in numbers.
case.txt file input format will be something like this-
10 2 2 1 1 3
23 21 2 -1 6
we can give how many bubbles as we want in the code i have set maximum limit to 100 we can change it if we wish to.
the bubbles information format in input file is-x_coordinate y_coordinate velocity_in_x_direction velocity_in_y_direction radius colour_given in numbers
given each bubble in each line.
the code takes this case.txt input file and then gives output for 10 seconds i mean for each second after updating code it would check for collisions and output those collisions at each time stamp
as i have put 10 timestamps it will give 10 ,we can change this acoording to our need. each time same colour bubbles  collision occur score increases by 1.
so the output format would look something like this-
Simulation Step 0:
Bubble 0 at (12.00,3.00) Color: 3
Score: 0

Simulation Step 1:
Bubble 0 at (14.00,4.00) Color: 3
Score: 0

Simulation Step 2:
Bubble 0 at (16.00,5.00) Color: 3
Score: 0

Simulation Step 3:
Bubble 0 at (18.00,6.00) Color: 3
Score: 0

Simulation Step 4:
Bubble 0 at (20.00,7.00) Color: 3
Score: 0

Simulation Step 5:
Bubble 0 at (22.00,8.00) Color: 3
Score: 0

Simulation Step 6:
Bubble 0 at (24.00,9.00) Color: 3
Score: 0

Simulation Step 7:
Bubble 0 at (26.00,10.00) Color: 3
Score: 0

Simulation Step 8:
Bubble 0 at (28.00,11.00) Color: 3
Score: 0

Simulation Step 9:
Bubble 0 at (30.00,12.00) Color: 3
Score: 0

Final Score: 0

To run the code a C compiler should be installed in the computer like mingw which I installed in my windows and I used vs code to run the program.
To compille we use-gcc problem1.c -o problem1.exe and we run the executable file by-problem1.exe  we can also have coderunner installed in vscode so by clicking the run button we get the output.
