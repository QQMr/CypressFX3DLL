#include "stdafx.h"
#include "CyAPIWrapper.h"
#include <stdio.h>


#define     VENDOR_CMD_START_TRANSFER           0xB5

CyAPIWrapper::~CyAPIWrapper()
{

}

CyAPIWrapper::CyAPIWrapper(int vendorID, int proID)
{
    VENDOR_ID = vendorID;
    PRODUCT_ID = proID;
}

CyAPIWrapper::CyAPIWrapper()
{
}

bool CyAPIWrapper::ConnectDevice()
{
    USBDevice = new CCyUSBDevice(NULL, CYUSBDRV_GUID, true);

    if (USBDevice == NULL) return false;

    int n = USBDevice->DeviceCount();

    /////////////////////////////////////////////////////////////////
// Walk through all devices looking for VENDOR_ID/PRODUCT_ID
// We No longer got restricted with vendor ID and Product ID.
// Check for vendor ID and product ID is discontinued.
///////////////////////////////////////////////////////////////////
    for (int i = 0; i < n; i++)
    {
 
        stringstream  stringStream;
        USBDevice->Open(i);
        strDeviceData = "";
        string strTemp = string(USBDevice->FriendlyName);
        strDeviceData = strDeviceData + "(0x";// string::Concat(strDeviceData, "(0x");
        stringStream << setfill('0') << setw(4) << right << hex << USBDevice->VendorID;
        strDeviceData = strDeviceData + stringStream.str();// String::Concat(strDeviceData, USBDevice->VendorID.ToString("X4"));
        strDeviceData = strDeviceData + " - 0x";//  String::Concat(strDeviceData, " - 0x");
        stringStream.str("");
        stringStream.clear();
        stringStream << setfill('0') << setw(4) << right << hex << USBDevice->ProductID;
        strDeviceData = strDeviceData + stringStream.str();// String::Concat(strDeviceData, USBDevice->ProductID.ToString("X4"));
        strDeviceData = strDeviceData + ") ";// String::Concat(strDeviceData, ") ");
        strDeviceData = strDeviceData + strTemp;// String::Concat(strDeviceData, strTemp);
        cout << strDeviceData;

        if ((USBDevice->VendorID == VENDOR_ID) && (USBDevice->ProductID == PRODUCT_ID))
        {
            EndPt = USBDevice->EndPoints[1];
            break;
        }
        USBDevice->Close();
    }

    return USBDevice->IsOpen();
}

bool CyAPIWrapper::StartSampleData()
{
    long BytesXferred = 0;
    unsigned long Successes = 0;
    unsigned long Failures = 0;
    int i = 0;

    ReceiveDataIndex = 0;
   

    StartThreadReceive();
    return Successes;
}

bool CyAPIWrapper::EndSampleData()
{
    long BytesXferred = 0;
    unsigned long Successes = 0;
    unsigned long Failures = 0;
    int i = 0;

    CCyControlEndPoint* ControlEndPt = (CCyControlEndPoint*)(USBDevice->EndPointOf(0));

    LONG dataLength = 0;
    ControlEndPt->Target = TGT_DEVICE;
    ControlEndPt->ReqType = REQ_VENDOR;
    // Vendor Command that is transmitted for starting the read.
    ControlEndPt->ReqCode = VENDOR_CMD_START_TRANSFER;
    ControlEndPt->Direction = DIR_TO_DEVICE;
    //// Send Value = 0 and Index = 0, to stop the transaction.
    ControlEndPt->Value = 0x0000;
    ControlEndPt->Index = 0;

    // Send vendor command now......
    Successes = ControlEndPt->XferData(NULL, dataLength, NULL);

    bStreaming = false;

    receiveThread->join();
    return Successes;
}

const char* CyAPIWrapper::DeviceName()
{
    return strDeviceData.c_str();
}

