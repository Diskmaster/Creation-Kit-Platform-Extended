﻿// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

namespace CreationKitPlatformExtended
{
	namespace Utils
	{
		void __Assert(LPCSTR File, int Line, LPCSTR Format, ...)
		{
			CHAR buffer[2048] = { 0 };
			CHAR message[2048] = { 0 };

			va_list ap;
			va_start(ap, Format);
			vsprintf_s(buffer, _TRUNCATE, Format, ap);
			sprintf_s(message, "%s(%d):\n\n%s", File, Line, buffer);
			va_end(ap);

			MessageBoxA(nullptr, message, "ASSERTION", MB_ICONERROR);

			if (IsDebuggerPresent()) {
				OutputDebugStringA(message);
				__debugbreak();
			}

			TerminateProcess(GetCurrentProcess(), 1);
			__assume(0);
		}

		void __Assert(LPCWSTR File, int Line, LPCWSTR Format, ...)
		{
			WCHAR buffer[2048] = { 0 };
			WCHAR message[2048] = { 0 };

			va_list ap;
			va_start(ap, Format);
			vswprintf_s(buffer, _TRUNCATE, Format, ap);
			swprintf_s(message, L"%s(%d):\n\n%s", File, Line, buffer);
			va_end(ap);

			MessageBoxW(nullptr, message, L"ASSERTION", MB_ICONERROR);

			if (IsDebuggerPresent()) {
				OutputDebugStringW(message);
				__debugbreak();
			}

			TerminateProcess(GetCurrentProcess(), 1);
			__assume(0);
		}

		uintptr_t FindPattern(uintptr_t StartAddress, uintptr_t MaxSize, const char* Mask)
		{
			std::vector<std::pair<uint8_t, bool>> pattern;

			for (size_t i = 0; i < strlen(Mask);) {
				if (Mask[i] != '?') {
					pattern.emplace_back((uint8_t)strtoul(&Mask[i], nullptr, 16), false);
					i += 3;
				}
				else {
					pattern.emplace_back(0x00, true);
					i += 2;
				}
			}

			const uint8_t* dataStart = (uint8_t*)StartAddress;
			const uint8_t* dataEnd = (uint8_t*)StartAddress + MaxSize + 1;

			auto ret = std::search(dataStart, dataEnd, pattern.begin(), pattern.end(),
				[](uint8_t CurrentByte, std::pair<uint8_t, bool>& Pattern) {
					return Pattern.second || (CurrentByte == Pattern.first);
				});

			if (ret == dataEnd)
				return 0;

			return std::distance(dataStart, ret) + StartAddress;
		}

		Array<uintptr_t> FindPatterns(uintptr_t StartAddress, uintptr_t MaxSize, const char* Mask)
		{
			Array<uintptr_t> results;
			Array<std::pair<uint8_t, bool>> pattern;

			for (size_t i = 0; i < strlen(Mask);) {
				if (Mask[i] != '?') {
					pattern.emplace_back((uint8_t)strtoul(&Mask[i], nullptr, 16), false);
					i += 3;
				}
				else {
					pattern.emplace_back(0x00, true);
					i += 2;
				}
			}

			const uint8_t* dataStart = (uint8_t*)StartAddress;
			const uint8_t* dataEnd = (uint8_t*)StartAddress + MaxSize + 1;

			for (const uint8_t* i = dataStart;;) {
				auto ret = std::search(i, dataEnd, pattern.begin(), pattern.end(),
					[](uint8_t CurrentByte, std::pair<uint8_t, bool>& Pattern) {
						return Pattern.second || (CurrentByte == Pattern.first);
					});

				// Не соответствует шаблону байтов, уходим
				if (ret == dataEnd)
					break;

				uintptr_t addr = std::distance(dataStart, ret) + StartAddress;
				results.push_back(addr);

				i = (uint8_t*)(addr + 1);
			}

			return results;
		}

		bool GetPESectionRange(uintptr_t ModuleBase, const char* Section, uintptr_t* Start, uintptr_t* End)
		{
			PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(ModuleBase + 
				((PIMAGE_DOS_HEADER)ModuleBase)->e_lfanew);
			PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);

			// Предположим, что заголовок PE отсутствует, если раздел отсутствует
			if (!Section || strlen(Section) <= 0) {
				if (Start)
					*Start = ModuleBase;

				if (End)
					*End = ModuleBase + ntHeaders->OptionalHeader.SizeOfHeaders;

				return true;
			}

