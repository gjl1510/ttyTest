/*************************************************************************
	> File Name: main.cpp
	> Author: gjl1510
	> Mail: gjl1510@126.com 
	> Created Time: 2016年03月30日 星期三 11时41分16秒
 ************************************************************************/

#include<iostream>
#include <iomanip>
#include"SerialPort.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
using namespace std;

#define BUF_SIZE                                                2048

class ttyTest:public SerialPort{
    private:
        char buf[BUF_SIZE];
        char cache[BUF_SIZE];
        char protocol[256];
    public:
        ttyTest(string name, int baudrate)
        {
            PortName = name;
            BaudRate = baudrate;
            BaseStream = -1;
        }
        ~ttyTest(){}
    
        int a2i(char *str)
        {
        	char *p = str;
        	int ret = 0;
        	if (!p)
        		return ret;
        	while (*p != '\0')
        	{
        		switch(*p)
        		{
        			case '0':
        				break;

        			case '1':
        				ret += 1;
        				break;
        			
        			case '2':
        				ret += 2;
        				break;

        			case '3':
        				ret += 3;
        				break;
        			
        			case '4':
        				ret += 4;
        				break;
        			
        			case '5':
        				ret += 5;
        				break;
        			
        			case '6':
        				ret += 6;
        				break;
        			
        			case '7':
        				ret += 7;
        				break;
        			
        			case '8':
        				ret += 8;
        				break;
        			
        			case '9':
        				ret += 9;
        				break;

        			case 'a':
        			case 'A':
        				ret += 0x0A;
        				break;

        			case 'b':
        			case 'B':
        				ret += 0x0B;
        				break;

        			case 'c':
        			case 'C':
        				ret += 0x0C;
        				break;

        			case 'd':
        			case 'D':
        				ret += 0x0D;
        				break;

        			case 'e':
        			case 'E':
        				ret += 0x0E;
        				break;

        			case 'f':
        			case 'F':
        				ret += 0x0F;
        				break;

        			case ' ':
        				return (ret >> 4);
        		}
        		ret = (ret << 4);
        		p++;
        	}
        	return (ret >> 4);
        }

        void write_tty()
        {
        	int i, no;
        	char *p;
        	char *pEnd;

        	while (1)
        	{
        		p = fgets((char*)cache, sizeof(cache), stdin);
        		if (!p)continue;

        		no = 0;
        		pEnd = p + strlen(p) - 1;
        		while(*pEnd == '\r' || *pEnd == '\n')
        			*pEnd-- = '\0';
        		
        		if (*pEnd++ != ' ')
        		{
        			*pEnd++ = ' ';
        			*pEnd = '\0';
        		}

           		if (*p == ' ')
        		{
        			p++;
        			continue;
        		}

        		while(*p != '\0')
        		{
        			protocol[no++] = (unsigned char)a2i(p);

        			while (*p != ' ' && *p != '\0')
        				p++;
        			
        			if (*p == ' ')
        			{
        				p++;
        				continue;
        			}
                }
                Write(protocol, 0, no);
        		cout<<"write:";
        		for (i = 0; i < no; i++)
                    printf("%02X ", protocol[i]);
        		cout<<endl;
        	}
        }

        void read_tty()
        {
            int ret, i;
        	while (1) 
        	{
                ret = Read(buf, 0, BUF_SIZE);
                if (ret > 0)
                {
        			cout<<"read:"<<ret<<endl;
        			for (i = 0; i < ret; i++)
                        printf("%02X ", buf[i]);
        			cout<<endl;
                }
                //sleep(1);
        	};
        }
};

static void Usage()
{
	cout<<"Usage: ttyTest [-h?v] -f DEVICE -b BAUDRATE [-w]"<<endl;
	cout<<"        -h?            help"<<endl;
	cout<<"        -f device      open and use the specified DEVICE"<<endl;
	cout<<"        -b baudrate    set baudrate for the DEVICE"<<endl;
	cout<<"        -w             write data to the DEVICE"<<endl;
	cout<<"Example: ttyTest -f /dev/ttySAC1 -b 115200"<<endl;
}

int main(int argc, char *argv[])
{
	char *tty_dev = NULL;
	unsigned int baudrate = 115200;
	int c = 0;
	int writeflag = 0;
	/* Check the passed arg */
	while ((c = getopt(argc, argv, "h?wf:b:")) > 0)
	{
		switch (c) 
		{
    		case 'f':
    			tty_dev = optarg;
    			break;
    		case 'b':
    			baudrate = atoi(optarg);
    			break;
    		case 'w':
    			writeflag = 1;
    			break;
    		case 'h':
    		case '?':
    		default:
            	Usage();
                return 0;
		}
	}
	if (!tty_dev)
	{
    	Usage();
        return 0;
    }
    string* dev = new string(tty_dev);
    ttyTest tty(*dev, baudrate);
    delete dev;
    dev = NULL;
    tty.Open();
	if (tty.BaseStream < 0) 
	{
		cout<<tty.BaseStream<<endl;
        return 0;
	}
	if (writeflag)
		tty.write_tty();
	else
        tty.read_tty();
	return 0;
}
