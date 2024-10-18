#ifndef ANALYZER_H
#define ANALYZER_H

#include <QObject>
#include "fft.h"

class Analyzer : public QObject
{
    Q_OBJECT
public:
    explicit Analyzer(QObject *parent = nullptr);

private:
    Cplx16 x[FFT_LEN];
    Cplx16 w[FFT_LEN / 2];

signals:

public slots:
    void fft_data();
};

#endif // ANALYZER_H
