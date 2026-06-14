#pragma once
#include <imgui.h>
#include <string>
#include <fmt/core.h>
#include "../../project/Project.h"
#include "../_UxComponent.h"

class ModalNewFile : public ModalBase
{
public:
	ModalNewFile() : ModalBase("modal.new_file", "New File") {}
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
			static char fileNameInput[128] = "new file";

			ImGui::TextUnformatted("Filename:");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::InputText("##FileName", fileNameInput, IM_ARRAYSIZE(fileNameInput));

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
				//get file kind for selected folder
				UUID currentFolderId = 
					context.workspaceManager->lastFolderInTree;
				FolderKind currentFolderKind = 
					context.projectManager->
					GetCurrentProject()->
					folders.find(currentFolderId)->second.type;
				FileType fileType = FileTypeFromFolderKind(currentFolderKind);
				std::string fixedExtention = FixedExtensionFromFileType(fileType);

				std::string fileName = std::string(fileNameInput);

				if (fixedExtention != "" && fileName.substr(fileName.size() - fixedExtention.size()) != fixedExtention) {
					fileName += fixedExtention;
				}

				std::string fileExtension = fileName.substr(fileName.find_last_of('.'));
				
				//create file in project manager
				UUID newFileId = context.projectManager->NewFile(
					fileName,
					fileExtension,
					fileType,
					currentFolderId,
					context.projectManager->GetNextUUID());

				if (newFileId != 0)
				{
					//file created, close modal
					ImGui::CloseCurrentPopup();
					open = false;
					//push app request to create editor state for new file
					context.appCommandQueue->Push(AppCommandRequest{
						.command = AppCommand::NewFile,
						.id = context.workspaceManager->lastAction,
						.path = context.workspaceManager->lastPath,
						.targetId = newFileId
					});
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