#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#include <vector>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <Psapi.h>

#include <sstream>
#include <algorithm>

#include "hook.h"

namespace Scene
{
	ImGuiWindow& CreateScene();
	VOID DestroyScene(ImGuiWindow& window);
}

