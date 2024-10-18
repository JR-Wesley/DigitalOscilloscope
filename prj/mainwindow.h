#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "siggen.h"
#include "analyzer.h"
#include "vlfd.h"
#include <qcustomplot.h>
#include <QPushButton>
#include <QTimer>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define REFRESH_TIME      200      //读取一帧数据的时间41毫秒，采样率为50K

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initMembers();
    void initLayout();
    void initConnection();
private:
    Ui::MainWindow *ui;
    sigGen *signal;
    Analyzer *ana;
    fpga::VLFD *vlfd_manager;
    QTimer timer_plot;
    int mode;

    QSharedPointer<QCPGraphDataContainer> wave_data;
    QSharedPointer<QCPGraphDataContainer> freq_data;
public slots:
    void replot();
    void newConfig(int value);
    void startTimer();
    void stopTimer();
    void textEdt(QString);
    //void modeCfg();
signals:
//    void newCycle(int value);
    void newSig(QList<int>, QList<int>, QList<int>, QList<int>);
};
#endif // MAINWINDOW_H
