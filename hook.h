#include "scanner.h"

namespace SteamHelper
{
	uintptr_t GetSteamdModule()
	{
		printf("1\n");
		std::string module = ("GameOverlayRenderer64.dll");
		return (uintptr_t)GetModuleHandleA(module.c_str());
	}

	void InsertHook(__int64 addr, __int64 func, __int64* orig)
	{
		printf("2\n");
		static uintptr_t hook_addr;
		if (!hook_addr)
			hook_addr = Scanners::PatternScan(GetSteamdModule(), ("48 ? ? ? ? 57 48 83 EC 30 33 C0"));

		auto hook = ((__int64(__fastcall*)(__int64 addr, __int64 func, __int64* orig, __int64 smthng))(hook_addr));
		hook((__int64)addr, (__int64)func, orig, (__int64)1);
	}

	short GetAsyncKeyState(const int vKey)
	{
		printf("3\n");
		static uintptr_t addrGetAsyncKeyState = NULL;

		if (!addrGetAsyncKeyState)
		{
			addrGetAsyncKeyState = Scanners::PatternScan(GetSteamdModule(),
				("40 53 48 83 EC 20 80 3D ? ? ? ? ? 8B D9"));
		}

		if (!addrGetAsyncKeyState)
			return false;

		using GetAsyncKeyState_t = short(__fastcall*)(int);
		auto fnGetAyncKeyState = (GetAsyncKeyState_t)addrGetAsyncKeyState;

		return fnGetAyncKeyState(vKey);
	}

	short SetCursorPos(int x, int y)
	{
		printf("4\n");
		static uintptr_t addrSetCursorPos = NULL;

		if (!addrSetCursorPos)
		{
			addrSetCursorPos = Scanners::PatternScan(GetSteamdModule(),
				(""));
		}

		if (!addrSetCursorPos)
			return false;

		using SetCursorPos_t = short(__fastcall*)(int, int);
		auto fnSetCursorPos = (SetCursorPos_t)addrSetCursorPos;

		return fnSetCursorPos(x, y);
	}


	bool GetCursorPos(LPPOINT lpPoint)
	{
		printf("5\n");
		static uintptr_t addrGetCursorPos = NULL;

		if (!addrGetCursorPos)
		{
			addrGetCursorPos = Scanners::PatternScan(GetSteamdModule(),
				(""));
		}

		if (!addrGetCursorPos)
			return false;

		using GetCursorPos_t = short(__fastcall*)(LPPOINT);
		auto fnGetCursorPos = (GetCursorPos_t)addrGetCursorPos;

		return fnGetCursorPos(lpPoint);
	}

	HCURSOR SetCursor(HCURSOR hCursor)
	{
		printf("6\n");
		static uintptr_t addrSetCursor = NULL;

		if (!addrSetCursor)
		{
			addrSetCursor = Scanners::PatternScan(GetSteamdModule(),
				(""));
		}

		if (!addrSetCursor)
			return (HCURSOR)false;

		using SetCursor_t = HCURSOR(__fastcall*)(HCURSOR);
		auto fnSetCursor = (SetCursor_t)addrSetCursor;

		return fnSetCursor(hCursor);
	}
}