void CyAPIWrapper::AbortXferLoop(int pending, PUCHAR* buffers, CCyIsoPktInfo** isoPktInfos, PUCHAR* contexts, OVERLAPPED inOvLap[])
{
    //EndPt->Abort(); - This is disabled to make sure that while application is doing IO and user unplug the device, this function hang the app.
    long len = EndPt->MaxPktSize * PPX;
    EndPt->Abort();

    for (int j = 0; j < QueueSize; j++)
    {
        if (j < pending)
        {
            EndPt->WaitForXfer(&inOvLap[j], TimeOut);
            /*{
                EndPt->Abort();
                if (EndPt->La       stError == ERROR_IO_PENDING)
                    WaitForSingleObject(inOvLap[j].hEvent,2000);
            }*/
            EndPt->FinishDataXfer(buffers[j], len, &inOvLap[j], contexts[j]);
        }

        CloseHandle(inOvLap[j].hEvent);

        delete[] buffers[j];
        delete[] isoPktInfos[j];
    }

    delete[] buffers;
    delete[] isoPktInfos;
    delete[] contexts;


    bStreaming = false;

}

void CyAPIWrapper::Display16Bytes(UCHAR*)
{
}

void CyAPIWrapper::Display(string)
{
}

void CyAPIWrapper::ShowStats(clock_t t, long bytesXferred, unsigned long successes, unsigned long failures)
{
   



    clock_t elapsed = clock();;

    long totMS = elapsed-t;
    if (totMS <= 0)	return;

    unsigned long XferRate = bytesXferred / totMS;

    // Convert to KBps
    XferRate = XferRate * 1000 / 1024;

    // Truncate last 1 or 2 digits
    int rounder = (XferRate > 2000) ? 100 : 10;
    XferRate = XferRate / rounder * rounder;

    if (XferRate > 625000)
        XferRate = 625000;

   // cout << "bytesXferred" << bytesXferred << std::endl;
   // cout << "successes" << successes << std::endl;
   // cout << "failures" << failures << std::endl;
    cout << "XferRate" << XferRate << std::endl;

}

