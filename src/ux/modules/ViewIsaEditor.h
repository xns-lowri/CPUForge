#pragma once
#include <fmt/core.h>
#include "../_UxInterface.h"
#include "../../AppContext.h"
//#include "../WindowManager.h"

class ViewIsaEditor : public WindowBase {
public:
	ViewIsaEditor() = default;

	void Render(AppContext& context) {
		ImGui::Begin(title.c_str(), nullptr);

		//todo render contents

		ImGui::End();
	}
};