//
// @Author: MorningXu 
// @Description:
// @Date: 2023-07-03
//

#pragma once

#include <string>
#include <vector>
#include <fcntl.h>   /*文件控制定义*/
#include <termios.h> /*POSIX 终端控制定义*/
#include <cstdio>   /*标准输入输出定义*/
#include <cstdlib>  /*标准函数库定义*/
#include <unistd.h>  /*Unix 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <memory>
#include <iostream>
#include <dirent.h>
#include <utility>


struct termios;

class SerialPort {
public:
    enum BaudRate {
        BR0 = 0000000,
        BR50 = 0000001,
        BR75 = 0000002,
        BR110 = 0000003,
        BR134 = 0000004,
        BR150 = 0000005,
        BR200 = 0000006,
        BR300 = 0000007,
        BR600 = 0000010,
        BR1200 = 0000011,
        BR1800 = 0000012,
        BR2400 = 0000013,
        BR4800 = 0000014,
        BR9600 = 0000015,
        BR19200 = 0000016,
        BR38400 = 0000017,
        BR57600 = 0010001,
        BR115200 = 0010002,
        BR230400 = 0010003,
        BR460800 = 0010004,
        BR500000 = 0010005,
        BR576000 = 0010006,
        BR921600 = 0010007,
        BR1000000 = 0010010,
        BR1152000 = 0010011,
        BR1500000 = 0010012,
        BR2000000 = 0010013,
        BR2500000 = 0010014,
        BR3000000 = 0010015,
        BR3500000 = 0010016,
        BR4000000 = 0010017
    };

    enum DataBits {
        DataBits5,
        DataBits6,
        DataBits7,
        DataBits8,
    };

    enum StopBits {
        StopBits1,
        StopBits2
    };

    enum Parity {
        ParityNone,
        ParityEven,
        PariteMark,
        ParityOdd,
        ParitySpace
    };

    struct OpenOptions {
        bool autoOpen;
        BaudRate baudRate;
        DataBits dataBits;
        StopBits stopBits;
        Parity parity;
        bool xon;
        bool xoff;
        bool xany;
        int vmin;
        int vtime;
    };

    static BaudRate BaudRateMake(unsigned long baudrate);

    static OpenOptions defaultOptions;

    explicit SerialPort(std::string path,OpenOptions options = defaultOptions);
    //打开串口
    bool open();

    bool open(const std::string &path, const OpenOptions &options);

    bool isOpen() const;

    //写
    int write(const void *data, int length);

    //读
    int read(void *data, int length);

    //串口及监听线程关闭
    void close();

    static std::vector<std::string> list();

    void setCounter(unsigned char counter);

protected:

    void termiosOptions(termios &tios, const OpenOptions &options);

    long getBaudRate(int baudRate);

private:
    std::string _path;
    OpenOptions _open_options;
    int _tty_fd{};
    bool _is_open = false;
    int _id;
    bool isHealth = true;
    unsigned char counter = 0;
};

bool operator==(const SerialPort::OpenOptions &lhs, const SerialPort::OpenOptions &rhs);

bool operator!=(const SerialPort::OpenOptions &lhs, const SerialPort::OpenOptions &rhs);