			for (uint32_t i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, section++) {
				// Имя может не заканчиваться нулем
				char sectionName[sizeof(IMAGE_SECTION_HEADER::Name) + 1] = { };
				memcpy(sectionName, section->Name, sizeof(IMAGE_SECTION_HEADER::Name));

				if (!strcmp(sectionName, Section)) {
					if (Start)
						*Start = ModuleBase + section->VirtualAddress;

					if (End)
						*End = ModuleBase + section->VirtualAddress + section->Misc.VirtualSize;

					return true;
				}
			}

			return false;
		}

		void PatchMemory(uintptr_t Address, uint8_t* Data, size_t Size) {
			DWORD d = 0;
			VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

			for (uintptr_t i = Address; i < (Address + Size); i++)
				*(volatile uint8_t*)i = *Data++;

			VirtualProtect((LPVOID)Address, Size, d, &d);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
		}

		void PatchMemory(uintptr_t Address, std::initializer_list<uint8_t> Data) {
			DWORD d = 0;
			VirtualProtect((LPVOID)Address, Data.size(), PAGE_EXECUTE_READWRITE, &d);

			uintptr_t i = Address;
			for (auto value : Data)
				*(volatile uint8_t*)i++ = value;

			VirtualProtect((LPVOID)Address, Data.size(), d, &d);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Data.size());
		}

		void PatchMemoryNop(uintptr_t Address, size_t Size) {
			DWORD d = 0;
			VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

			for (uintptr_t i = Address; i < (Address + Size); i++)
				*(volatile uint8_t*)i = 0x90;

			VirtualProtect((LPVOID)Address, Size, d, &d);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
		}

		void PatchMemoryWP(uintptr_t Address, uint8_t* Data, size_t Size)
		{
			for (uintptr_t i = Address; i < (Address + Size); i++)
				*(volatile uint8_t*)i = *Data++;
		}

		void PatchMemoryWP(uintptr_t Address, std::initializer_list<uint8_t> Data)
		{
			uintptr_t i = Address;
			for (auto value : Data)
				*(volatile uint8_t*)i++ = value;
		}

		void PatchMemoryNopWP(uintptr_t Address, size_t Size)
		{
			for (uintptr_t i = Address; i < (Address + Size); i++)
				*(volatile uint8_t*)i = 0x90;
		}

		DWORD UnlockWP(uintptr_t Address, size_t Size)
		{
			DWORD OldFlag = 0;
			if (VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &OldFlag))
				return OldFlag;
			return 0;
		}

		void LockWP(uintptr_t Address, size_t Size, DWORD OldFlags)
		{
			DWORD OldFlag = 0;
			VirtualProtect((LPVOID)Address, Size, OldFlags, &OldFlag);
			FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
		}

		void SetThreadName(uint32_t ThreadID, LPCSTR ThreadName)
		{
#pragma pack(push, 8)  
			const DWORD MS_VC_EXCEPTION = 0x406D1388;

			typedef struct tagTHREADNAME_INFO {
				DWORD dwType;		// Must be 0x1000.  
				LPCSTR szName;		// Pointer to name (in user addr space).  
				DWORD dwThreadID;	// Thread ID (-1=caller thread).  
				DWORD dwFlags;		// Reserved for future use, must be zero.  
			} THREADNAME_INFO;
#pragma pack(pop)

			THREADNAME_INFO info = { 0 };
			info.dwType = 0x1000;
			info.szName = ThreadName;
			info.dwThreadID = ThreadID;
			info.dwFlags = 0;

			__try {
				RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}

		uint64_t GetTotalPhysicalMemory()
		{
			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (!GlobalMemoryStatusEx(&statex)) return 0;
			return (uint64_t)statex.ullTotalPhys;
		}

		uint64_t GetTotalPageFileMemory()
		{
			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (!GlobalMemoryStatusEx(&statex)) return 0;
			return (uint64_t)statex.ullTotalPageFile;
		}

		uint64_t GetAvailableTotalPhysicalMemory()
		{
			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (!GlobalMemoryStatusEx(&statex)) return 0;
			return (uint64_t)statex.ullAvailPhys;
		}

		uint64_t GetAvailableTotalPageFileMemory()
		{
			MEMORYSTATUSEX statex = { 0 };
			statex.dwLength = sizeof(MEMORYSTATUSEX);
			if (!GlobalMemoryStatusEx(&statex)) return 0;
			return (uint64_t)statex.ullAvailPageFile;
		}

		char* StrDub(const char* s)
		{
			auto l = strlen(s);
			return (char*)memcpy((char*)Core::GlobalMemoryManagerPtr->MemAlloc(l + 1), s, l + 1);
		}

		void Quit()
		{
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
}