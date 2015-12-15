#include <stdio.h>
#include <gsl/gsl_math.h>

#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include "plot.c"
#include "timer.c"
#include "home_integrand.c"
#include "vegas_integrand.c"
#include "printers.c"
#define N 20


//parameter structure for vegas integrator
struct my_params
{
    double r;
};

extern double g (double *t, size_t dim, void *params);

//begin integration
int main (void)
{
    //print header
    print_header ();

    printf ("\nGSL Vegas Integrator:\nDistance      Energy\n");
    //initialize variables and arrays
    double res, err;
    double distance[N];
    double result[N];
    size_t dim = 6;
    double xl[] = { 0., 0., 0., 0., 0., 0. };
    double xu[] = { 1., 1., 1., 1., 1., 1. };
    double tvegas;

    //initialize random number generator
    gsl_rng *r = gsl_rng_alloc (gsl_rng_taus2);
    unsigned long seed = 1UL;
    double vegas_error[N];

    gsl_rng_set (r, seed);

    size_t calls = 1000000;
    double dd = (4 - 1.001) / N;

    timer_start ();
    //run n times
    for (int q = 0; q < N; q++)
    {
        struct my_params params = { 1.001 + dd * (double) q };

        //run vegas integrator
        gsl_monte_function G = { &g, dim, &params };

        //store distances in distance array
        distance[q] = 1.001 + dd * (double) q;
        gsl_monte_vegas_state *sv = gsl_monte_vegas_alloc (dim);

        gsl_monte_vegas_init (sv);
        gsl_monte_vegas_integrate (&G, xl, xu, dim, calls / 5, r, sv, &res,
            &err);

        //run until there is close agreement
        do
        {
            gsl_monte_vegas_integrate (&G, xl, xu, dim, calls, r, sv, &res,
                &err);
            fflush (stdout);
        }
        while (fabs (gsl_monte_vegas_chisq (sv) - 1.0) > 0.2);

        //store vegas results in result array
        result[q] = res;
        vegas_error[q] = err;
        print_steps (distance[q], result[q]);
        //free vegas space
        gsl_monte_vegas_free (sv);
    }
    tvegas = timer_stop ();

    //free random number generator space
    gsl_rng_free (r);

    printf ("\nHome-made Integrator:\nDistance      Energy\n");
    //begin homemade integrator
    //initialize variables
    double x1, x2, y1, y2, z1, z2;
    int num = 1000000;
    double thome;

    //use random number generator
    gsl_rng *rando = gsl_rng_alloc (gsl_rng_taus2);

    gsl_rng_set (rando, seed);

    //initialize results array for homemade integrator
    double varray[N];

    timer_start ();
    //loop over distances

    for (int kt = 0; kt < N; kt++)
    {
        double dz = 1.001 + kt * dd;
        double value = 0.;


        //assign random coordinates
        for (int qi = 0; qi < num; qi++)
        {
            x1 = gsl_rng_uniform (rando);
            x2 = gsl_rng_uniform (rando);
            y1 = gsl_rng_uniform (rando);
            y2 = gsl_rng_uniform (rando);
            z1 = gsl_rng_uniform (rando);
            z2 = gsl_rng_uniform (rando);
            //calculate value of integrand
            value += f (x1, x2, y1, y2, z1, z2, dz);
        }
        //store results in homemade results array

        varray[kt] = value / num;
        print_steps (distance[kt], varray[kt]);

    }
    thome = timer_stop ();

    double karray[N];
    double tapprox;

    timer_start ();
    printf ("\nDipole Approximation:\nDistance      Energy\n");
    //Begin dipole approximation
    for (int v = 0; v < N; v++)
    {
        karray[v] = -2. / (distance[v] * distance[v] * distance[v]);
        print_steps (distance[v], karray[v]);
    }
    tapprox = timer_stop ();



    //print distances with results of vegas and results of homemade integrator
    printf
        ("\nResults Table (with absolute value of energies):\nDistance     GSL Vegas     Home-made     Dipole Approximation\n");
    for (int j = 0; j < N; j++)
    {
        printf ("%.6f     %.6f      %.6f     %.6f\n", distance[j],
            fabs (result[j]), fabs (varray[j]), fabs (karray[j]));
    }

    double vegasarray[N], homearray[N], approxarray[N];

    for (int l = 0; l < N; l++)
    {
        vegasarray[l] = fabs (result[l]);
        homearray[l] = fabs (varray[l]);
        approxarray[l] = fabs (karray[l]);
    }
    printf ("\nTiming for the different methods:\n");
    print_timing (tvegas, thome, tapprox);
    printf
        ("\nTime ratio vegas / home-made = %.6f\nTime ratio vegas / dipole approximation = %.6f\nTime ratio home-made / dipole approximation = %.6f\n",
        tvegas / thome, tvegas / tapprox, thome / tapprox);
    plot (distance, vegasarray, homearray, approxarray, N);


    printf ("Distance      Vegas Error       Home-made Error\n");
    for (int h = 0; h < N; h++)
    {
        printf ("%.6f      %.6f      %.6f\n", distance[h], vegas_error[h],
            fabs (homearray[h] - vegasarray[h]));
    }

    return 0;

}
