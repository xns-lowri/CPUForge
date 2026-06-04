#pragma once
#include <fmt/core.h>
#include "../_UxComponent.h"
#include "../../AppContext.h"
//#include "../WindowManager.h"

class ViewIsaEditor : public WindowBase {
public:
	ViewIsaEditor(const std::string& id, WindowManager& mgr) : 
		WindowBase("main.isa_editor." + id, "ISA Editor", true), manager(mgr) {};

	void Render(AppContext& context) {

		ImGui::Begin(title.c_str(), nullptr);

		//todo render contents

		ImGui::End();
	}
private:
	WindowManager& manager;
};