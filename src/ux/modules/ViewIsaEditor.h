#pragma once
#include <fmt/core.h>
#include "../_UxComponent.h"
#include "../../AppContext.h"
//#include "../WindowManager.h"

class ViewIsaEditor : public WindowBase {
public:
	ViewIsaEditor(const UUID& docId, WindowManager& mgr, const std::string id) : 
		WindowBase(id, "ISA Editor", true),
		manager(mgr), documentId(docId) {};

	void Render(AppContext& context) {

		bool openButton = open;

		ImGui::Begin(title.c_str(), &openButton);

		//todo render contents

		ImGui::End();

		if (!openButton) {
			//todo close all the way??
			//manager.RemoveWindow("main.isa_editor_" + std::to_string(documentId));
			context.appCommandQueue->Push(AppCommandRequest{
				.command = AppCommand::CloseWindow,
				.id = id,
				.targetId = documentId
			});
		}
	}
private:
	WindowManager& manager;
	UUID documentId;
};