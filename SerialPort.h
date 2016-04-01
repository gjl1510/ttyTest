
#include<iostream>
using namespace std;


class SerialPort
{
	public:
        enum Parity{None,Odd,Even,Mark,Space};
        enum StopBits{NONE,One,Two,OnePointFive};
    public:
        SerialPort(string name="", int baudrate=115200, Parity _parity=None, int bits=8, StopBits _stopbits=One);
        virtual~SerialPort();
    public:
	    virtual void Close(); //关闭端口连接，将 IsOpen 属性设置为 false，并释放内部 Stream 对象。
		virtual void DiscardInBuffer(); //丢弃来自串行驱动程序的接收缓冲区的数据。
		virtual void DiscardOutBuffer(); //丢弃来自串行驱动程序的传输缓冲区的数据。
		virtual void Dispose(); //释放由 Component 使用的所有资源。（从 Component 继承。）
		virtual void Dispose(bool disposing); //释放由 SerialPort 占用的非托管资源，还可以释放托管资源。(替代 Component::Dispose(Boolean)。)
		virtual string* GetPortNames(); //获取当前计算机的串行端口名的数组。
		virtual void Open(); //打开一个新的串行端口连接。
		virtual int Read(char byte[], int offset, int count); //从 SerialPort 输入缓冲区读取一些字节并将那些字节写入字节数组中指定的偏移量处。
		virtual int ReadByte(); //从 SerialPort 输入缓冲区中同步读取一个字节。
		virtual string ReadExisting(); //在编码的基础上，读取 SerialPort 对象的流和输入缓冲区中所有立即可用的字节。
		virtual string ReadLine(); //一直读取到输入缓冲区中的 NewLine 值。
		virtual string ReadTo(string text); //一直读取到输入缓冲区中的指定 value 的字符串。
		virtual void Write(char byte[], int offset, int count); //使用缓冲区中的数据将指定数量的字节写入串行端口。
		virtual void Write(string text); //将指定的字符串写入串行端口。
		virtual void WriteLine(string text); //将指定的字符串和 NewLine 值写入输出缓冲区。
	// 属性
    public:
        int BaseStream; //获取 fd。
        int BaudRate; //获取或设置串行波特率。
        bool BreakState; //获取或设置中断信号状态。
        int BytesToRead; //获取接收缓冲区中数据的字节数。
        int BytesToWrite; //获取发送缓冲区中数据的字节数。
        bool CDHolding; //获取端口的载波检测行的状态。
        bool CtsHolding; //获取“可以发送”行的状态。
        int DataBits; //获取或设置每个字节的标准数据位长度。
        bool DsrHolding; //获取数据设置就绪 (DSR) 信号的状态。
        bool DtrEnable; //获取或设置一个值，该值在串行通信过程中启用数据终端就绪 (DTR) 信号。
        bool IsOpen; //获取一个值，该值指示 SerialPort 对象的打开或关闭状态。
        Parity parity; //获取或设置奇偶校验检查协议。
        string PortName; //获取或设置通信端口，包括但不限于所有可用的 COM 端口。
        int ReadBufferSize; //获取或设置 SerialPort 输入缓冲区的大小。
        int ReadTimeout; //获取或设置读取操作未完成时发生超时之前的毫秒数。
        int ReceivedBytesThreshold; //获取或设置 DataReceived 事件发生前内部输入缓冲区中的字节数。
        bool RtsEnable; //获取或设置一个值，该值指示在串行通信中是否启用请求发送 (RTS) 信号。
        StopBits stopbits; //获取或设置每个字节的标准停止位数。
        int WriteBufferSize; //获取或设置串行端口输出缓冲区的大小。
        int WriteTimeout; //获取或设置写入操作未完成时发生超时之前的毫秒数。
    public:
        // {{Event_Begin(SerialPort)
        virtual void DataReceived();//指示已通过由 SerialPort 对象表示的端口接收了数据。
        virtual void Disposed();//当通过调用 Dispose 方法释放组件时发生。（从 Component 继承。）
        virtual void ErrorReceived();//指示由 SerialPort 对象表示的端口上发生了错误。
        virtual void PinChanged();//指示由 SerialPort 对象表示的端口上发生了非数据信号事件。
        // }}Event_End;
    private:
        char *m_Receive;
        char *m_Send;
        int m_WriteBufferSize;
        int m_ReadBufferSize;
    private:
        void OnChangeBufferSize();
};
