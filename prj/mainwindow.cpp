#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#define LENGTH 1 << 10
#define HALFLEN 1 << 9
extern int y_data[LENGTH];
extern double f_data[HALFLEN];

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initMembers();
    initLayout();
    initConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete vlfd_manager;
    delete signal;
    delete ana;
}

void MainWindow::initMembers()
{
    vlfd_manager = new fpga::VLFD(this);
    signal = new sigGen(this);
    ana = new Analyzer(this);
    wave_data = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    freq_data = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    timer_plot.stop();
    mode = 0;
}

void MainWindow::initConnection()
{
//    connect(ui->startBut, SIGNAL(clicked()),
//            this, SLOT(startTimer()));
//    connect(ui->stopBut, SIGNAL(clicked()),
//            this, SLOT(stopTimer()));

    // timer -> generator, vlfd
//    connect(&timer_plot, SIGNAL(timeout()),
//            signal, SLOT(start_generate()));
//    connect(&timer_plot, SIGNAL(timeout()),
//            vlfd_manager, SLOT(fdeWnR()));
    connect(ui->startBut, SIGNAL(clicked()),
            signal, SLOT(start_generate()));
    connect(signal, SIGNAL(sigCfg(int,int,int,int)),
            vlfd_manager, SLOT(cfg(int,int,int,int)));
    connect(ui->startBut, SIGNAL(clicked()),
            vlfd_manager, SLOT(fdeWnR()));

    // vlfd -> analyzer
    connect(vlfd_manager, SIGNAL(proc()),
            ana, SLOT(fft_data()));
    connect(vlfd_manager, SIGNAL(proc()),
            this, SLOT(replot()));

    //  generate -> progBit
    connect(ui->genBut, SIGNAL(clicked()),
            vlfd_manager, SLOT(progBit()));
    connect(ui->comboBox_mode, SIGNAL(currentIndexChanged(int)),
            vlfd_manager, SLOT(progCfg(int)));
    // vlfd -> text
    connect(vlfd_manager, SIGNAL(log(QString)),
            this, SLOT(textEdt(QString)));

    // signal -> text
    connect(this, &MainWindow::newSig,
            signal, &sigGen::Config);

    connect(ui->comboBox_1, SIGNAL(currentIndexChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->comboBox_4, SIGNAL(currentIndexChanged(int)),
            this, SLOT(newConfig(int)));

    connect(ui->spinBox_A1, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_A2, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_A3, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_A4, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_F1, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_F2, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_F3, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_F4, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_P1, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_P2, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_P3, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
    connect(ui->spinBox_P4, SIGNAL(valueChanged(int)),
            this, SLOT(newConfig(int)));
}


void MainWindow::initLayout()
{
    this->setWindowTitle(QString("FDE signal analyzer @Hanyu Zhang"));

    ui->comboBox_mode->addItem(QString("sine"));
    ui->comboBox_mode->addItem(QString("rectangle"));
    ui->comboBox_mode->addItem(QString("sawtooth"));
    ui->comboBox_mode->addItem(QString("random"));
    ui->comboBox_mode->addItem(QString("invtest"));
    ui->comboBox_mode->setCurrentText("invtest");

    // select wave format
    ui->comboBox_1->addItem(QString("none"));
    ui->comboBox_1->addItem(QString("sine"));
    ui->comboBox_1->addItem(QString("rectangle"));
    ui->comboBox_1->addItem(QString("sawtooth"));
    ui->comboBox_1->addItem(QString("random"));
    ui->comboBox_1->setCurrentText("sine");

    ui->comboBox_2->addItem(QString("none"));
    ui->comboBox_2->addItem(QString("sine"));
    ui->comboBox_2->addItem(QString("rectangle"));
    ui->comboBox_2->addItem(QString("sawtooth"));
    ui->comboBox_2->addItem(QString("random"));
    ui->comboBox_2->setCurrentText("rectangle");

    ui->comboBox_3->addItem(QString("none"));
    ui->comboBox_3->addItem(QString("sine"));
    ui->comboBox_3->addItem(QString("rectangle"));
    ui->comboBox_3->addItem(QString("sawtooth"));
    ui->comboBox_3->addItem(QString("random"));
    ui->comboBox_3->setCurrentText("none");

    ui->comboBox_4->addItem(QString("none"));
    ui->comboBox_4->addItem(QString("sine"));
    ui->comboBox_4->addItem(QString("rectangle"));
    ui->comboBox_4->addItem(QString("sawtooth"));
    ui->comboBox_4->addItem(QString("random"));
    ui->comboBox_4->setCurrentText("none");

    // set attribute
    ui->spinBox_A1->setMaximum(2047);
    ui->spinBox_A1->setMinimum(0);
    ui->spinBox_A1->setValue(1000);
    ui->spinBox_A1->setSingleStep(10);
    ui->spinBox_A2->setMaximum(2047);
    ui->spinBox_A2->setMinimum(0);
    ui->spinBox_A2->setValue(1000);
    ui->spinBox_A2->setSingleStep(10);
    ui->spinBox_A3->setMaximum(2047);
    ui->spinBox_A3->setMinimum(0);
    ui->spinBox_A3->setValue(1000);
    ui->spinBox_A3->setSingleStep(10);
    ui->spinBox_A4->setMaximum(2047);
    ui->spinBox_A4->setMinimum(0);
    ui->spinBox_A4->setValue(1000);
    ui->spinBox_A4->setSingleStep(10);

    // Freq
    ui->spinBox_F1->setMaximum(5e7);
    ui->spinBox_F1->setMinimum(6103);
    ui->spinBox_F1->setValue(9600);
    ui->spinBox_F1->setSingleStep(100);

    ui->spinBox_F2->setMaximum(5e7);
    ui->spinBox_F2->setMinimum(6103);
    ui->spinBox_F2->setValue(9600);

    ui->spinBox_F2->setSingleStep(10);
    ui->spinBox_F3->setMaximum(5e7);
    ui->spinBox_F3->setMinimum(6103);
    ui->spinBox_F3->setValue(9600);
    ui->spinBox_F3->setSingleStep(100);

    ui->spinBox_F4->setMaximum(5e7);
    ui->spinBox_F4->setMinimum(6103);
    ui->spinBox_F4->setValue(9600);
    ui->spinBox_F4->setSingleStep(100);

    ui->spinBox_P1->setMaximum(360);
    ui->spinBox_P1->setMinimum(0);
    ui->spinBox_P1->setValue(0);
    ui->spinBox_P1->setSingleStep(5);
    ui->spinBox_P2->setMaximum(360);
    ui->spinBox_P2->setMinimum(0);
    ui->spinBox_P2->setValue(90);
    ui->spinBox_P2->setSingleStep(5);
    ui->spinBox_P3->setMaximum(360);
    ui->spinBox_P3->setMinimum(0);
    ui->spinBox_P3->setValue(180);
    ui->spinBox_P3->setSingleStep(5);
    ui->spinBox_P4->setMaximum(360);
    ui->spinBox_P4->setMinimum(0);
    ui->spinBox_P4->setValue(180);
    ui->spinBox_P4->setSingleStep(5);

    // set graph
    QFont legendFont = font();
    // plot in time domain
    ui->plot_time->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);//随意活动
    ui->plot_time->legend->setVisible(true);
    legendFont.setPointSize(8);
    ui->plot_time->legend->setFont(legendFont);
    ui->plot_time->legend->setSelectedFont(legendFont);
    ui->plot_time->legend->setSelectableParts(QCPLegend::spItems);
    ui->plot_time->xAxis->setRange(0, 1 << 10);
    ui->plot_time->yAxis->setLabel("Amplitude(mV)");
    ui->plot_time->yAxis->setRange(-2050, 2150);
    ui->plot_time->xAxis->setLabel("time(ns)");
    ui->plot_time->clearGraphs();
    ui->plot_time->addGraph();

    ui->plot_time->graph()->setLineStyle(QCPGraph::lsLine);//使用线连接起来
    ui->plot_time->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));//节点绘制风格
    ui->plot_time->graph()->setPen(QPen(Qt::black));
    ui->plot_time->graph()->setData(wave_data);
    ui->plot_time->graph()->setName("时域波形");
    ui->plot_time->axisRect()->setupFullAxesBox();

    // plot in freq domain
    ui->plot_freq->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);//随意活动
    ui->plot_freq->legend->setVisible(true);
    legendFont.setPointSize(12);
    ui->plot_freq->legend->setFont(legendFont);
    ui->plot_freq->legend->setSelectedFont(legendFont);
    ui->plot_freq->legend->setSelectableParts(QCPLegend::spItems);
    ui->plot_freq->xAxis->setRange(0, 1 << 10);
    ui->plot_freq->yAxis->setLabel("Amplitude(mV)");
    ui->plot_freq->yAxis->setRange(0, 2000);
    ui->plot_freq->xAxis->setLabel("Frequency(Hz)");
    ui->plot_freq->clearGraphs();
    ui->plot_freq->addGraph();

    ui->plot_freq->graph()->setLineStyle(QCPGraph::lsLine);//使用线连接起来
    ui->plot_freq->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));//节点绘制风格
    ui->plot_freq->graph()->setPen(QPen(Qt::black));
    ui->plot_freq->graph()->setData(freq_data);
    ui->plot_freq->graph()->setName("频谱");
    ui->plot_freq->axisRect()->setupFullAxesBox();

    QFont textFont("Microsoft yaHei", 16, 75);
    ui->textEdit->setFont(textFont);
    ui->textEdit->insertPlainText("program!");
}

