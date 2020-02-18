#include <Windows.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#pragma pack(push, 1)

typedef struct _BOOT_NTFS
{
    BYTE    jump[3];
    BYTE    name[8];
    UINT16  sec_size;
    UINT16  secs_cluster;
    BYTE    zeros_0[6];
    UINT16  media_desc;
    BYTE    zeros_1;
    UINT16  secs_track;
    UINT16  num_heads;
    BYTE    zeros_2[8];
    UINT32  defaultValue;
    UINT64  num_secs;
    UINT64  LCNofMFT;
    UINT64  LCNofMFTMirr;
    UINT32  clustersPerMFT;
    UINT32  clustersPerIndex;
    UINT64  volumeSerialNumber;
} BOOT_NTFS;

#pragma pack(pop)

int main()
{
    const WCHAR* fileName = L"\\\\.\\H:";

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
        perror("Error: ");
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
        perror("Error: ");
    }

    BYTE dataBuffer[1024];

    DWORD bytesRead;

    bool readResult = ReadFile(
        fileHandle,
        dataBuffer,
        1024,
        &bytesRead,
        NULL
    );

    if (!readResult || bytesRead != 1024)
    {
        perror("Error: ");
    };

    BOOT_NTFS* pBootRecord = reinterpret_cast<BOOT_NTFS*>(dataBuffer);

    cout << "File system type: " << pBootRecord->name << endl;
    cout << "Sector size: " << pBootRecord->sec_size << endl;
    cout << "Sectors per cluster: " << pBootRecord->secs_cluster << endl;
    cout << "Volume descriptor: " << pBootRecord->media_desc << endl;
    cout << "Sectors per track: " << pBootRecord->secs_track << endl;
    cout << "Number of heads: " << pBootRecord->num_heads << endl;
    cout << "Number of sectors: " << pBootRecord->num_secs << endl;
    cout << "First cluster of MFT: " << pBootRecord->LCNofMFT << endl;
    cout << "First cluster of MFT mirror: " << pBootRecord->LCNofMFTMirr << endl;
    cout << "Clusters per MFT Record: " << pBootRecord->clustersPerMFT << endl;
    cout << "Clusters per Index Record: " << pBootRecord->clustersPerIndex << endl;
    cout << "Volume serial number: " << pBootRecord->volumeSerialNumber << endl;

    CloseHandle(fileHandle);

    return 0;
};