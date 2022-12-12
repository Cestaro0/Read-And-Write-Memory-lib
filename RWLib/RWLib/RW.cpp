#include "RW.h"

void ReadWriteLib::ToolKit::ReadMemoryOpcodes(HANDLE hProc, uintptr_t moduleBase, std::vector<unsigned int>x32PatchAddress, std::vector<uint8_t>* bbyte)
{
    uintptr_t val = 0;

    for (unsigned int x : x32PatchAddress)
    {
        val = moduleBase + x;
        uint8_t local = 0;

        ReadProcessMemory(hProc, (void*)val, &local, sizeof(uint8_t), 0);

        (*bbyte).push_back(local);
    }
}

void ReadWriteLib::ToolKit::WriteMemoryOpcodes(HANDLE hProc, uintptr_t moduleBase, std::vector<unsigned int>x32PatchAddress, std::vector<uint8_t*> x32assemblyOpcodes)
{
    uintptr_t val = 0;

    for (int i = 0; i < static_cast<int>(x32assemblyOpcodes.size()); i++)
    {
        val = moduleBase + x32PatchAddress.at(i);

        WriteProcessMemory(hProc, (void*)val, &x32assemblyOpcodes.at(i), sizeof(unsigned char), 0);
    }
}

uintptr_t ReadWriteLib::ToolKit::GetModuleBaseAddres(DWORD Pid, const wchar_t* moduleName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, Pid);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, moduleName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;

                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }

    CloseHandle(hSnap);

    return modBaseAddr;
}

DWORD ReadWriteLib::ToolKit::GetPID(const wchar_t* processName)
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    do {
        if (wcscmp(pe32.szExeFile, processName) == 0)
        {
            CloseHandle(hProcessSnap);

            return pe32.th32ProcessID;
        }

    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    return 0;
}

HANDLE ReadWriteLib::ToolKit::OpenProcessByName(const wchar_t* processName)
{
    return OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetPID(processName));
}
