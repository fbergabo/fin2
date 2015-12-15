#include <stdio.h>
#include <stdlib.h>

//function which pipes data straight to gnuplot so that the program creates the plot automatically
void plot (double x[], double y1[], double y2[], double y3[], int points);
void plot (double x[], double y1[], double y2[], double y3[], int points)
{
    char *commandsForGnuplot[] =
        { "set title \"Energy vs. Distance for three different methods\"",
            "set logscale x 2\n" "set logscale y 10\n"
            "set xlabel 'Distance'\n" "set ylabel '|Energy|'\n" "set grid\n"
            "plot 'res' u 1:2 title 'vegas' w l, 'res' u 1:3 title 'home-made' w l, 'res' u 1:4 title 'dipole approx.' w l"
        };
    FILE *temp = fopen ("res", "w");    // write coordinates here.
    FILE *gnuplotPipe = popen ("gnuplot -persistent", "w");
    int i;

    for (int q = 0; q < points; q++)
    {
        fprintf (temp, "%lf %lf %lf %lf \n", x[q], y1[q], y2[q], y3[q]);        //Write the data to a temporary file
    }
    for (i = 0; i < 2; i++)
    {
        fprintf (gnuplotPipe, "%s \n", commandsForGnuplot[i]);  //Send commands to gnuplot one by one.
    }
}
