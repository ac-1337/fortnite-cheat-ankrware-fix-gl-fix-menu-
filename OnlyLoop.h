#include "sdk.h"
#include "XOR.hpp"
//PVOID TargetPawn = nullptr;

SDK::Structs::Vector3 CamLoc;
SDK::Structs::Vector3 CamRot;
uint64_t entityx;
#define ReadPointer(base, offset) (*(PVOID *)(((PBYTE)base + offset)))
#define ReadDWORD(base, offset) (*(PDWORD)(((PBYTE)base + offset)))
#define ReadBYTE(base, offset) (*(((PBYTE)base + offset)))
#define ReadDWORD_PTR(base, offset) (*(DWORD_PTR *)(((PBYTE)base + offset)))

SDK::Structs::FVector* GetPawnRootLocations(uintptr_t pawn) {
	auto root = SDK::Utilities::read<uintptr_t>(pawn + SDK::Classes::StaticOffsets::RootComponent);
	if (!root) {
		return nullptr;
	}
	return reinterpret_cast<SDK::Structs::FVector*>(reinterpret_cast<PBYTE>(root) + SDK::Classes::StaticOffsets::RelativeLocation);
}
void DrawAimingEnemy()
{
	if (!entityx) return;

	uint64_t currentactormesh = SDK::Utilities::read<uint64_t>(entityx + Offsets::Mesh);
	if (!currentactormesh) return;
	SDK::Structs::Vector3 target_loc = SDK::Classes::USkeletalMeshComponent::GetBoneLocation(currentactormesh, 66, &target_loc);
	SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(target_loc.x, target_loc.y, target_loc.z + 20), &target_loc);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(target_loc.x, target_loc.y), ImVec2(X / 2, X / 2), ImGui::GetColorU32({ 200 / 255.f, 0 / 255.f, 0 / 255.f, 1.0f }), 1);
}
SDK::Structs::Vector3 GetBonePos(DWORD_PTR mesh, int id)
{
	auto bonearray = ReadPointer(mesh, 0x4A8);
	if (bonearray == NULL)
	{
		bonearray = ReadPointer(mesh, 0x4A8 + 0x10);
	}
	FTransform bone = ReadFTRANSFORM(bonearray, (id * 0x30));
	FTransform ComponentToWorld = ReadFTRANSFORM(mesh, 0x1C0);
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return SDK::Structs::Vector3(Matrix._41, Matrix._42, Matrix._43);
}
namespace overlay
{
	std::string string_To_UTF8(const std::string& str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2);
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);
		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
		std::string retStr(pBuf);
		delete[]pwBuf;
		delete[]pBuf;
		pwBuf = NULL;
		pBuf = NULL;
		return retStr;
	}
	// Drawings for custom menus + Toggle Button's / Checkboxes ETC
	void RegularRGBText(int x, int y, ImColor color, const char* str)
	{
		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = string_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImColor(color), utf_8_2.c_str());
	}
	void Test(int x, int y, ImColor color, const char* str)
	{
		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = string_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());
	}
	void ShadowRGBText(int x, int y, ImColor color, const char* str)
	{
		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = string_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 240)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 240)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());
	}
	void OutlinedRBGText(int x, int y, ImColor color, const char* str)
	{
		ImFont a;
		std::string utf_8_1 = std::string(str);
		std::string utf_8_2 = string_To_UTF8(utf_8_1);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y + 2), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 30 / 30.0)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y + 2), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 30 / 30.0)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());
	}
	void Rect(int x, int y, int w, int h, ImColor color, int thickness)
	{
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0, thickness);
	}
	void FilledRect(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
	}

	void OutlinedString(ImGuiWindow& windowshit, std::string str, ImVec2 loc, ImU32 colr, bool centered = false)
	{
		ImVec2 size = { 0,0 };
		float minx = 0;
		float miny = 0;
		if (centered)
		{
			size = ImGui::GetFont()->CalcTextSizeA(windowshit.DrawList->_Data->FontSize, 0x7FFFF, 0, str.c_str());
			minx = size.x / 2.f;
			miny = size.y / 2.f;
		}

		windowshit.DrawList->AddText(ImVec2((loc.x - 1) - minx, (loc.y - 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
		windowshit.DrawList->AddText(ImVec2((loc.x + 1) - minx, (loc.y + 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
		windowshit.DrawList->AddText(ImVec2((loc.x + 1) - minx, (loc.y - 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
		windowshit.DrawList->AddText(ImVec2((loc.x - 1) - minx, (loc.y + 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
		windowshit.DrawList->AddText(ImVec2(loc.x - minx, loc.y - miny), colr, str.c_str());
	}
	float DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

			if (center)
			{
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

				window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
			}
			else
			{
				window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

				window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
			}

			y = pos.y + textSize.y * (i + 1);
			i++;
		}
		return y;
	}
	float DrawNormalText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int i = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

			if (center)
			{
				window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
			}
			else
			{
				window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
			}

			y = pos.y + textSize.y * (i + 1);
			i++;
		}
		return y;
	}
	void DrawLine(int x1, int y1, int x2, int y2, const ImVec4& color, int thickness)
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::GetColorU32(color), thickness);
	}
	void DrawCorneredBox(int X, int Y, int W, int H, ImColor color, int thickness) {
		float lineW = (W / 3);
		float lineH = (H / 3);

		DrawLine(X, Y, X, Y + lineH, color, thickness);
		DrawLine(X, Y, X + lineW, Y, color, thickness);
		DrawLine(X + W - lineW, Y, X + W, Y, color, thickness);
		DrawLine(X + W, Y, X + W, Y + lineH, color, thickness);
		DrawLine(X, Y + H - lineH, X, Y + H, color, thickness);
		DrawLine(X, Y + H, X + lineW, Y + H, color, thickness);
		DrawLine(X + W - lineW, Y + H, X + W, Y + H, color, thickness);
		DrawLine(X + W, Y + H - lineH, X + W, Y + H, color, thickness);
	}
	void Outline(int X, int Y, int W, int H, ImColor color) {
		float lineW = (W / 3);
		float lineH = (H / 3);

		DrawLine(X, Y, X, Y + lineH, color, 2);
		DrawLine(X, Y, X + lineW, Y, color, 2);
		DrawLine(X + W - lineW, Y, X + W, Y, color, 2);
		DrawLine(X + W, Y, X + W, Y + lineH, color, 2);
		DrawLine(X, Y + H - lineH, X, Y + H, color, 2);
		DrawLine(X, Y + H, X + lineW, Y + H, color, 2);
		DrawLine(X + W - lineW, Y + H, X + W, Y + H, color, 2);
		DrawLine(X + W, Y + H - lineH, X + W, Y + H, color, 2);
	}
}


namespace SetLoop
{
	bool PlayersLoop(ImGuiWindow& presentwindow)
	{
		try
		{
			float FOVmax = 9999.f;
			uintptr_t MyTeamIndex = 0, EnemyTeamIndex = 0;
			uintptr_t GWorld = SDK::Utilities::read<uintptr_t>(Details.UWORLD);
			if (!GWorld) return false;

			X = SDK::Utilities::SpoofCall(GetSystemMetrics, SM_CXSCREEN);
			Y = SDK::Utilities::SpoofCall(GetSystemMetrics, SM_CYSCREEN);

			uintptr_t Gameinstance = SDK::Utilities::read<uint64_t>(GWorld + Offsets::OwningGameInstance);
			if (!Gameinstance) return false;

			uintptr_t LocalPlayers = SDK::Utilities::read<uint64_t>(Gameinstance + Offsets::LocalPlayers);
			if (!LocalPlayers) return false;

			uintptr_t LocalPlayer = SDK::Utilities::read<uint64_t>(LocalPlayers);
			if (!LocalPlayer) return false;

			PlayerController = SDK::Utilities::read<uint64_t>(LocalPlayer + Offsets::PlayerController);
			if (!PlayerController) return false;

			uintptr_t PlayerCameraManager = SDK::Utilities::read<uint64_t>(PlayerController + Offsets::PlayerCameraManager);
			if (!PlayerCameraManager) return false;

			FOVAngle = SDK::Classes::APlayerCameraManager::GetFOVAngle(PlayerCameraManager);
			LocalPawn = SDK::Utilities::read<uint64_t>(PlayerController + Offsets::AcknowledgedPawn);

			uintptr_t levels = SDK::Utilities::read<uintptr_t>(GWorld + Offsets::PersistentLevel);
			if (!levels) return false;

			uintptr_t Actors = SDK::Utilities::read<uintptr_t>(levels + Offsets::AActors);
			if (!Actors) return false;

			int ActorC = *(int*)(levels + Offsets::ActorCount);
			if (!ActorC) return false;

			GetPlayerViewPoint(PlayerCameraManager, &SDK::Utilities::CamLoc, &SDK::Utilities::CamRot);

			uintptr_t LocalRootComponent = SDK::Utilities::read<uintptr_t>(LocalPawn + 0x130);
			SDK::Structs::Vector3 LocalRelativeLocation = SDK::Utilities::read<SDK::Structs::Vector3>(LocalRootComponent + 0x11C);

			uint64_t CurrentWeapon = SDK::Utilities::read<uint64_t>(LocalPawn + 0x600);

			for (int i = 0; i < ActorC; i++) {

				uintptr_t pawn = SDK::Utilities::read<uint64_t>(Actors + i * sizeof(uintptr_t));
				auto name = GetObjectName(pawn);

				if (strstr(name.c_str(), ("BP_PlayerPawn")) || strstr(name.c_str(), ("PlayerPawn")))
				{
					SDK::Structs::Vector3 Headbox, bottom, pelviss, localpelviss, WeaponPos;

					SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 66, &Headbox);
					SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 2, &pelviss);
					SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 0, &bottom);

					SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(Headbox.x, Headbox.y, Headbox.z + 20), &Headbox);
					SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(pelviss.x, pelviss.y, pelviss.z + 70), &pelviss);
					SDK::Classes::AController::WorldToScreen(bottom, &bottom);

					DWORD_PTR player_mesh = ReadDWORD_PTR(pawn, 0x280);
					SDK::Structs::Vector3 Head = GetBonePos(player_mesh, 66);
					SDK::Structs::Vector3 HeadW2S = SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(Head.x, Head.y, Head.z + 15), &Head);

					SDK::Structs::Vector3 head2, neck, pelvis, chest, leftShoulder, rightShoulder, leftElbow, rightElbow, leftHand, rightHand, leftLeg, rightLeg, leftThigh, rightThigh, leftFoot, rightFoot, leftFeet, rightFeet, leftFeetFinger, rightFeetFinger;

					ImColor BoxColor = { 255, 255, 255 };
					ImColor LineColor = { 255, 255, 255, 100 };

					SDK::Structs::Vector3 View;
					bool IsVisible;
					ImColor col = { 255, 255, 255, 220 };
					ImColor SkelColor = { 255, 255, 255, 220 };
					if (settings::vischeck)
					{
						IsVisible = SDK::Classes::APlayerCameraManager::LineOfSightTo((PVOID)PlayerController, (PVOID)pawn, &View);
						if (IsVisible) {
							col = { 0, 235, 235 };
						}
						else {
							col = { 255, 255, 255 };
						}
						if (IsVisible)
						{
							SkelColor = { 0, 235, 235 };
						}
						else
						{
							SkelColor = { 255, 255, 255 };
						}
					}

					if (settings::Lines && SDK::Classes::Utils::CheckInScreen(pawn, X, Y))
					{
						overlay::DrawLine(X / 2, Y, bottom.x, bottom.y, ImColor(col), 1);
					}

					if (pawn == LocalPawn) continue;

					if (Headbox.x == 0 && Headbox.y == 0) continue;
					if (bottom.x == 0 && bottom.y == 0) continue;

					float Height = Headbox.y - bottom.y;
					if (Height < 0)
						Height = Height * (-1.f);
					float Width = Height * 0.25;
					Headbox.x = Headbox.x - (Width / 2);

					if (settings::esp && SDK::Classes::Utils::CheckInScreen(pawn, X, Y))
					{
						overlay::Rect(Headbox.x, Headbox.y, Width, Height, ImColor(col), 1);
					}

					if (settings::corner && SDK::Classes::Utils::CheckInScreen(pawn, X, Y))
					{
						SDK::Structs::Vector3 head2, neck, pelvis, chest, leftShoulder, rightShoulder, leftElbow, rightElbow, leftHand, rightHand, leftLeg, rightLeg, leftThigh, rightThigh, leftFoot, rightFoot, leftFeet, rightFeet, leftFeetFinger, rightFeetFinger;

						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 66, &head2);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 65, &neck);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 2, &pelvis);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 36, &chest);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 9, &leftShoulder);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 37, &rightShoulder);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 10, &leftElbow);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 38, &rightElbow);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 11, &leftHand);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 39, &rightHand);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 67, &leftLeg);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 74, &rightLeg);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 73, &leftThigh);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 80, &rightThigh);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 68, &leftFoot);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 75, &rightFoot);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 71, &leftFeet);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 78, &rightFeet);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 72, &leftFeetFinger);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 79, &rightFeetFinger);
						SDK::Classes::AController::WorldToScreen(head2, &head2);
						SDK::Classes::AController::WorldToScreen(neck, &neck);
						SDK::Classes::AController::WorldToScreen(pelvis, &pelvis);
						SDK::Classes::AController::WorldToScreen(chest, &chest);
						SDK::Classes::AController::WorldToScreen(leftShoulder, &leftShoulder);
						SDK::Classes::AController::WorldToScreen(rightShoulder, &rightShoulder);
						SDK::Classes::AController::WorldToScreen(leftElbow, &leftElbow);
						SDK::Classes::AController::WorldToScreen(rightElbow, &rightElbow);
						SDK::Classes::AController::WorldToScreen(leftHand, &leftHand);
						SDK::Classes::AController::WorldToScreen(rightHand, &rightHand);
						SDK::Classes::AController::WorldToScreen(leftLeg, &leftLeg);
						SDK::Classes::AController::WorldToScreen(rightLeg, &rightLeg);
						SDK::Classes::AController::WorldToScreen(leftThigh, &leftThigh);
						SDK::Classes::AController::WorldToScreen(rightThigh, &rightThigh);
						SDK::Classes::AController::WorldToScreen(leftFoot, &leftFoot);
						SDK::Classes::AController::WorldToScreen(rightFoot, &rightFoot);
						SDK::Classes::AController::WorldToScreen(leftFeet, &leftFeet);
						SDK::Classes::AController::WorldToScreen(rightFeet, &rightFeet);
						SDK::Classes::AController::WorldToScreen(leftFeetFinger, &leftFeetFinger);
						SDK::Classes::AController::WorldToScreen(rightFeetFinger, &rightFeetFinger);

						int array[20] = { head2.x, neck.x, pelvis.x, chest.x, leftShoulder.x, rightShoulder.x, leftElbow.x, rightElbow.x, leftHand.x, rightHand.x, leftLeg.x, rightLeg.x, leftThigh.x, rightThigh.x, leftFoot.x, rightFoot.x, leftFeet.x, rightFeet.x, leftFeetFinger.x, rightFeetFinger.x };
						int mostright = array[0];
						int mostleft = array[0];

						for (int mostrighti = 0; mostrighti < 20; mostrighti++)
						{
							if (array[mostrighti] > mostright)
								mostright = array[mostrighti];
						}

						for (int mostlefti = 0; mostlefti < 20; mostlefti++)
						{
							if (array[mostlefti] < mostleft)
								mostleft = array[mostlefti];
						}
						overlay::DrawCorneredBox(pelviss.x - ((mostright - mostleft) / 2), pelviss.y, (mostright - mostleft), Height, ImColor(col), 1.5);
					}
					if (settings::name && SDK::Classes::Utils::CheckInScreen(LocalPawn, X, Y))
					{
						SDK::Structs::Vector3 headA;
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(LocalPawn, 66, &headA);
						SDK::Classes::AController::WorldToScreen(headA, &headA);
						SDK::Structs::FString gayName = SDK::Utilities::read<SDK::Structs::FString>(EnemyTeamIndex + 0x300);

						if (gayName.c_str())
						{
							CHAR Eichhörnchen[0xFF] = { 0 };
							wcstombs(Eichhörnchen, gayName.c_str(), sizeof(gayName));
							overlay::OutlinedRBGText(headA.x, headA.y + 25, ImColor(161, 5, 5),(xorthis("[ %s ]"), Eichhörnchen));
						}
					}
					if (settings::Distance)
					{
						if (SDK::Utilities::valid_pointer(LocalPawn))
						{
							float dist;
							Headbox + 15;
							uintptr_t localPlayerRoot = SDK::Utilities::read<uintptr_t>(LocalPawn + SDK::Classes::StaticOffsets::RootComponent);
							auto localPlayerLocation = reinterpret_cast<float*>(reinterpret_cast<PBYTE>(localPlayerRoot) + SDK::Classes::StaticOffsets::RelativeLocation);
							auto playerRoot = GetPawnRootLocations(pawn);
							if (playerRoot) {
								auto playerPos = *playerRoot;
								float x = localPlayerLocation[0] - playerPos.X;
								float y = localPlayerLocation[1] - playerPos.Y;
								float z = localPlayerLocation[2] - playerPos.Z;

								char draw[64];
								dist = sqrtf((x * x) + (y * y) + (z * z)) / 100.0f;
								sprintf_s(draw, "%.0f m", dist);

								overlay::ShadowRGBText(Headbox.x - 5, Headbox.y - 35, ImColor(0, 255, 255, 200), draw);
							}
						}
					}

					if (settings::bones && SDK::Classes::Utils::CheckInScreen(pawn, X, Y))
					{
						SDK::Structs::Vector3 head2, neck, pelvis, chest, leftShoulder, rightShoulder, leftElbow, rightElbow, leftHand, rightHand, leftLeg, rightLeg, leftThigh, rightThigh, leftFoot, rightFoot, leftFeet, rightFeet, leftFeetFinger, rightFeetFinger;

						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 66, &head2);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 65, &neck);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 2, &pelvis);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 36, &chest);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 9, &leftShoulder);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 62, &rightShoulder);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 10, &leftElbow);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 38, &rightElbow);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 11, &leftHand);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 39, &rightHand);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 67, &leftLeg);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 74, &rightLeg);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 73, &leftThigh);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 80, &rightThigh);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 68, &leftFoot);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 75, &rightFoot);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 71, &leftFeet);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 78, &rightFeet);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 72, &leftFeetFinger);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 79, &rightFeetFinger);

						SDK::Classes::AController::WorldToScreen(head2, &head2);
						SDK::Classes::AController::WorldToScreen(neck, &neck);
						SDK::Classes::AController::WorldToScreen(pelvis, &pelvis);
						SDK::Classes::AController::WorldToScreen(chest, &chest);
						SDK::Classes::AController::WorldToScreen(leftShoulder, &leftShoulder);
						SDK::Classes::AController::WorldToScreen(rightShoulder, &rightShoulder);
						SDK::Classes::AController::WorldToScreen(leftElbow, &leftElbow);
						SDK::Classes::AController::WorldToScreen(rightElbow, &rightElbow);
						SDK::Classes::AController::WorldToScreen(leftHand, &leftHand);
						SDK::Classes::AController::WorldToScreen(rightHand, &rightHand);
						SDK::Classes::AController::WorldToScreen(leftLeg, &leftLeg);
						SDK::Classes::AController::WorldToScreen(rightLeg, &rightLeg);
						SDK::Classes::AController::WorldToScreen(leftThigh, &leftThigh);
						SDK::Classes::AController::WorldToScreen(rightThigh, &rightThigh);
						SDK::Classes::AController::WorldToScreen(leftFoot, &leftFoot);
						SDK::Classes::AController::WorldToScreen(rightFoot, &rightFoot);
						SDK::Classes::AController::WorldToScreen(leftFeet, &leftFeet);
						SDK::Classes::AController::WorldToScreen(rightFeet, &rightFeet);
						SDK::Classes::AController::WorldToScreen(leftFeetFinger, &leftFeetFinger);
						SDK::Classes::AController::WorldToScreen(rightFeetFinger, &rightFeetFinger);

						overlay::DrawLine(head2.x, head2.y, neck.x, neck.y, SkelColor, 1);
						overlay::DrawLine(neck.x, neck.y, pelvis.x, pelvis.y, SkelColor, 1);
						overlay::DrawLine(chest.x, chest.y, leftShoulder.x, leftShoulder.y, SkelColor, 1);
						overlay::DrawLine(chest.x, chest.y, rightShoulder.x, rightShoulder.y, SkelColor, 1);
						overlay::DrawLine(leftShoulder.x, leftShoulder.y, leftElbow.x, leftElbow.y, SkelColor, 1);
						overlay::DrawLine(rightShoulder.x, rightShoulder.y, rightElbow.x, rightElbow.y, SkelColor, 1);
						overlay::DrawLine(leftElbow.x, leftElbow.y, leftHand.x, leftHand.y, SkelColor, 1);
						overlay::DrawLine(rightElbow.x, rightElbow.y, rightHand.x, rightHand.y, SkelColor, 1);
						overlay::DrawLine(pelvis.x, pelvis.y, leftLeg.x, leftLeg.y, SkelColor, 1);
						overlay::DrawLine(pelvis.x, pelvis.y, rightLeg.x, rightLeg.y, SkelColor, 1);
						overlay::DrawLine(leftLeg.x, leftLeg.y, leftThigh.x, leftThigh.y, SkelColor, 1);
						overlay::DrawLine(rightLeg.x, rightLeg.y, rightThigh.x, rightThigh.y, SkelColor, 1);
						overlay::DrawLine(leftThigh.x, leftThigh.y, leftFoot.x, leftFoot.y, SkelColor, 1);
						overlay::DrawLine(rightThigh.x, rightThigh.y, rightFoot.x, rightFoot.y, SkelColor, 1);
						overlay::DrawLine(leftFoot.x, leftFoot.y, leftFeet.x, leftFeet.y, SkelColor, 1);
						overlay::DrawLine(rightFoot.x, rightFoot.y, rightFeet.x, rightFeet.y, SkelColor, 1);
						overlay::DrawLine(leftFeet.x, leftFeet.y, leftFeetFinger.x, leftFeetFinger.y, SkelColor, 1);
						overlay::DrawLine(rightFeet.x, rightFeet.y, rightFeetFinger.x, rightFeetFinger.y, SkelColor, 1);
					}
					if (settings::Box3D and SDK::Classes::Utils::CheckInScreen(pawn, X, Y)) {

						SDK::Structs::Vector3 BottomNoW2S;
						SDK::Structs::Vector3 HeadNoW2S;

						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 66, &HeadNoW2S);
						SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 0, &BottomNoW2S);


						SDK::Structs::Vector3 bottom1;
						SDK::Structs::Vector3 bottom2;
						SDK::Structs::Vector3 bottom3;
						SDK::Structs::Vector3 bottom4;

						SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(BottomNoW2S.x + 30, BottomNoW2S.y - 30, BottomNoW2S.z), &bottom1);
						SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(BottomNoW2S.x - 30, BottomNoW2S.y - 30, BottomNoW2S.z), &bottom2);
						SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(BottomNoW2S.x - 30, BottomNoW2S.y + 30, BottomNoW2S.z), &bottom3);
						SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(BottomNoW2S.x + 30, BottomNoW2S.y + 30, BottomNoW2S.z), &bottom4);



						SDK::Structs::Vector3 top1;
						SDK::Structs::Vector3 top2;
						SDK::Structs::Vector3 top3;
						SDK::Structs::Vector3 top4;

						SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(HeadNoW2S.x + 30, HeadNoW2S.y - 30, HeadNoW2S.z), &top1);
						SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(HeadNoW2S.x - 30, HeadNoW2S.y - 30, HeadNoW2S.z), &top2);
						SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(HeadNoW2S.x - 30, HeadNoW2S.y + 30, HeadNoW2S.z), &top3);
						SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(HeadNoW2S.x + 30, HeadNoW2S.y + 30, HeadNoW2S.z), &top4);




						overlay::DrawLine(bottom1.x, bottom1.y, top1.x, top1.y, ImColor(col), 1);
						overlay::DrawLine(bottom2.x, bottom2.y, top2.x, top2.y, ImColor(col), 1);
						overlay::DrawLine(bottom3.x, bottom3.y, top3.x, top3.y, ImColor(col), 1);
						overlay::DrawLine(bottom4.x, bottom4.y, top4.x, top4.y, ImColor(col), 1);


						overlay::DrawLine(bottom1.x, bottom1.y, bottom2.x, bottom2.y, ImColor(col), 1);
						overlay::DrawLine(bottom2.x, bottom2.y, bottom3.x, bottom3.y, ImColor(col), 1);
						overlay::DrawLine(bottom3.x, bottom3.y, bottom4.x, bottom4.y, ImColor(col), 1);
						overlay::DrawLine(bottom4.x, bottom4.y, bottom1.x, bottom1.y, ImColor(col), 1);


						overlay::DrawLine(top1.x, top1.y, top2.x, top2.y, ImColor(col), 1);
						overlay::DrawLine(top2.x, top2.y, top3.x, top3.y, ImColor(col), 1);
						overlay::DrawLine(top3.x, top3.y, top4.x, top4.y, ImColor(col), 1);
						overlay::DrawLine(top4.x, top4.y, top1.x, top1.y, ImColor(col), 1);

					}
					if (IF_AIMKEY())
					{
						if (SDK::Utilities::CheckIfInFOV(pawn, FOVmax)) {

							if (settings::aimbot)
							{
								if (settings::vischeck and IsVisible) {

									SDK::Structs::Vector3 NewAngle = SDK::Utilities::GetSilentRotation(pawn);

									if (NewAngle.x == 0 && NewAngle.y == 0) continue;

									if (settings::smoothing > 0)
										NewAngle = SDK::Utilities::SmoothAngles(SDK::Utilities::CamRot, NewAngle);

									NewAngle.z = 0;

									SDK::Classes::AController::ValidateClientSetRotation(NewAngle, false);
									SDK::Classes::AController::ClientSetRotation(NewAngle, false);
								}
							}
							else if (!settings::vischeck) {

								if (settings::aimbot)
								{
									SDK::Structs::Vector3 NewAngle = SDK::Utilities::GetSilentRotation(pawn);

									if (NewAngle.x == 0 && NewAngle.y == 0) continue;

									if (settings::smoothing > 0)
										NewAngle = SDK::Utilities::SmoothAngles(SDK::Utilities::CamRot, NewAngle);

									NewAngle.z = 0;

									SDK::Classes::AController::ValidateClientSetRotation(NewAngle, false);
									SDK::Classes::AController::ClientSetRotation(NewAngle, false);
								}
							}
						}
							if (settings::Vehicles and strstr(name.c_str(), xorthis("WeakSpot_C"))) {
									float dist;
									uintptr_t VehicleRootComponent = SDK::Utilities::read<uintptr_t>(LocalPawn + 0x130);
									SDK::Structs::Vector3 VehicleRoot = SDK::Utilities::read<SDK::Structs::Vector3>(VehicleRootComponent + 0x11C);
									auto itemPlayerLocation = reinterpret_cast<float*>(reinterpret_cast<PBYTE>(VehicleRootComponent) + SDK::Classes::StaticOffsets::RelativeLocation);
									SDK::Structs::Vector3 VehicleRootW2S;
									SDK::Classes::AController::WorldToScreen(VehicleRoot, &VehicleRootW2S);
									auto itemRoot = GetPawnRootLocations(LocalPawn);
									if (itemRoot) {
										auto itemPos = *itemRoot;
										float x = itemPlayerLocation[0] - itemPos.X;
										float y = itemPlayerLocation[1] - itemPos.Y;
										float z = itemPlayerLocation[2] - itemPos.Z;

										dist = sqrtf((x * x) + (y * y) + (z * z)) / 100.0f;

										
										overlay::OutlinedString(presentwindow, "(WS)", ImVec2(VehicleRootW2S.x, VehicleRootW2S.y), ImColor(54, 186, 166), false);
										if (settings::weakspotaim)
										{
											if (GetAsyncKeyState(VK_SHIFT))
											{
												SDK::Structs::Vector3 NewAngle = SDK::Utilities::GetRotation(VehicleRootComponent);

												if (NewAngle.x == 0 && NewAngle.y == 0) continue;

												if (settings::smoothing > 0)
													NewAngle = SDK::Utilities::SmoothAngles(SDK::Utilities::CamRot, NewAngle);

												NewAngle.z = 0;

												SDK::Classes::AController::ValidateClientSetRotation(NewAngle, false);
												SDK::Classes::AController::ClientSetRotation(NewAngle, false);
											}
										}
									}
									}
						if (settings::NoAnimation and CurrentWeapon) {
							SDK::Utilities::write<bool>(CurrentWeapon + Offsets::bDisableEquipAnimation, true);
						}
						else {
							SDK::Utilities::write<bool>(CurrentWeapon + Offsets::bDisableEquipAnimation, false);
						}
						if (settings::InstaReload and CurrentWeapon and GetAsyncKeyState(0x52)) {
							float a1 = SDK::Utilities::read<float>(CurrentWeapon + Offsets::ReloadTime);
							float b1 = SDK::Utilities::read<float>(CurrentWeapon + Offsets::ReloadScale);
							SDK::Utilities::write<float>(CurrentWeapon + Offsets::ReloadTime, a1 + b1 - 0.101); //reload speed value
						}
						if (settings::InstaReload and CurrentWeapon) {
							float a = SDK::Utilities::read<float>(CurrentWeapon + Offsets::LastReloadTime);
							float b = SDK::Utilities::read<float>(CurrentWeapon + Offsets::LastSuccessfulReloadTime);
							SDK::Utilities::write<float>(CurrentWeapon + Offsets::LastReloadTime, a + b - 1000); //RapidFire value
						}
						DWORD_PTR FindClosestPawn = NULL;
						if (SDK::Utilities::valid_pointer(LocalPawn))
						{
							if (settings::Rapid_Fire)
							{
								if (CurrentWeapon) {

									float x = 0;
									float d = 0;

									x = *(float*)(CurrentWeapon + 0x9BC); // LastFireTime = 0x9BC;

									*(float*)(CurrentWeapon + 0x9BC) = x - 0.003f;

									d = *(float*)(CurrentWeapon + 0x9C0); // LastFireTimVerified = 0x9C0;

									*(float*)(CurrentWeapon + 0x9C0) = 0.101f;
								}
							}

							if (settings::No_Bloom)
							{
								*(float*)(CurrentWeapon + 0x0D90) = 999999.99f;
							}

							if (settings::InstaRevive)
							{
								*(float*)(LocalPawn + 0x33B8) = 0.11f;
							}
							if (settings::AimAir)
							{
								*(bool*)(LocalPawn + 0x3AB1) = true;
							}
						}
					}
				}
				if (settings::BotEsp && strstr(name.c_str(), "PlayerPawn_Athena_Phoebe_C"))
				{ //Headbox.x - 5, Headbox.y - 35,
					SDK::Structs::Vector3 headpos = { 0 };
					SDK::Classes::USkeletalMeshComponent::GetBoneLocation(pawn, 66, &headpos);
					SDK::Classes::AController::WorldToScreen(SDK::Structs::Vector3(headpos.x, headpos.y, headpos.z), &headpos);
					overlay::OutlinedString(presentwindow, "Bot", ImVec2(headpos.x -5, headpos.y -65), ImColor(255, 255, 20), false);
				}
				if (settings::Vehicles)
				{
					if (settings::Vehicles and strstr(name.c_str(), xorthis("Valet_Taxi"))) {
						if (SDK::Utilities::valid_pointer(LocalPawn)) {
							uintptr_t VechicleRootComponent = SDK::Utilities::read<uintptr_t>(pawn + 0x130);
							SDK::Structs::Vector3 VehicleRoot = SDK::Utilities::read<SDK::Structs::Vector3>(VechicleRootComponent + 0x11C);
							SDK::Structs::Vector3  VehicleRootW2S;
							SDK::Classes::AController::WorldToScreen(VehicleRoot, &VehicleRootW2S);

							overlay::OutlinedString(presentwindow, "Taxi", ImVec2(VehicleRootW2S.x, VehicleRootW2S.y), ImColor(255, 255, 20), false);
						}
					}
					else if (settings::Vehicles and strstr(name.c_str(), xorthis("Valet_BigRig")) || strstr(name.c_str(), xorthis("Valet_BasicTr"))) {
						if (SDK::Utilities::valid_pointer(LocalPawn)) {
							uintptr_t VechicleRootComponent = SDK::Utilities::read<uintptr_t>(pawn + 0x130);
							SDK::Structs::Vector3 VehicleRoot = SDK::Utilities::read<SDK::Structs::Vector3>(VechicleRootComponent + 0x11C);
							SDK::Structs::Vector3  VehicleRootW2S;
							SDK::Classes::AController::WorldToScreen(VehicleRoot, &VehicleRootW2S);

							overlay::OutlinedString(presentwindow, "Bigrig Truck", ImVec2(VehicleRootW2S.x, VehicleRootW2S.y), ImColor(255, 255, 20), false);
						}
					}

					else if (settings::Vehicles and strstr(name.c_str(), xorthis("Valet_SportsC")) || strstr(name.c_str(), xorthis("Valet_BasicC"))) {
						if (SDK::Utilities::valid_pointer(LocalPawn)) {
							uintptr_t VechicleRootComponent = SDK::Utilities::read<uintptr_t>(pawn + 0x130);
							SDK::Structs::Vector3 VehicleRoot = SDK::Utilities::read<SDK::Structs::Vector3>(VechicleRootComponent + 0x11C);
							SDK::Structs::Vector3  VehicleRootW2S;
							SDK::Classes::AController::WorldToScreen(VehicleRoot, &VehicleRootW2S);

							overlay::OutlinedString(presentwindow, "Car", ImVec2(VehicleRootW2S.x, VehicleRootW2S.y), ImColor(255, 255, 20), false);
						}
					}
					else if (settings::Vehicles and strstr(name.c_str(), xorthis("MeatballVehicle_L"))) {
						if (SDK::Utilities::valid_pointer(LocalPawn)) {
							uintptr_t VechicleRootComponent = SDK::Utilities::read<uintptr_t>(pawn + 0x130);
							SDK::Structs::Vector3 VehicleRoot = SDK::Utilities::read<SDK::Structs::Vector3>(VechicleRootComponent + 0x11C);
							SDK::Structs::Vector3  VehicleRootW2S;
							SDK::Classes::AController::WorldToScreen(VehicleRoot, &VehicleRootW2S);

							overlay::OutlinedString(presentwindow, "Boat", ImVec2(VehicleRootW2S.x, VehicleRootW2S.y), ImColor(255, 255, 20), false);
						}
					}
				}
				else if (settings::tire and strstr(name.c_str(), ("Tiered_Chest")))
				{
					if (SDK::Utilities::valid_pointer(LocalPawn))
					{

						uintptr_t VechicleRootComponent = SDK::Utilities::read<uintptr_t>(pawn + Offsets::RootComponent);
						SDK::Structs::Vector3 VehicleRoot = SDK::Utilities::read<SDK::Structs::Vector3>(VechicleRootComponent + Offsets::RelativeLocation + Offsets::bAlreadySearched);
						SDK::Structs::Vector3  VehicleRootW2S;
						SDK::Classes::AController::WorldToScreen(VehicleRoot, &VehicleRootW2S);


						overlay::OutlinedString(presentwindow, "Chest", ImVec2(VehicleRootW2S.x, VehicleRootW2S.y), ImColor(255, 255, 20), false);
					}
				}
				else if (settings::tire and strstr(name.c_str(),("FortPickupAthena")))
				{
					if (SDK::Utilities::valid_pointer(LocalPawn))
					{

						uintptr_t VechicleRootComponent = SDK::Utilities::read<uintptr_t>(pawn + Offsets::RootComponent);
						SDK::Structs::Vector3 VehicleRoot = SDK::Utilities::read<SDK::Structs::Vector3>(VechicleRootComponent + Offsets::RelativeLocation);
						SDK::Structs::Vector3  VehicleRootW2S;
						SDK::Classes::AController::WorldToScreen(VehicleRoot, &VehicleRootW2S);


						overlay::OutlinedString(presentwindow, "Tire", ImVec2(VehicleRootW2S.x, VehicleRootW2S.y), ImColor(255, 255, 20), false);
					}
				}
			}
		}
		catch (...) {}
	}
}