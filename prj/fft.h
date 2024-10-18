#ifndef __FFT_H__
#define __FFT_H__

#define FFT_M 10
#define FFT_LEN (1 << FFT_M)

#define S16MAX	 2047
#define S16MIN	-2048
// #define PI 3.1415926535897932385

typedef struct _Cplx16
{
    int R;
    int I;
} Cplx16;

void CalcW(Cplx16 *W);
void FFT(Cplx16 *D, Cplx16 *W);
void IFFT(Cplx16 *D, Cplx16 *W);

#endif
