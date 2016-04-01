
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
	    virtual void Close(); //�رն˿����ӣ��� IsOpen ��������Ϊ false�����ͷ��ڲ� Stream ����
		virtual void DiscardInBuffer(); //�������Դ�����������Ľ��ջ����������ݡ�
		virtual void DiscardOutBuffer(); //�������Դ�����������Ĵ��仺���������ݡ�
		virtual void Dispose(); //�ͷ��� Component ʹ�õ�������Դ������ Component �̳С���
		virtual void Dispose(bool disposing); //�ͷ��� SerialPort ռ�õķ��й���Դ���������ͷ��й���Դ��(��� Component::Dispose(Boolean)��)
		virtual string* GetPortNames(); //��ȡ��ǰ������Ĵ��ж˿��������顣
		virtual void Open(); //��һ���µĴ��ж˿����ӡ�
		virtual int Read(char byte[], int offset, int count); //�� SerialPort ���뻺������ȡһЩ�ֽڲ�����Щ�ֽ�д���ֽ�������ָ����ƫ��������
		virtual int ReadByte(); //�� SerialPort ���뻺������ͬ����ȡһ���ֽڡ�
		virtual string ReadExisting(); //�ڱ���Ļ����ϣ���ȡ SerialPort ������������뻺�����������������õ��ֽڡ�
		virtual string ReadLine(); //һֱ��ȡ�����뻺�����е� NewLine ֵ��
		virtual string ReadTo(string text); //һֱ��ȡ�����뻺�����е�ָ�� value ���ַ�����
		virtual void Write(char byte[], int offset, int count); //ʹ�û������е����ݽ�ָ���������ֽ�д�봮�ж˿ڡ�
		virtual void Write(string text); //��ָ�����ַ���д�봮�ж˿ڡ�
		virtual void WriteLine(string text); //��ָ�����ַ����� NewLine ֵд�������������
	// ����
    public:
        int BaseStream; //��ȡ fd��
        int BaudRate; //��ȡ�����ô��в����ʡ�
        bool BreakState; //��ȡ�������ж��ź�״̬��
        int BytesToRead; //��ȡ���ջ����������ݵ��ֽ�����
        int BytesToWrite; //��ȡ���ͻ����������ݵ��ֽ�����
        bool CDHolding; //��ȡ�˿ڵ��ز�����е�״̬��
        bool CtsHolding; //��ȡ�����Է��͡��е�״̬��
        int DataBits; //��ȡ������ÿ���ֽڵı�׼����λ���ȡ�
        bool DsrHolding; //��ȡ�������þ��� (DSR) �źŵ�״̬��
        bool DtrEnable; //��ȡ������һ��ֵ����ֵ�ڴ���ͨ�Ź��������������ն˾��� (DTR) �źš�
        bool IsOpen; //��ȡһ��ֵ����ֵָʾ SerialPort ����Ĵ򿪻�ر�״̬��
        Parity parity; //��ȡ��������żУ����Э�顣
        string PortName; //��ȡ������ͨ�Ŷ˿ڣ����������������п��õ� COM �˿ڡ�
        int ReadBufferSize; //��ȡ������ SerialPort ���뻺�����Ĵ�С��
        int ReadTimeout; //��ȡ�����ö�ȡ����δ���ʱ������ʱ֮ǰ�ĺ�������
        int ReceivedBytesThreshold; //��ȡ������ DataReceived �¼�����ǰ�ڲ����뻺�����е��ֽ�����
        bool RtsEnable; //��ȡ������һ��ֵ����ֵָʾ�ڴ���ͨ�����Ƿ����������� (RTS) �źš�
        StopBits stopbits; //��ȡ������ÿ���ֽڵı�׼ֹͣλ����
        int WriteBufferSize; //��ȡ�����ô��ж˿�����������Ĵ�С��
        int WriteTimeout; //��ȡ������д�����δ���ʱ������ʱ֮ǰ�ĺ�������
    public:
        // {{Event_Begin(SerialPort)
        virtual void DataReceived();//ָʾ��ͨ���� SerialPort �����ʾ�Ķ˿ڽ��������ݡ�
        virtual void Disposed();//��ͨ������ Dispose �����ͷ����ʱ���������� Component �̳С���
        virtual void ErrorReceived();//ָʾ�� SerialPort �����ʾ�Ķ˿��Ϸ����˴���
        virtual void PinChanged();//ָʾ�� SerialPort �����ʾ�Ķ˿��Ϸ����˷������ź��¼���
        // }}Event_End;
    private:
        char *m_Receive;
        char *m_Send;
        int m_WriteBufferSize;
        int m_ReadBufferSize;
    private:
        void OnChangeBufferSize();
};
