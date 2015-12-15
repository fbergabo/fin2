PHYS 2200 Final Part 2

In this program, two cubes of non-uniform charge density have an interaction energy that depends on the distance between them. In order to calculate the potential energy, we use monte carlo techniques to integrate the expression. The first technique is using the GSL vegas monte carlo integrator, the second technique is a home-made integrator, and the third technique is to use the dipole approximation. The timing and errors for the first two methods are compared to get a general idea of the time needed to make the home-made integrator achieve the same level of accuracy as the vegas integrator. In order to run the program, please download all of the program files in this repository, type the word "make" in the command line to compile, and then type ./main in order to run the program. It will take a few minutes to complete, and it will automatically generate a graph with logarithmic axes in which all three methods are plotted. 

The graph produced by the program is shown below:

![](graph1.png?)

This program design is the product of a collaboration with Connor Occhialini.
