#include "analyzer.h"
#include <iostream>
#include <cmath>

#define LENGTH 1 << 10
#define HALFLEN 1 << 9
extern int y_data[LENGTH];
extern double f_data[LENGTH];
const double PI = acos(-1.0);

Analyzer::Analyzer(QObject *parent)
    : QObject{parent}
{
    for (int i = 0; i < LENGTH; i++)
    {
        x[i].R = 2000 * sin(2 * PI * i / (1 << 14));
        x[i].I = 0;
    }
    CalcW(w);
}

void Analyzer::fft_data()
{
    for(int i = 0; i < LENGTH; i++)
    {
        x[i].R = y_data[i];
        x[i].I = 0;
    }
    FFT(x, w);

    // std::cout << "after FFT" << std::endl;
    for (int i = 0; i < HALFLEN; i++)
    {
        if (i != 0){
            f_data[i]  = sqrt(x[i].R * x[i].R + x[i].I * x[i].I) + \
                   sqrt(x[FFT_LEN - i].R * x[FFT_LEN - i].R + x[FFT_LEN - i].I * x[FFT_LEN - i].I );
        }
         // if(i < 5) std::cout << f_data[i] << "    " << "x. R" << x[i].R << "  " << x[i].I << std::endl;
    }

    // std::cout << std::endl;
}
