/*Dr.Hwang 2017/05*/
#ifndef _USBCOMPORT_H
#define _USBCOMPORT_H

#include <string>
#include <Windows.h>
#include <cstddef>
#include <cstdlib>
#include <cassert>

typedef std::string porttype;

typedef unsigned long ulong;
typedef unsigned char uchar;


class USBPort
{
public:
	USBPort(
		const porttype portNum = "COM",		// PORT
		DWORD baudRate = 115200,			// BAUD RATE
		BYTE  byteSize = 8,				// DATA BITS
		BYTE  parityBit = NOPARITY,		// PARITY
		BYTE  stopBit =ONE5STOPBITS		// STOP
		);

	~USBPort();

public:

	bool OpenComm();										// OPEN PORT
	void CloseComm();										// CLOSE PORT
	bool WriteToComm(BYTE data[], DWORD dwLegnth);			// SEND DATA
	bool ReadFromComm(int buffer[], DWORD dwLength);		// GET  DATA
	bool ReadFromComm_NeuroFlex(double buffer[], int PackageSize);		//size for neuroflex
	bool ReadFromComm_NeuroFlex_2(double buffer[], int PackageSize,int end_indx);		
	bool ReadFromComm_NeuroFlex_3(double buffer1[], double buffer2[],int PackageSize,int end_indx);
private:

	HANDLE m_hComm;
	porttype m_portNum;
	DWORD m_dwBaudRate;
	BYTE  m_byteSize;
	BYTE  m_parityBit;
	BYTE  m_stopBit;
	bool  m_bOpen;				// PORT SWITCH FLAG
	
private:

	enum BufferSize
	{
		MIN_BUFFER_SIZE = 256,
		BUFFER_SIZE = 512,
		MAX_BUFFER_SIZE = 1024
	};

	void setPortNum(const porttype &portNum)
	{
		this->m_portNum = portNum;
	}

	void setBaudRate(const ulong baudRate)
	{
		this->m_dwBaudRate = baudRate;
	}

	void setByteSize(const uchar byteSize)
	{
		this->m_byteSize = byteSize;
	}

	void setParityBit(const uchar parityBit)
	{
		this->m_parityBit = parityBit;
	}

	void setStopBit(const uchar stopBit)
	{
		this->m_stopBit = stopBit;
	}

public:
	porttype getPortNum() { return m_portNum; }

	ulong getBaudRate() { return m_dwBaudRate; }

	uchar getByteSize() { return m_byteSize; }

	uchar getParityBit() { return m_parityBit; }

	uchar getStopBit() { return m_stopBit; }
};




#endif