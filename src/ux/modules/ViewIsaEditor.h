#pragma once
#include <fmt/core.h>
#include "../_UxComponent.h"
#include "../../AppContext.h"
#include "../../components/isa/IsaEditorState.h"
//#include "../WindowManager.h"

class ViewIsaEditor : public WindowBase {
public:
	ViewIsaEditor(
		const UUID& docId, 
		WindowManager& mgr, 
		const std::string& id, 
		const std::string& title
	) : 
		WindowBase(id, title, true),
		manager(mgr), 
		documentId(docId)
	{
		state = IsaEditorState();
	}

	void Render(AppContext& context) {

		bool openButton = open;

		ImGui::Begin(title.c_str(), &openButton);

		//render toolbar
		RenderToolbar(context);

		//render panels
		ImGui::BeginChild("IsaNav", ImVec2(240, 0), true);
		//DrawIsaNavigationTree(app, isa, state);
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("IsaMain", ImVec2(0, 0), true);
		//DrawSelectedIsaEditor(app, isa, state);
		ImGui::EndChild();

		//ImGui::SameLine();
		//
		//ImGui::BeginChild("IsaInspector", ImVec2(0, 0), true);
		////DrawIsaInspector(app, isa, state);
		//ImGui::EndChild();

		ImGui::End();

		//send command to close if X pressed
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

	void RenderToolbar(AppContext& context) {
		//if (ImGui::Button("Validate"))
		//{
		//	//todo
		//}
		//
		//ImGui::SameLine();
		//
		//if (ImGui::Button("Save"))
		//{
		//	//todo
		//}
		if (ImGui::BeginTabBar("TabBar")) {
			if (ImGui::BeginTabItem("Details")) {
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Data Model")) {
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Registers")) {
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Instructions")) {
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();

			//ImGui::SameLine();
			//if (ImGui::Button("Validate")) {}

			//ImGui::SameLine();
			//if (ImGui::Button("Save")) {}
		}
	}

private:
	WindowManager& manager;
	UUID documentId;

	IsaEditorState state;
};