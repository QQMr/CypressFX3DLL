#pragma once
#include "stdafx.h"
#include <windows.h>

#undef GetObject
#include "CyAPI.h"
#include <dbt.h>

class CyAPIWrapper
{
	CCyUSBDevice* USBDevice;

	int	MAX_QUEUE_SZ = 64;
	int	VENDOR_ID = 0x04B4;
	int	PRODUCT_ID = 0x00F1;

public:
	CyAPIWrapper();
	 ~CyAPIWrapper();
};

