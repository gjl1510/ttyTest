/*************************************************************************
	> File Name: SerialPort.cpp
	> Author: gjl1510
	> Mail: gjl1510@126.com 
	> Created Time: 2016年03月30日 星期三 11时42分12秒
 ************************************************************************/

#include"SerialPort.h"

#include <stdio.h>          /*标准输入输出定义*/
#include <stdlib.h>         /*标准函数库定义*/
#include <string.h>
#include <unistd.h>         /*Unix 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>          /*文件控制定义*/
#include <errno.h>          /*错误号定义*/
#include <sys/ioctl.h>
#include <signal.h>
#include <termios.h>        /*PPSIX 终端控制定义*/

SerialPort::SerialPort(string name, int baudrate, Parity _parity, int bits, StopBits _stopbits)
{
    // 传入参数初始化
    PortName = name;
    BaudRate = baudrate;
    parity = _parity;
    DataBits = bits;
    stopbits = _stopbits;
    // 其他变量初始化
    BaseStream = -1;
    BreakState = false;
    BytesToRead = 0;
    BytesToWrite = 0;
    CDHolding = false;
    CtsHolding = false;
    DsrHolding = false;
    DtrEnable = false;
    IsOpen = false;
    ReadBufferSize = 4096;
    ReadTimeout = 0;
    ReceivedBytesThreshold = 0;
    RtsEnable = false;
    WriteBufferSize = 0;
    WriteTimeout = 0;
    
    m_Receive = NULL;
    m_Send = NULL;
    m_WriteBufferSize = 0;
    m_ReadBufferSize = 0;
}

SerialPort::~SerialPort()
{
    if (IsOpen)
        Close();
}

void SerialPort::Close()
{
    if (BaseStream >= 0)
        close(BaseStream);
    BaseStream = -1;
    IsOpen = false;
    ReadBufferSize = 0;
    WriteBufferSize = 0;
}

void SerialPort::DiscardInBuffer()
{
    while(Read(m_Receive, 0, m_ReadBufferSize) == m_ReadBufferSize);
}

static unsigned int get_baud_rate(int baud)
{
    unsigned int speed = B0;
	switch (baud) 
	{
    	case 150:
    		speed = B150;
    		break;
    	case 200:
    		speed = B200;
    		break;
    	case 300:
    		speed = B300;
    		break;
    	case 600:
    		speed = B600;
    		break;
    	case 1200:
    		speed = B1200;
    		break;
    	case 1800:
    		speed = B1800;
    		break;
    	case 2400:
    		speed = B2400;
    		break;
    	case 4800:
    		speed = B4800;
    		break;
    	case 9600:
    		speed = B9600;
    		break;
    	case 19200:
    		speed = B19200;
    		break;
    	case 38400:
    		speed = B38400;
    		break;
    	case 57600:
    		speed = B57600;
    		break;
    	case 115200:
    		speed = B115200;
    		break;
    	case 230400:
    		speed = B230400;
    		break;
    	case 460800:
    		speed = B460800;
    		break;
    	case 500000:
    		speed = B500000;
    		break;
    	case 576000:
    		speed = B576000;
    		break;
    	default:
            break;
	}
    return speed;
}

static unsigned int get_data_bits(int bit)
{
    unsigned int cs = CS8;
    switch(bit)
    {
        case 5:
            cs = CS5;
            break;
        case 6:
            cs = CS6;
            break;
        case 7:
            cs = CS7;
            break;
        default:
            break;
    }
    return cs;
}

static void setup_tty(int tty, unsigned int baud)
{
	if (tty < 0)
		return;
    unsigned int speed = get_baud_rate(baud);
    if (speed == B0)
        return;
    unsigned int databits = get_data_bits(8);
	struct termios term;
	tcgetattr(tty, &term);
	term.c_iflag     = 0;
	term.c_oflag     = 0;
	term.c_lflag     = 0;
	term.c_cflag     &= ~(CSIZE | PARENB | INPCK);
	term.c_cflag     |= CLOCAL | databits;
	term.c_cc[VMIN]  = 1;
	term.c_cc[VTIME] = 0;
	cfsetispeed(&term, speed);
	cfsetospeed(&term, speed);
	tcsetattr(tty, TCSANOW, &term);
}

