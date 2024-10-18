#include "fft.h"
#include <math.h>

const double PI = acos(-1.0);
__inline int bitReverse(int x)
{
    int i, rtn = 0;
    for (i = 0; i < FFT_M; i++)
        rtn = (rtn << 1) | ((x >> i) & 1);
    return rtn;
}

void CalcW(Cplx16 *W)
{
    int i;
    double arg, argStep;
    argStep = PI / (FFT_LEN >> 1);
    for (arg = 0.0, i = 0; i < (FFT_LEN >> 1); arg -= argStep, i++)
    {
        W[i].R = round(cos(arg) * S16MAX);
        W[i].I = round(sin(arg) * S16MAX);
    }
}

void FFT(Cplx16 *D, Cplx16 *W)
{
    int i, j, step, grpLen, grp, w;
    Cplx16 T, Sub;
    for (step = 0, grpLen = 1 << (FFT_M - 1); step < FFT_M; step++, grpLen >>= 1)
    {
        for (grp = 0; grp < FFT_LEN; grp += grpLen << 1)
        {
            for (i = grp, j = grp + grpLen, w = 0; i < grp + grpLen; i++, j++, w += (1 << step))
            {
                T.R = ((int)D[i].R + (int)D[j].R) >> 1;
                T.I = ((int)D[i].I + (int)D[j].I) >> 1;
                Sub.R = ((int)D[i].R - (int)D[j].R) >> 1;
                Sub.I = ((int)D[i].I - (int)D[j].I) >> 1;
                D[j].R = (((int)Sub.R * (int)W[w].R) >> 11) - (((int)Sub.I * (int)W[w].I) >> 11);
                D[j].I = (((int)Sub.I * (int)W[w].R) >> 11) + (((int)Sub.R * (int)W[w].I) >> 11);
                D[i] = T;
            }
        }
    }
    for (i = 0; i < FFT_LEN; i++)
    {
        j = bitReverse(i);
        if (i < j)
        {
            T = D[j];
            D[j] = D[i];
            D[i] = T;
        }
    }
}

//#define REVIFFT
void IFFT(Cplx16 *D, Cplx16 *W)
{
#ifndef REVIFFT
    int i, j, step, grpLen, grp, w;
    Cplx16 T, Sub;
    for (step = 0, grpLen = 1 << (FFT_M - 1); step < FFT_M; step++, grpLen >>= 1)
    {
        for (grp = 0; grp < FFT_LEN; grp += grpLen << 1)
        {
            for (i = grp, j = grp + grpLen, w = 0; i < grp + grpLen; i++, j++, w += (1 << step))
            {
                T.R = ((int)D[i].R + (int)D[j].R);
                T.I = ((int)D[i].I + (int)D[j].I);
                Sub.R = ((int)D[i].R - (int)D[j].R);
                Sub.I = ((int)D[i].I - (int)D[j].I);
                D[j].R = (((int)Sub.R * (int)W[w].R) >> 15) + (((int)Sub.I * (int)W[w].I) >> 15);
                D[j].I = (((int)Sub.I * (int)W[w].R) >> 15) - (((int)Sub.R * (int)W[w].I) >> 15);
                D[i] = T;
            }
        }
    }
    for (i = 0; i < FFT_LEN; i++)
    {
        j = bitReverse(i);
        if (i < j)
        {
            T = D[j];
            D[j] = D[i];
            D[i] = T;
        }
    }
#else
    int i, j, step, grpLen, grp, w;
    Cplx16 T, Sub;
    for (i = 0; i < FFT_LEN; i++)
    {
        j = bitReverse(i);
        if (i < j)
        {
            T = D[j];
            D[j] = D[i];
            D[i] = T;
        }
    }
    for (step = FFT_M - 1, grpLen = 1; step >= 0; step--, grpLen <<= 1)
    {
        for (grp = 0; grp < FFT_LEN; grp += grpLen << 1)
        {
            for (i = grp, j = grp + grpLen, w = 0; i < grp + grpLen; i++, j++, w += (1 << step))
            {
                Sub.R = (((int)D[j].R * (int)W[w].R) >> 15) + (((int)D[j].I * (int)W[w].I) >> 15);
                Sub.I = (((int)D[j].I * (int)W[w].R) >> 15) - (((int)D[j].R * (int)W[w].I) >> 15);
                T.R = ((int)D[i].R + (int)Sub.R);
                T.I = ((int)D[i].I + (int)Sub.I);
                D[j].R = ((int)D[i].R - (int)Sub.R);
                D[j].I = ((int)D[i].I - (int)Sub.I);
                D[i] = T;
            }
        }
    }
#endif
}
