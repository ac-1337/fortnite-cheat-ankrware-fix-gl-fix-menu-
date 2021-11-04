#include "utilitys.h"
#include "sdk.h"
#include "OnlyLoop.h"
ID3D11Device* device = nullptr;
ID3D11DeviceContext* immediateContext = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;

HRESULT(*presenth)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
HRESULT(*resizeh)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

WNDPROC WindowProcess = NULL;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwnd = NULL;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) && settings::menu)
	{
		return true;
	}
	return CallWindowProc(WindowProcess, hWnd, msg, wParam, lParam);
}
// combo box stuff here











void gui()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImColor(38, 38, 38, 255);
	colors[ImGuiCol_ButtonHovered] = ImColor(49, 49, 49, 255);
	colors[ImGuiCol_ButtonActive] = ImColor(235, 89, 88, 255);
	colors[ImGuiCol_Button] = ImColor(49, 49, 49, 255);
	colors[ImGuiCol_FrameBgActive] = ImColor(38, 38, 38);
	colors[ImGuiCol_FrameBg] = ImColor(38, 38, 38);
	colors[ImGuiCol_CheckMark] = ImColor(235, 89, 88, 255);
	colors[ImGuiCol_Text] = ImColor(255, 255, 255);

	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 0.0f;
	style->FramePadding = ImVec2(2, 2);
	style->FrameRounding = 0.0f;
	style->ItemSpacing = ImVec2(8, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 0.0f;
	style->GrabMinSize = 10.0f;
	style->GrabRounding = 0.0f;
	style->ChildRounding = 0.f;

	style->WindowTitleAlign.x = 0.50f;
	style->FrameRounding = 2.0f;

	ImGui::SetNextWindowSize({ 260, 420 });
	ImGui::Begin("#Fortnite", NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	int x = 30;
	int y = 20;

	ImVec2 p = ImGui::GetWindowPos();

	Draw::BackgroundFilledRBG(p.x, p.y, 260, 40, ImColor(235, 89, 88, 255));
	Draw::RegularRGBText(p.x + 8, p.y + 10, ImColor(255, 255, 255), xorthis("CheaterMAD.com + horny.win"));

	static int fortnitetab;
	ImGuiStyle* Style = &ImGui::GetStyle();
	ImGui::SetCursorPos({ 15, 60 });
	if (ImGui::Button("##AimBot", ImVec2(70, 28))) fortnitetab = 1;
	ImGui::SameLine();
	if (ImGui::Button("##WallHeck", ImVec2(70, 28))) fortnitetab = 2;
	ImGui::SameLine();
	if (ImGui::Button("##Exploits", ImVec2(70, 28))) fortnitetab = 3;

	if (fortnitetab == 1)
	{
		Draw::RoundedRect(p.x + 15, p.y + 60, 70, 28, ImColor(235, 89, 88, 255));

		ImGui::Checkbox("Enable Aimbot", &settings::aimbot);
		ImGui::Checkbox("Display FOV", &settings::aimbotfov);
		ImGui::Checkbox("Visibility Check", &settings::vischeck);
		std::string FOVAMT = std::to_string(settings::radius);
		std::string SMOOTHAMT = std::to_string(settings::smoothing);

		Draw::RegularRGBText(p.x + 115, p.y + 187, ImColor(255, 255, 255), FOVAMT.c_str());
		Draw::RegularRGBText(p.x + 125, p.y + 244, ImColor(255, 255, 255), SMOOTHAMT.c_str());

		ImGui::Text("Field Of View");
		ImGui::SliderFloat("fov", &settings::radius, 0, 1000);
		ImGui::Text("Aim Smoothing");
		ImGui::SliderFloat("smoothing", &settings::smoothing, 0, 25);
		static const char* HitboxList1[] = { "  Mouse", "  Memory", "  Silent" };
		static int SelectedHitbox1 = 1;

		if (ImGui::Combo("Aimbot", &SelectedHitbox1, HitboxList1, IM_ARRAYSIZE(HitboxList1)))
		{
			if (SelectedHitbox1 == 0)
			{
				settings::aimbot = true;
				settings::silent = false;
			}
			if (SelectedHitbox1 == 1)
			{
				settings::aimbot = true;
				settings::silent = false;
			}
			if (SelectedHitbox1 == 2)
			{
				settings::aimbot = false;
				settings::silent = true;
			}
		}
	}
	if (fortnitetab == 2)
	{
		Draw::RoundedRect(p.x + 92, p.y + 60, 70, 28, ImColor(235, 89, 88, 255));
		ImGui::Checkbox("Distance", &settings::Distance);
		ImGui::Checkbox("Box", &settings::esp);
		ImGui::Checkbox("3D Box", &settings::Box3D);
		ImGui::Checkbox("Lines", &settings::Lines);
		ImGui::Checkbox("Cornered Box", &settings::corner);
		ImGui::Checkbox("Skeletons", &settings::bones);
		ImGui::Checkbox("Vehicles", &settings::Vehicles);
		ImGui::Checkbox("Bot Esp", &settings::BotEsp);
		ImGui::Checkbox("chest", &settings::chest);
		ImGui::Checkbox("Ground Esp", &settings::tire);
		ImGui::Checkbox("Player Name", &settings::name);
	}
	if (fortnitetab == 3)
	{
		Draw::RoundedRect(p.x + 170, p.y + 60, 70, 28, ImColor(235, 89, 88, 255));
		ImGui::Checkbox("Rapid Fire", &settings::Rapid_Fire);
		ImGui::Checkbox("No Bloom", &settings::No_Bloom);
		ImGui::Checkbox("Instant Revive", &settings::InstaRevive);
		ImGui::Checkbox("Aim When in air", &settings::AimAir);
		ImGui::Checkbox("NoAnimation", &settings::NoAnimation);
		ImGui::Checkbox("InstaReload", &settings::InstaReload);
		ImGui::Checkbox("weakspotaim", &settings::weakspotaim);
	}
	Draw::RegularRGBText(p.x + 27, p.y + 65, ImColor(255, 255, 255), "Aimbot");
	Draw::RegularRGBText(p.x + 115, p.y + 65, ImColor(255, 255, 255), "ESP");
	Draw::RegularRGBText(p.x + 190, p.y + 65, ImColor(255, 255, 255), "Misc");

	ImGui::End();
	}
}