static void setup_tty_option(int tty, unsigned int data, unsigned int stop, unsigned int parity)
{
	if (tty < 0)
		return;
    unsigned int databits = get_data_bits(8);
    if (stop < 1 || stop > 2)
        return;
	struct termios term;
	tcgetattr(tty, &term);
	term.c_iflag     = 0;
	term.c_oflag     = 0;
	term.c_lflag     = 0;
	term.c_cflag     |= CLOCAL | CSIZE | databits;
    if (stop == 2)
        term.c_cflag |= CSTOPB;
    switch(parity)
    {
        case 0: // None
        	term.c_cflag &= ~(PARENB | INPCK);
            break;
        case 1: // Odd
            term.c_cflag |= (PARENB|PARODD|INPCK);
            break;
        case 2: // Even
            term.c_cflag |= (PARENB|INPCK);
        case 3: // Mark
            term.c_cflag |= INPCK;
            break;
        case 4: // Space
            term.c_cflag &= ~CSTOPB;
            term.c_cflag |= INPCK;
            break;
    }
	term.c_cc[VMIN]  = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(tty, TCSANOW, &term);
}

static void setup_tty_speed(int tty, unsigned int baud)
{
	if (tty < 0)
		return;
    unsigned int speed = get_baud_rate(baud);
    if (speed == B0)
        return;
	struct termios term;
	tcgetattr(tty, &term);
	cfsetispeed(&term, speed);
	cfsetospeed(&term, speed);
	tcsetattr(tty, TCSANOW, &term);

}
static int open_tty(const char *tty_dev)
{
    int tty_fd;

	if (!tty_dev)
		return -1;
	tty_fd = open(tty_dev, O_RDWR | O_NOCTTY);
	if (tty_fd < 0) 
		return -1;
    return tty_fd;
}


void SerialPort::Open()
{
    if (IsOpen)
        Close();
    if (PortName.empty())
        return;
    BaseStream = open_tty(PortName.c_str());
    if (BaseStream < 0)
        return;
    IsOpen = true;
	OnChangeBufferSize();
    if (BaudRate > 0)
    {
        if (DataBits == 8 && stopbits == One && parity == None)
        {
            setup_tty(BaseStream, BaudRate);
        }
        else
        {
            setup_tty_option(BaseStream, DataBits, stopbits, parity);
            setup_tty_speed(BaseStream, BaudRate);
        }
    }
}

int SerialPort::Read(char byte[], int offset, int count)
{
    return read(BaseStream, &byte[offset], count);
}

int SerialPort::ReadByte()
{
    int byte = 0;
    if (read(BaseStream, &byte, 1))
        return byte;
    return 0;
}

void SerialPort::Write(char byte[], int offset, int count)
{
    write(BaseStream, &byte[offset], count);
}

void SerialPort::OnChangeBufferSize()
{
    if (m_WriteBufferSize != WriteBufferSize)
    {
        delete m_Send;
        m_WriteBufferSize = WriteBufferSize;
        if (m_WriteBufferSize > 0)
            m_Send = new char[m_WriteBufferSize]();
        else
            m_Send = NULL;
    }
    if (m_ReadBufferSize != ReadBufferSize)
    {
        delete m_Send;
        m_ReadBufferSize = ReadBufferSize;
        if (m_ReadBufferSize > 0)
            m_Receive = new char[m_ReadBufferSize]();
        else
            m_Receive = NULL;
    }
}

void SerialPort::DiscardOutBuffer(){}
void SerialPort::Dispose(){}
void SerialPort::Dispose(bool disposing){}
string* SerialPort::GetPortNames()
{
	string *str = new string(PortName);
	return str;
}

string SerialPort::ReadExisting(){ return 0; }
string SerialPort::ReadLine(){ return 0; }
string SerialPort::ReadTo(string text){ return 0; }
void SerialPort::SerialPort::Write(string text){}
void SerialPort::SerialPort::WriteLine(string text){}
void SerialPort::DataReceived(){}
void SerialPort::Disposed(){}
void SerialPort::ErrorReceived(){}
void SerialPort::PinChanged(){}
