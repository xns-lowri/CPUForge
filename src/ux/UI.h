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
#include "../project/Project.h"
#include "WindowManager.h"

#include "../AppContext.h"

class UI
{
public:
	int Init();
	bool Render(AppContext& context);
	int Close();
private:
	SDL_Window* window = nullptr;
	ImGuiViewport* viewport = nullptr;
	SDL_GLContext gl_context;

	ImGuiID last_selected = NULL;

	WindowManager windowManager;

	bool renderMainMenuBar(AppContext& context);
};