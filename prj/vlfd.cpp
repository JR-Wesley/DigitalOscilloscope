#include "vlfd.h"
#include <iostream>
using namespace std;

#define LENGTH 1 << 10
#define HALFLEN 1 << 9
extern int y_data[LENGTH];
extern double f_data[HALFLEN];
const auto bufferLength = LENGTH;

// initial
fpga::VLFD::VLFD(QObject *parent)
    : QObject{parent}
{
    mode = 4;
    prog = 0;
    WriteBuffer = new uint16_t[bufferLength];
    ReadBuffer = new uint16_t[bufferLength];
//    memset(WriteBuffer, 0, data_size * sizeof(WORD));
//    memset(ReadBuffer, 0, data_size * sizeof(WORD));

    test_io_open = (myIOOpen)GetProcAddress(hDLL, "VLFD_IO_Open");
    test_get_error = (GetError)GetProcAddress(hDLL, "VLFD_GetLastErrorMsg");
    test_program = (myProgram)GetProcAddress(hDLL, "VLFD_ProgramFPGA");
    test_io_close = (myIOClose)GetProcAddress(hDLL, "VLFD_IO_Close");
    test_write_read_data = (myWriteReadData)GetProcAddress(hDLL, "VLFD_IO_WriteReadData");
    hDLL = LoadLibrary(L"D:\\prj\\qt\\prj\\fde\\DLL\\win\\x64\\VLFD.dll");
    if (!hDLL) {
        cout << "No Library\n";
//        emit log("log Library");
//        return -1;
    }
}

fpga::VLFD::~VLFD()
{
    delete[] WriteBuffer;
    delete[] ReadBuffer;
    if (hDLL)
        FreeLibrary(hDLL);
}

void fpga::VLFD::progBit()
{
    test_io_open = (myIOOpen)GetProcAddress(hDLL, "VLFD_IO_Open");
    test_get_error = (GetError)GetProcAddress(hDLL, "VLFD_GetLastErrorMsg");
    test_program = (myProgram)GetProcAddress(hDLL, "VLFD_ProgramFPGA");
    test_io_close = (myIOClose)GetProcAddress(hDLL, "VLFD_IO_Close");
    test_write_read_data = (myWriteReadData)GetProcAddress(hDLL, "VLFD_IO_WriteReadData");
    hDLL = LoadLibrary(L"D:\\prj\\qt\\prj\\fde\\DLL\\win\\x64\\VLFD.dll");
    if (!hDLL) {
        cout << "No Library\n";
//        return -1;
    }

    if (!test_io_open || !test_get_error || !test_program || !test_io_close || !test_write_read_data)
    {
        cout << GetLastError() << endl;
//        return -1;
    }

    // int prog;
    switch(mode)
    {
    case(0):
        prog = test_program(iBoard, BitFileSINE);
        break;
    case(1):
        prog = test_program(iBoard, BitFileREC);
        break;
    case(2):
        prog = test_program(iBoard, BitFileRAW);
        emit log("NO BITFILE");
         break;
    case(3):
        prog = test_program(iBoard, BitFileRAN);
        emit log("NO BITFILE");
         break;
    case(4):
        prog = test_program(iBoard, BitFileINV);
        break;
    default:
        prog = 0;
        break;
    }

    if (!prog)
    {
        cout << "program error\n";
        cout << test_get_error(iBoard) << endl;
//        return -1;
    }
//    if (!test_io_open(iBoard, SerialNo))
//    {
//        cout << "open error\n";
//        cout << test_get_error(iBoard) << endl;
////        return -1;
//    }

    if(prog){
        switch(mode){
        case(0):
            emit log("SINE programmed!");
            break;
        case(1):
            emit log("REC programmed!");
            break;
        case(4):
            emit log("INV programmed!");
            break;
        default:
            break;
        }
    }else{
        emit log("FPGA program failed!");
        return;
    }

    if (!test_io_close(iBoard))
    {
        cout << "close error\n";

        cout << test_get_error(iBoard) << endl;
        // return -1;
    }
    cout << "\nsuccess\n";
}

void fpga::VLFD::fdeWnR()
{

    if (!test_io_open(iBoard, SerialNo)){
        pinCfg = 0;
    } else {
        test_io_close(iBoard);
    }
    if(pinCfg == 0){
        for (int i = 0; i < LENGTH; i++)
            y_data[i] = 0x00;

        emit proc();
    }
    if(1){
        switch(mode){
        case(0):
            sineTest();
            break;
        case(1):
            recTest();
            break;
        case(4):
            invTest();
            break;
        default:
            break;
        }
        emit proc();
    }else{
        emit log("not programmed!");
        return;
    }
}

int fpga::VLFD::invTest(){

    if (!test_io_open(iBoard, SerialNo))
    {
        cout << "open error\n";
        cout << test_get_error(iBoard) << endl;
//        return -1;
    }

    printf("index\tinput\tdata\toutput\n");

    // map of Buffer is shown in xml
    for (int clock = 0; clock < LENGTH; clock++)
    {
        if (clock < 5){
            WriteBuffer[0] = 0x00; // reset = 0
            WriteBuffer[1] = 0x00; // reset = 0
            WriteBuffer[2] = 0 << 12; // reset = 0
            WriteBuffer[3] = 0x00; // reset = 0
        }
        else
        {
            WriteBuffer[0] = clock;
            WriteBuffer[1] = 2 * clock;
            WriteBuffer[2] = 1 << 12;
            WriteBuffer[3] = 2 * clock;
        }

        uint16_t data = WriteBuffer[0] >> 1;

        for (int i = 0; i < data_size; i++)
            ReadBuffer[i] = 0x00;

        if (!test_write_read_data(iBoard, WriteBuffer, ReadBuffer, data_size))
        {
            cout << "write or read error\n";
            cout << test_get_error(iBoard) << endl;
            return -1;
        }

        printf("[%04X]\t[%04X]\t[%04X]\t[%04X]\t",
            WriteBuffer[0], WriteBuffer[1], WriteBuffer[2], WriteBuffer[3]);
        printf("[%04X]\t[%04X]\t[%04X]\t[%04X]\n",
            ReadBuffer[0], ReadBuffer[1], ReadBuffer[2], ReadBuffer[3]);
         printf("[%d]\t[%04X]\t[%04X]\t[%04X]\n",
            clock, WriteBuffer[0], data, ReadBuffer[0]);
        y_data[clock] = data;
     }

    if (!test_io_close(iBoard))
    {
        cout << "close error\n";

        cout << test_get_error(iBoard) << endl;
        return -1;
    }
    cout << "\nsuccess\n";
    return 0;
}

