#include<stdio.h>

void print_header (void);
void print_header (void)
{
    printf
        ("Please wait, this program takes a long time to run\nWhen the program has finished, it will print all of the reults in one table and automatically graph them\n");
}

void print_timing (double tvegas, double thome, double tapprox);
void print_timing (double tvegas, double thome, double tapprox)
{
    printf
        ("Vegas Time: %.6f  Home-made Time: %.6f  Dipole Approximation: %.6f",
        tvegas, thome, tapprox);
}

void print_steps (double distance, double result);
void print_steps (double distance, double result)
{
    printf ("%.6f   %.6f\n", distance, result);
}
