//
// Created by piotr on 18.03.2021.
//

#include <stdio.h>
#include <time.h>
#include <omp.h>

long long num_steps = 100000000;
double step;

int main(int argc, char* argv[])
{
    clock_t start, stop;
    double x, pi, sum=0.0;
    int i;
    step = 1./(double)num_steps;
    start = clock();
    for (i=0; i<num_steps; i++)
    {
        x = (i + .5)*step;
        sum = sum + 4.0/(1.+ x*x);
    }

    pi = sum*step;
    stop = clock();

    printf("Wartosc liczby PI wynosi %15.12f\n",pi);
    printf("Czas przetwarzania wynosi %f ms\n",((double)(stop - start)/1000.0));
    return 0;
}