#include <iostream>
#include <stdio.h>
#include <ctime>

#include "Array.hpp"

int main()
{
    FILE* output = fopen("dots", "w");
    if(!output)
    {
        PrintErr("Couldn't open file\n");
        return -1;
    }

    Array<double> a;

    int knots = 20;
    size_t amplitude = 20;

    srand(time(nullptr));
    for(int i = 0; i < knots; i++)
        a << rand() % amplitude;

    int counter = 0;
    double step = 0.05;
    for(double i = 2; i < knots - 2; i += step)
    {
        fprintf(output, "%lg\t%lg", i, a[i]);
        if(counter == 1/step || counter == 0)
        {
            fprintf(output, "\t%lg", a[i]);
            counter = 0;
        }
        fprintf(output, "\n");
        counter++;
    }

    fclose(output);

    system("gnuplot CreatePlot");
    system("xdg-open plotty.png");
}

// Как запихать нормально в другой файл?

// template<typename data_T>
// Array<data_T> Array<data_T>::operator[](data_T new_elem)
//              ^
//              |
//              &       <-- какой конструктор вызывается без него?

// Канарейки? Через new?

// xvalue - что это?

// Swap - нормально ли это, что руками, а не swap?

// Perfect forwarding - что это?

// Что можно сделать с []? Ambiguating :(

// Частичная и явная специализация - пишем каждый раз все заново?







