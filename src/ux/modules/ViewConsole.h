#pragma once
#include <fmt/core.h>
#include "../_UxComponent.h"
#include "../../AppContext.h"
#include "../WindowManager.h"

class ViewConsole : public WindowBase {
public:
	ViewConsole(WindowManager& manager) :
		WindowBase("main.console", "Console", true), manager(manager) {}

	void Render(AppContext& context) {
		ImGui::Begin(title.c_str(), nullptr);

		ImGui::End();
	}

private:
	WindowManager& manager;
};