#ifndef VLFD_H
#define VLFD_H

#include <QObject>
#include <Windows.h>
#include <QLibrary>     // 添加动态编译库DLL
#include <QVector>
#include <QString>

// Single clock run 64 bits data && WORDx4=64 bits
#define data_size 4

// 定义函数指针
typedef BOOL(WINAPI* myIOOpen)			(int, const char*);
typedef const char* (WINAPI* GetError)	(int);
typedef BOOL(WINAPI* myProgram)			(int, const char*);
typedef BOOL(WINAPI* myIOClose)			(int);
typedef BOOL(WINAPI* myWriteReadData)	(int, WORD*, WORD*, unsigned);

namespace fpga {
// @brief VLFD class
// used to process the read/write of the FPGA
class VLFD : public QObject
{
    Q_OBJECT
public:
    explicit VLFD(QObject *parent = nullptr);
    ~VLFD();
    void RXTX();

    HINSTANCE		hDLL;
    myIOOpen		test_io_open;
    GetError		test_get_error;
    myProgram		test_program;
    myIOClose		test_io_close;
    myWriteReadData test_write_read_data;
    int invTest();
    void sineTest();
    void recTest();

private:
    uint16_t *WriteBuffer;
    uint16_t *ReadBuffer;
    const int ClockCount = 0x1ff; // run 170 clock cycle
    const int iBoard = 0;
    const char* SerialNo = "F4YF-K2II-Y0Z0-AT05-F805-A478";
    //const char* BitFileINV = R"(D:\prj\qt\prj\fde_repo\hardware\FDE\NoUseFIFO_fde_dc.bit)";
    const char* BitFileINV = R"(D:\prj\qt\prj\fde_repo\hardware\inv.bit)";
    const char* BitFileSINE = R"(D:\prj\qt\prj\fde_repo\hardware\cor.bit)";
    const char* BitFileREC = R"(D:\prj\qt\prj\fde_repo\hardware\rec.bit)";
    const char* BitFileRAW = R"(D)";
    const char* BitFileRAN = R"(D)";
    const int DW = 12, AW = 14;
    unsigned long long pinCfg;
    int mode;
    int prog;

signals:
    void log(QString);
    void proc();
public slots:
// @brief Slot called to test inv
    void progCfg(int);
    void progBit();
    void cfg(int wave, int amp, int freq, int pha);
    void fdeWnR();
};

}

#endif // VLFD_H
