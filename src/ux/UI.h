#pragma once
#include <fmt/core.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <optional>

#include <nfd.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include "../../external/imgui_impl_sdl2.h"
#include "../../external/imgui_impl_opengl3.h"

//include simulation headers
#include "../project/ProjectData.h"
#include "WindowManager.h"
#include "menubar/ImGuiMenuHandler.h"

#include "KeyHandler.h" //keyboard scanner

#include "../AppContext.h"

#ifdef __APPLE__
#include "menubar/MacOsNative.h"
#endif

class UI
{
public:
	UI(WindowManager& mgr) : windowManager(mgr) {};
	int Init(AppContext& context);
	bool Render(AppContext& context);
	int Close();
	WindowManager& GetWindowManager();
private:
	SDL_Window* window = nullptr;
	ImGuiViewport* viewport = nullptr;
	SDL_GLContext glContext;

	//ImGuiID lastSelected = NULL; //moved to editor state

	WindowManager& windowManager;
};