#ifndef SIGGEN_H
#define SIGGEN_H

#include <QObject>
#include <cmath>

#define GENNUM  4

class sigGen: public QObject
{
    Q_OBJECT
public:
    enum wave_format {NONE = 0, SINE, RECTANGLE, SAWTOOTH, RANDOM};
    explicit sigGen(QObject *parent = nullptr);
    ~sigGen();

private:
    int *waveFormat;
    int *amplitudes;
    int *frequency;
    int *phase;
signals:
    void sigCfg(int, int, int, int);
public slots:
    void start_generate(void);
    void stop_generate(void);
    void Config(QList<int> mode,
                   QList<int> amp,
                   QList<int> freq,
                   QList<int> pha);
};

#endif // SIGGEN_H