void MainWindow::startTimer()
{
    // 500ms
    std::cout << "start" << std::endl;
    timer_plot.start(500);
}
void MainWindow::stopTimer()
{
    timer_plot.stop();
    std::cout << "stop" << std::endl;
}

void MainWindow::newConfig(int value)
{
//    if(value != mode){
//        mode = value;
//        std::cout << mode;
//        // return;
//    }
    Q_UNUSED(value);
    QList<int> mode, amp, freq, pha;

    mode.append(ui->comboBox_1->currentIndex());
    mode.append(ui->comboBox_2->currentIndex());
    mode.append(ui->comboBox_3->currentIndex());
    mode.append(ui->comboBox_4->currentIndex());

    amp.append(ui->spinBox_A1->value());
    amp.append(ui->spinBox_A2->value());
    amp.append(ui->spinBox_A3->value());
    amp.append(ui->spinBox_A4->value());

    freq.append(ui->spinBox_F1->value());
    freq.append(ui->spinBox_F2->value());
    freq.append(ui->spinBox_F3->value());
    freq.append(ui->spinBox_F4->value());

    pha.append(ui->spinBox_P1->value());
    pha.append(ui->spinBox_P2->value());
    pha.append(ui->spinBox_P3->value());
    pha.append(ui->spinBox_P4->value());
    emit newSig(mode, amp, freq, pha);
    // renew config to FPGA
}

void MainWindow::replot()
{
    wave_data->clear();
    for(int i = 0; i < LENGTH; i++){
        wave_data->add(QCPGraphData((double)i, (double)y_data[i]));
    }
    ui->plot_time->replot()  ;//重绘

    freq_data->clear();
    for(int i = 0; i < HALFLEN; i++){
        freq_data->add(QCPGraphData((double)i, f_data[i]));
    }
    ui->plot_freq->replot()  ;//重绘
    timer_plot.stop();
    // std::cout << "plot";
}

void MainWindow::textEdt(QString log){
    ui->textEdit->clear();
    ui->textEdit->insertPlainText(log);
}
