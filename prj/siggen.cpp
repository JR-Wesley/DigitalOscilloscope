#include "siggen.h"
#include <iostream>

#define LENGTH 1 << 10
#define HALFLEN 1 << 9
extern int y_data[LENGTH];
extern double f_data[HALFLEN];
const double PI = acos(-1.0);

sigGen::sigGen(QObject *parent):
    QObject(parent)
{
    waveFormat = new int[GENNUM];
    amplitudes = new int[GENNUM];
    frequency = new int[GENNUM];
    phase = new int[GENNUM];
    waveFormat[0] = SINE;
    waveFormat[1] = RECTANGLE;
    waveFormat[2] = NONE;
    waveFormat[3] = NONE;
    amplitudes[0] = 1000;
    amplitudes[1] = 1000;
    amplitudes[2] = 1000;
    amplitudes[3] = 1000;
    frequency[0] = 9600;
    frequency[1] = 9600;
    frequency[2] = 9600;
    frequency[3] = 9600;
    phase[0] = 0;
    phase[1] = 90;
    phase[2] = 180;
    phase[3] = 180;
}

sigGen::~sigGen()
{
    delete []waveFormat;
    delete []amplitudes;
    delete []frequency;
    delete []phase;
}

void sigGen::start_generate()
{
    std::cout << "gene" << std::endl;

    for(int i = 0; i < LENGTH; i++) y_data[i] = 0;
    emit sigCfg(*(waveFormat + 3), *(amplitudes + 3), *(frequency + 3), *(phase + 3));
}

void sigGen::stop_generate(void)
{
    // stop generation
}

void sigGen::Config(QList<int> mode, QList<int> amp,
                   QList<int> freq, QList<int> pha){
    for(int num = 0; num < GENNUM; num++)
    {
        *(waveFormat + num) = mode[num];
        *(amplitudes + num) = amp[num];
        *(frequency + num) = freq[num];
        *(phase + num) = pha[num];
    }
     // std::cout << "config" << std::endl;
}