void CyAPIWrapper::XferLoop()
{
    long BytesXferred = 0;
    unsigned long Successes = 0;
    unsigned long Failures = 0;
    int i = 0;

    // Allocate the arrays needed for queueing
    PUCHAR* buffers = new PUCHAR[QueueSize];
    CCyIsoPktInfo** isoPktInfos = new CCyIsoPktInfo * [QueueSize];
    PUCHAR* contexts = new PUCHAR[QueueSize];
    OVERLAPPED		inOvLap[MAX_QUEUE_SZ];
    //Allocate memory for Vector
    queueReceiveData.clear();
    for(int i=0;i< MAX_RECEIVE_LENGTH;i++)
        queueReceiveData.push_back(vector<UINT8>(EndPt->MaxPktSize * PPX));
    ReceiveDataIndex = 0;

    long len = EndPt->MaxPktSize * PPX; // Each xfer request will get PPX isoc packets

    EndPt->SetXferSize(len);

    // Allocate all the buffers for the queues
    for (i = 0; i < QueueSize; i++)
    {
        buffers[i] = new UCHAR[len];
        isoPktInfos[i] = new CCyIsoPktInfo[PPX];
        inOvLap[i].hEvent = CreateEvent(NULL, false, false, NULL);

        memset(buffers[i], 0xEF, len);
    }


    //-----------------------Use usb control to send vendor command------------------------//
    CCyControlEndPoint* ControlEndPt = (CCyControlEndPoint*)(USBDevice->EndPointOf(0));

    LONG dataLength = 0;
    ControlEndPt->Target = TGT_DEVICE;
    ControlEndPt->ReqType = REQ_VENDOR;
    // Vendor Command that is transmitted for starting the read.
    ControlEndPt->ReqCode = VENDOR_CMD_START_TRANSFER;
    ControlEndPt->Direction = DIR_TO_DEVICE;
    //// Send Value = 1 and Index = 0, to kick start the transaction.
    ControlEndPt->Value = 0x0001;
    ControlEndPt->Index = 0;

    // Send vendor command now......
    Successes = ControlEndPt->XferData(NULL, dataLength, NULL);
    //-----------------------Use usb control to send vendor command------------------------//

    
    clock_t t1 = clock();	// For calculating xfer rate

    // Queue-up the first batch of transfer requests
    for (i = 0; i < QueueSize; i++)
    {
        contexts[i] = EndPt->BeginDataXfer(buffers[i], len, &inOvLap[i]);
        if (EndPt->NtStatus || EndPt->UsbdStatus) // BeginDataXfer failed
        {
            //Display(String::Concat("Xfer request rejected. NTSTATUS = ", EndPt->NtStatus.ToString("x")));
            AbortXferLoop(i + 1, buffers, isoPktInfos, contexts, inOvLap);
            return;
        }
    }

    i = 0;

    // The infinite xfer loop.
    for (; bStreaming;)
    {
        long rLen = len;	// Reset this each time through because
        // FinishDataXfer may modify it

        if (!EndPt->WaitForXfer(&inOvLap[i], TimeOut))
        {
            EndPt->Abort();
            if (EndPt->LastError == ERROR_IO_PENDING)
                WaitForSingleObject(inOvLap[i].hEvent, 2000);
        }

        if (EndPt->Attributes == 1) // ISOC Endpoint
        {
            if (EndPt->FinishDataXfer(buffers[i], rLen, &inOvLap[i], contexts[i], isoPktInfos[i]))
            {
                CCyIsoPktInfo* pkts = isoPktInfos[i];
                for (int j = 0; j < PPX; j++)
                {
                    if ((pkts[j].Status == 0) && (pkts[j].Length <= EndPt->MaxPktSize))
                    {
                        BytesXferred += pkts[j].Length;

                        if (bShowData)
                            Display16Bytes(buffers[i]);

                        Successes++;
                    }
                    else
                        Failures++;

                    pkts[j].Length = 0;	// Reset to zero for re-use.
                    pkts[j].Status = 0;
                }

            }
            else
                Failures++;

        }

        else // BULK Endpoint
        {
            if (EndPt->FinishDataXfer(buffers[i], rLen, &inOvLap[i], contexts[i]))
            {
                Successes++;
                BytesXferred += rLen;

                if (bShowData)
                    Display16Bytes(buffers[i]);
                AddReceiveData(buffers[i], len);
            }
            else
                Failures++;
        }


        if (BytesXferred < 0) // Rollover - reset counters
        {
            BytesXferred = 0;
            t1 = clock();
        }

        // Re-submit this queue element to keep the queue full
        contexts[i] = EndPt->BeginDataXfer(buffers[i], len, &inOvLap[i]);
        if (EndPt->NtStatus || EndPt->UsbdStatus) // BeginDataXfer failed
        {
        
            string strDeviceData = "";
            string strTemp = "Xfer request rejected. NTSTATUS = ";
            strTemp = strTemp + std::to_string(EndPt->NtStatus);
            Display(strTemp);
            AbortXferLoop(QueueSize, buffers, isoPktInfos, contexts, inOvLap);
            return;
        }

        i++;

        if (i == QueueSize) //Only update the display once each time through the Queue
        {
            i = 0;
            ShowStats(t1, BytesXferred, Successes, Failures);
        }

    }  // End of the infinite loop

    // Memory clean-up
    AbortXferLoop(QueueSize, buffers, isoPktInfos, contexts, inOvLap);


}

long CyAPIWrapper::XferSize()
{
    return EndPt->MaxPktSize * PPX;
}

void CyAPIWrapper::AddReceiveData(UINT8* dataArray, unsigned size)
{
    vector<UINT8> dataVec;
    unsigned dataArraySize = size;// sizeof(dataArray) / sizeof(UINT8);

    memcpy(queueReceiveData[ReceiveDataIndex].data(), dataArray, dataArraySize);
    ReceiveDataIndex++;

    if (ReceiveDataIndex > MAX_RECEIVE_LENGTH)
    {
        ReceiveDataIndex = ReceiveDataIndex - 1;
        printf("ERROR");

    }
 //   dataVec.reserve(dataVec.size() + dataArraySize);
 //   copy(&dataArray[0], &dataArray[dataArraySize], back_inserter(dataVec));

 //   queueReceiveData.push_back(dataVec);
}

void ThreadWork(CyAPIWrapper * m)
{
    m->XferLoop();
}
void CyAPIWrapper::StartThreadReceive()
{
    bStreaming = true;
    receiveThread = new thread(ThreadWork, this);
}
