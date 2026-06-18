#pragma once

#include <imgui.h>
#include <imgui_stdlib.h>

#include "../_types.h"
#include "../project/Project.h"
#include "../AppContext.h"

class KeyHandler {
public:
	static void ScanKeys(AppContext& context) {
        //todo check against list of key combos?

        if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
            //save
            fmt::println("[Keyboard Handler] Save current file");
            context.appCommandQueue->Push(AppCommand::Save);
        }
	}
};