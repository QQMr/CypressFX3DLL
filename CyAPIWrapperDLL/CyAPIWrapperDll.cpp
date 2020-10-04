#include <utility>
#include <limits.h>
#include "CyAPIWrapperDll.h"
#include "..\CyAPIWrapper.h"

// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
void __cdecl fibonacci_init(
    const unsigned long long a,
    const unsigned long long b)
{
    index_ = 0;
    current_ = a;
    previous_ = b; // see special case when initialized
}

// Produce the next value in the sequence.
// Returns true on success, false on overflow.
bool __cdecl fibonacci_next()
{
    // check to see if we'd overflow result or position
    if ((ULLONG_MAX - previous_ < current_) ||
        (UINT_MAX == index_))
    {
        return false;
    }

    // Special case when index == 0, just return b value
    if (index_ > 0)
    {
        // otherwise, calculate next sequence value
        previous_ += current_;
    }
    std::swap(current_, previous_);
    ++index_;
    return true;
}

// Get the current value in the sequence.
unsigned long long __cdecl fibonacci_current()
{
    return current_;
}

// Get the current index position in the sequence.
unsigned __cdecl fibonacci_index()
{
    return index_;
}

CyAPIWrapper* mCyAPIWrapper;
void __cdecl init()
{
    mCyAPIWrapper = new CyAPIWrapper();
}


#define UNIT_DATA 4
int __cdecl GetReceiveData(UINT32 *data,int index)
{
    int size = mCyAPIWrapper->queueReceiveData[index].size();
    /*
    for (size_t i = 0; i < 4; i++)
    {
        data[i] = mCyAPIWrapper->queueReceiveData[index].data;
    }
    */

    memcpy(data, mCyAPIWrapper->queueReceiveData[index].data()
        , mCyAPIWrapper->queueReceiveData[index].size());


    
    return size/ UNIT_DATA;
}

int __cdecl GetXferLength()
{
    int size = mCyAPIWrapper->XferSize();
    return size;
}

int __cdecl GetReceiveTotalNumber()
{
    return mCyAPIWrapper->ReceiveDataIndex;
}

int __cdecl ConnectDevice()
{
    return mCyAPIWrapper->ConnectDevice();
}
int __cdecl StartSampleData()
{
    return mCyAPIWrapper->StartSampleData();
}
int __cdecl EndSampleData()
{
    return mCyAPIWrapper->EndSampleData();
}
const char* __cdecl  DeviceName()
{
    return mCyAPIWrapper->DeviceName();
}