void fpga::VLFD::sineTest()
{
    if (!test_io_open(iBoard, SerialNo))
    {
        cout << "open error\n";
        cout << test_get_error(iBoard) << endl;
//        return -1;
    }

    printf("index\tinput\tdata\toutput\n");

    unsigned long long temp;
    temp = pinCfg;
    // map of Buffer is shown in xml
    for (int clock = 0; clock < (LENGTH) + 14; clock++)
    {
        if (clock < 2)
            WriteBuffer[0] = 0x00; // reset = 0
        else if(clock == 2)
        {
            WriteBuffer[0] = temp;
            WriteBuffer[1] = temp >> 16;
            WriteBuffer[2] = temp >> 32;
            WriteBuffer[3] = temp >> 48;
        }

        for (int i = 0; i < data_size; i++)
            ReadBuffer[i] = 0x00;

        if (!test_write_read_data(iBoard, WriteBuffer, ReadBuffer, data_size))
        {
            cout << "write or read error\n";
            cout << test_get_error(iBoard) << endl;
            return ;
        }

//        printf("[%04X]\t[%04X]\t[%04X]\t[%04X]\t",
//            WriteBuffer[0], WriteBuffer[1], WriteBuffer[2], WriteBuffer[3]);
//        printf("[%04X]\t[%04X]\t[%04X]\t[%04X]\n",
//            ReadBuffer[0], ReadBuffer[1], ReadBuffer[2], ReadBuffer[3]);
        printf("[%d]\t[%04X]\t[%04X]\n",
            clock, WriteBuffer[0], ReadBuffer[0]);
        y_data[clock] = ReadBuffer[0];
     }

    if (!test_io_close(iBoard))
    {
        cout << "close error\n";

        cout << test_get_error(iBoard) << endl;
        return ;
    }
    cout << "\nsuccess\n";
    return ;
}

void fpga::VLFD::recTest()
{
    if (!test_io_open(iBoard, SerialNo))
    {
        cout << "open error\n";
        cout << test_get_error(iBoard) << endl;
//        return -1;
    }

    printf("index\tinput\tdata\toutput\n");

    unsigned long long temp;
    temp = pinCfg;
    // map of Buffer is shown in xml
    for (int clock = 0; clock < (LENGTH) + 14; clock++)
    {
        if (clock < 2)
            WriteBuffer[0] = 0x00; // reset = 0
        else if(clock == 2)
        {
            WriteBuffer[0] = temp;
            WriteBuffer[1] = temp >> 16;
            WriteBuffer[2] = temp >> 32;
            WriteBuffer[3] = temp >> 48;
        }

        for (int i = 0; i < data_size; i++)
            ReadBuffer[i] = 0x00;

        if (!test_write_read_data(iBoard, WriteBuffer, ReadBuffer, data_size))
        {
            cout << "write or read error\n";
            cout << test_get_error(iBoard) << endl;
            return ;
        }

//        printf("[%04X]\t[%04X]\t[%04X]\t[%04X] ",
//            WriteBuffer[0], WriteBuffer[1], WriteBuffer[2], WriteBuffer[3]);
//        printf("[%04X]\t[%04X]\t[%04X]\t[%04X]\n",
//            ReadBuffer[0], ReadBuffer[1], ReadBuffer[2], ReadBuffer[3]);
        printf("[%d]\t[%04X]\t[%04X]\n",
            clock, WriteBuffer[0], ReadBuffer[0]);
        y_data[clock] = ReadBuffer[0];
     }

    if (!test_io_close(iBoard))
    {
        cout << "close error\n";

        cout << test_get_error(iBoard) << endl;
        return ;
    }
    cout << "\nsuccess\n";
    return ;

}


void fpga::VLFD::RXTX()
{
    for(int i = 0; i < LENGTH; i++)
    {
        WriteBuffer[0] = y_data[i];
        if (!test_write_read_data(iBoard, WriteBuffer, ReadBuffer, data_size))
        {
            cout << "write or read error\n";
            cout << test_get_error(iBoard) << endl;
            // return -1;
        }
        printf("[%d]\t[%04X]\t[%04X]\n",
            i, WriteBuffer[0], ReadBuffer[0]);
    }
}

void fpga::VLFD::progCfg(int value)
{
    mode = value;
}

void fpga::VLFD::cfg(int wave, int amp, int freq, int pha)
{
    if(wave != (mode + 1)){
        pinCfg = 0;
        return;
    }
    switch(mode)
    {
    case(0):
        pinCfg = amp + (freq << DW) + (1 << (DW +AW));
        break;
    case(1):
        pinCfg = amp + (freq << DW) + (pha << (DW +AW)) + (1 << (DW + AW + AW));
        break;
    case(4):
        pinCfg = 1 + (0 << 1);
        break;
    default:
        pinCfg = 0;
        break;
    }
}
