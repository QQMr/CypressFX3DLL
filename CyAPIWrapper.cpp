#include "stdafx.h"
#include "CyAPIWrapper.h"
#include <string>
#include <sstream>
#include <iostream>

#include <iomanip>
using namespace std;

CyAPIWrapper::~CyAPIWrapper()
{

}

CyAPIWrapper::CyAPIWrapper()
{
	USBDevice = new CCyUSBDevice(NULL, CYUSBDRV_GUID, true);

	if (USBDevice == NULL) return;

	int n = USBDevice->DeviceCount();

    /////////////////////////////////////////////////////////////////
// Walk through all devices looking for VENDOR_ID/PRODUCT_ID
// We No longer got restricted with vendor ID and Product ID.
// Check for vendor ID and product ID is discontinued.
///////////////////////////////////////////////////////////////////
    for (int i = 0; i < n; i++)
    {
        //if ((USBDevice->VendorID == VENDOR_ID) && (USBDevice->ProductID == PRODUCT_ID)) 
        //    break;
        stringstream  stringStream;
        USBDevice->Open(i);
        string strDeviceData = "";
        string strTemp = string(USBDevice->FriendlyName);
        strDeviceData = strDeviceData+ "(0x";// string::Concat(strDeviceData, "(0x");
        stringStream << setfill('0') << setw(4) << right << hex << USBDevice->VendorID;
        strDeviceData = strDeviceData + stringStream.str();// String::Concat(strDeviceData, USBDevice->VendorID.ToString("X4"));
        strDeviceData = strDeviceData + " - 0x";//  String::Concat(strDeviceData, " - 0x");
        stringStream.str("");
        stringStream.clear();
        stringStream << setfill('0') << setw(4) << right << hex << USBDevice->ProductID;
        strDeviceData = strDeviceData + stringStream.str();// String::Concat(strDeviceData, USBDevice->ProductID.ToString("X4"));
        strDeviceData = strDeviceData + ") ";// String::Concat(strDeviceData, ") ");
        strDeviceData = strDeviceData + strTemp;// String::Concat(strDeviceData, strTemp);

    }
	
}