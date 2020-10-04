#pragma once
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>

#include <iomanip>
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#undef GetObject
#include <CyAPI.h>
#include <dbt.h>
#include <thread>
#include <queue>
#include <vector>
using namespace std;

class CyAPIWrapper
{
	CCyUSBDevice* USBDevice;

	static const int	MAX_QUEUE_SZ = 64;
	int	VENDOR_ID = 0x04B4;
	int	PRODUCT_ID = 0x00F1;

	// These declared static because accessed from the static XferLoop method
// XferLoop is static because it is used as a separate thread.

	 CCyUSBEndPoint* EndPt;
	 int					PPX = 16;
	 int					QueueSize = 0x10;
	 int					TimeOut =1500;
	 bool					bShowData = false;
	 bool					bStreaming = false;
	 bool                 bDeviceRefreshNeeded;
	 bool                 bAppQuiting;
	 bool					bHighSpeedDevice;
	 bool					bSuperSpeedDevice;

	 thread *receiveThread;
	
	void StartThreadReceive();
	
	void AbortXferLoop(int pending, PUCHAR* buffers, CCyIsoPktInfo** isoPktInfos, PUCHAR* contexts, OVERLAPPED inOvLap[]);
	void Display16Bytes(UCHAR*);
	void Display(string);
	void ShowStats(clock_t t, long bytesXferred, unsigned long successes, unsigned long failures);
	

	void AddReceiveData(UINT8*, unsigned);

	const int MAX_RECEIVE_LENGTH = 10000;
	
	

	string strDeviceData;
public:
	CyAPIWrapper(int vendorID, int proID);
	CyAPIWrapper();
	 ~CyAPIWrapper();
	 bool ConnectDevice();
	 bool StartSampleData();
	 bool EndSampleData();
	 const char* DeviceName();
	 void XferLoop();
	 long XferSize();
	 volatile int ReceiveDataIndex = 0;
	 vector< vector<UINT8> > queueReceiveData;
};