float X1 = GetSystemMetrics(0) / 2 - 1;
float Y1 = GetSystemMetrics(1) / 2 - 1;

HRESULT present_hk(IDXGISwapChain* swapchain, UINT sync, UINT flags)
{
	if (!device)
	{
		ID3D11Texture2D* renderTarget = 0;
		ID3D11Texture2D* backBuffer = 0;
		D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
		swapchain->GetDevice(__uuidof(device), (PVOID*)&device);
		device->GetImmediateContext(&immediateContext);

		swapchain->GetBuffer(0, __uuidof(renderTarget), (PVOID*)&renderTarget);
		device->CreateRenderTargetView(renderTarget, nullptr, &renderTargetView);
		renderTarget->Release();
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
		backBuffer->GetDesc(&backBufferDesc);

		Screen_X = backBufferDesc.Width;
		Screen_Y = backBufferDesc.Height;

		backBuffer->Release();
		if (!hwnd)
		{
			hwnd = FindWindowW(L"UnrealWindow", L"Fortnite  ");

			if (!hwnd)
				hwnd = GetForegroundWindow();
		}

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		X = (float)backBufferDesc.Width;
		Y = (float)backBufferDesc.Height;

		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", 18);

		ImGui_ImplDX11_Init(hwnd, device, immediateContext);
		ImGui_ImplDX11_CreateDeviceObjects();
	}
	immediateContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	auto& window = Scene::CreateScene();

	gui();
	SetLoop::PlayersLoop(window);
	if (settings::aimbotfov)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(X1, Y1), settings::radius, ImColor(255, 255, 255, 210), 100, 1);
	}
	if (settings::menu)
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImGui::GetIO().MousePos, ImVec2(ImGui::GetIO().MousePos.x + 7.f, ImGui::GetIO().MousePos.y + 7.f), ImColor(92, 52, 235));
	}

	Scene::DestroyScene(window);
	return presenth(swapchain, sync, flags);
}

HRESULT resize_hk(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
	ImGui_ImplDX11_Shutdown();
	renderTargetView->Release();
	immediateContext->Release();
	device->Release();
	device = nullptr;
	return resizeh(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
}

ImGuiWindow& Scene::CreateScene() {
	ImGui_ImplDX11_NewFrame();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::Begin(("##"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

	auto& io = ImGui::GetIO();
	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

	return *ImGui::GetCurrentWindow();
}

VOID Scene::DestroyScene(ImGuiWindow& window) {
	window.DrawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::Render();
}