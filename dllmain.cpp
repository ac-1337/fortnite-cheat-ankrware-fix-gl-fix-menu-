#include "frame.h"

float* (*CalculateShot)(PVOID, PVOID, PVOID) = nullptr;
float* CalculateShotHook(PVOID arg0, PVOID arg1, PVOID arg2) {
	auto ret = CalculateShot(arg0, arg1, arg2);

	if (settings::silent) {

		if (IF_AIMKEY()) {

			if (ret && settings::silent && Offsets::TargetPawn && LocalPawn) {

				SDK::Structs::Vector3 headpos_temp;
				SDK::Classes::USkeletalMeshComponent::GetBoneLocation(Offsets::TargetPawn, 66, &headpos_temp);

				SDK::Structs::FVector head = { headpos_temp.x, headpos_temp.y, headpos_temp.z };

				auto rootPtr = GetPawnRootLocations(LocalPawn);

				if (!rootPtr) return ret;
				auto root = *rootPtr;

				auto dx = head.X - root.X;
				auto dy = head.Y - root.Y;
				auto dz = head.Z - root.Z;

				if (dx * dx + dy * dy + dz * dz < 125000.0f) {
					ret[4] = head.X;
					ret[5] = head.Y;
					ret[6] = head.Z;
				}
				else {
					head.Z -= 16.0f;
					root.Z += 45.0f;
					auto y = atan2f(head.Y - root.Y, head.X - root.X);
					root.X += cosf(y + 1.5708f) * 32.0f;
					root.Y += sinf(y + 1.5708f) * 32.0f;
					auto length = SDK::Utilities::SpoofCall(sqrtf, powf(head.X - root.X, 2) + powf(head.Y - root.Y, 2));
					auto x = -atan2f(head.Z - root.Z, length);

					y = atan2f(head.Y - root.Y, head.X - root.X);

					x /= 2.0f;
					y /= 2.0f;

					ret[0] = -(sinf(x) * sinf(y));
					ret[1] = sinf(x) * cosf(y);
					ret[2] = cosf(x) * sinf(y);
					ret[3] = cosf(x) * cosf(y);
				}
			}
		}
	}
	return ret;
}

void minhookshit() {
	uintptr_t address1 = Scanners::PatternScan(("48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 48 8B F1")); //ida gay : 48 89 5C 24 ? 4C 89 4C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B F9 4C 8D 6C 24 ?
}
void steamhook()
{
	HWND fn_window = FindWindow(0, (L"Fortnite  "));

	// Initalize Steam hook

	auto address = Scanners::PatternScan(SteamHelper::GetSteamdModule(), xorthis("48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8"));
	SteamHelper::InsertHook(address, (uintptr_t)present_hk, (long long*)&presenth);

	address = Scanners::PatternScan(SteamHelper::GetSteamdModule(), xorthis("48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8"));
	SteamHelper::InsertHook(address, (uintptr_t)present_hk, (long long*)&resizeh);

	WindowProcess = (WNDPROC)SetWindowLongPtr(fn_window, GWLP_WNDPROC, (LONG_PTR)WndProc);
}
void sigs()
{
	Details.UWORLD = SDK::Utilities::Scanners::PatternScan(xorthis("48 8B 05 ? ? ? ? 4D 8B C2"));
	Details.UWORLD = RVA(Details.UWORLD, 7);

	if (!SDK::Utilities::valid_pointer(Details.UWORLD)) {
		SDK::Utilities::SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)xorthis("UWORLD OUTDATED"), (LPCSTR)0, (UINT)0);
	}

	LineOfS = SDK::Utilities::Scanners::PatternScan(xorthis("E8 ? ? ? ? 48 8B 0D ? ? ? ? 33 D2 40 8A F8"));
	LineOfS = RVA(LineOfS, 5);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		steamhook();
		sigs();
	}

	return TRUE;
}