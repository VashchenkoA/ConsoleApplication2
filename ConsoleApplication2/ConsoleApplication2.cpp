#include <Windows.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main()
{
    const WCHAR *fileName = L"\\\\.\\H:";

    HANDLE fileHandle = CreateFileW(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Error" << endl;
    }

    LARGE_INTEGER sectorOffset;
    sectorOffset.QuadPart = 0;

    unsigned long currentPosition = SetFilePointer(
        fileHandle,
        sectorOffset.LowPart,
        &sectorOffset.HighPart,
        FILE_BEGIN
    );

    if (currentPosition != sectorOffset.LowPart)
    {
        cout << "Error" << endl;
    }

    CloseHandle(fileHandle);

    return 0;

};