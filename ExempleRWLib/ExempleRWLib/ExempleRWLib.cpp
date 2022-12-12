/*
 	Test based on the use of functions from the "Bypass-CrackMe-Cruehead" repository, 
	with an example of how to make a simple crack in the "CrackMe" cruehead, through 
	the "rw" function library. 
											By: Cestaro
		Enjoy :)

		
	STEPS:
		1. Properties > VC directories > include directories - link to exemple: "C:\user\lib\include"
		2. Properties > VC directories > include library - link to exemple: "C:\user\lib\lib"

	Functions lib:
		1. GetPid							(ReadWriteLib::ToolKit::GetPID)
		2. OpenProcessByName				(ReadWriteLib::ToolKit::OpenProcessByName)
		3. GetModuleBaseAddress				(ReadWriteLib::ToolKit::GetModuleBaseAddress)
		4. ReadMemoryOpcodes				(ReadWriteLib::ToolKit::ReadMemoryOpcodes)
		5. WriteMemoryOpcodes				(ReadWriteLib::ToolKit::WriteMemoryOpcodes)


	Base references:
		https://github.com/keowu/KethoMemoryToolKit - codes 
		https://learn.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-static-library-cpp?view=msvc-170 - create static lib

*/

#include <iostream>
#include "RW.h"

#pragma region globals
	std::vector<unsigned int> x32PatchAddress = { 0x1243 };
	std::vector<unsigned int> x32AssemblyOpcodes = { 0xEB };
	std::vector<uint8_t> bbyte = { 0 };
#pragma endregion 

int main(void)
{
	const wchar_t ProcName[] = L"CRACKME.EXE";

	DWORD PID = ReadWriteLib::ToolKit::GetPID(ProcName);

	uintptr_t ModuleBase = ReadWriteLib::ToolKit::GetModuleBaseAddress(PID, ProcName);

	HANDLE hProc = ReadWriteLib::ToolKit::OpenProcessByName(ProcName);

	ReadWriteLib::ToolKit::ReadMemoryOpcodes(hProc, ModuleBase, x32PatchAddress, &bbyte);

	for (int i = 0; i < bbyte.size(); i++)
	{
		printf("%0.2X", bbyte.at(i));
	}

	bbyte.clear();

	ReadWriteLib::ToolKit::WriteMemoryOpcodes(hProc, ModuleBase, x32PatchAddress, x32AssemblyOpcodes);

	printf("\nSuccess!!\n");
}
