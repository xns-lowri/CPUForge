#pragma once
#include <imgui.h>
#include <string>
#include <fmt/core.h>
#include "../_UxComponent.h"

class ModalNewFolder : public ModalBase
{
public:
	ModalNewFolder() : ModalBase("modal.new_folder", "New Folder") {}
	void Render(AppContext& context) override {
		//todo
		if (showModal) {
			ImGui::OpenPopup(title.c_str());
			showModal = false;
			open = true;
		}

		//ImGui::SetNextWindowSize(ImVec2(600, 200));
		//ImVec2 center = ImGui::GetMainViewport()->GetCenter();

		ImVec2 center = ImGui::GetMousePos();
		center.x += 240;
		center.y += 50;

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(480, 0), ImGuiCond_Appearing);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings;

		if (ImGui::BeginPopupModal(title.c_str(), nullptr, flags)) {
			static char folderName[128] = "new folder";

			ImGui::TextUnformatted("Folder name:");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::InputText("##FolderName", folderName, IM_ARRAYSIZE(folderName));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			const char* createLabel = "Create";
			const char* cancelLabel = "Cancel";

			const float paddingX = ImGui::GetStyle().FramePadding.x * 2.0f;

			const float createButtonWidth = ImGui::CalcTextSize(createLabel).x + paddingX;
			const float cancelButtonWidth = ImGui::CalcTextSize(cancelLabel).x + paddingX;

			const float spacing = ImGui::GetStyle().ItemSpacing.x;
			const float availableWidth = ImGui::GetContentRegionAvail().x;

			const float rightGroupWidth =
				createButtonWidth +
				spacing +
				cancelButtonWidth;
			//ImGui::SameLine();

			const float rightGroupStartX =
				ImGui::GetCursorPosX() +
				availableWidth -
				spacing -
				rightGroupWidth;

			ImGui::SetCursorPosX(rightGroupStartX);

			if (ImGui::Button(createLabel, ImVec2(createButtonWidth, 0))) {
				//todo not this
				if (context.projectManager->NewFolder(
					std::string(folderName), 
					context.workspaceManager->GetSelectedFolder(),
					FolderProperties{ true, true, true },
					std::nullopt) != 0)
				{
					//project created, close modal
					ImGui::CloseCurrentPopup();
					open = false;
				}
				else {
					//project creation failed successfully, show error
					//todo
				}
			}

			ImGui::SameLine();

			if (ImGui::Button(cancelLabel, ImVec2(cancelButtonWidth, 0))) {
				ImGui::CloseCurrentPopup();
				open = false;
			}
			ImGui::EndPopup();
		}
	}
};