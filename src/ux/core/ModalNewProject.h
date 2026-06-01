#pragma once
#include <imgui.h>
#include <string>
#include <nfd.h>
#include <fmt/core.h>
#include "../_UxComponent.h"

class ModalNewProject : public ModalBase
{
public:
	ModalNewProject() : ModalBase("modal.new_project", "New Project") {}
	void Render(AppContext& context) override {
		//fmt::println("Rendering modal: {:s}", title);

		if (showModal) {
			ImGui::OpenPopup(title.c_str());
			showModal = false;
			open = true;
		}

		//ImGui::SetNextWindowSize(ImVec2(600, 200));
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(520, 0), ImGuiCond_Appearing);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings;

		if (ImGui::BeginPopupModal(title.c_str(), nullptr, flags)) {
			static char projectName[128] = "new project";
			static char projectPath[256] = "root folder path";

			ImGui::TextUnformatted("Enter details for new project");
			ImGui::Separator();

			ImGui::Spacing();

			ImGui::TextUnformatted("Name");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::InputText("##ProjectName", projectName, IM_ARRAYSIZE(projectName));

			ImGui::Spacing();

			ImGui::TextUnformatted("Path");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::InputText("##ProjectPath", projectPath, IM_ARRAYSIZE(projectPath));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			const char* browseLabel = "Browse Folder";
			const char* createLabel = "Create";
			const char* cancelLabel = "Cancel";

			const float paddingX = ImGui::GetStyle().FramePadding.x * 2.0f;

			const float browseButtonWidth = ImGui::CalcTextSize(browseLabel).x + paddingX;
			const float createButtonWidth = ImGui::CalcTextSize(createLabel).x + paddingX;
			const float cancelButtonWidth = ImGui::CalcTextSize(cancelLabel).x + paddingX;

			const float spacing = ImGui::GetStyle().ItemSpacing.x;
			const float availableWidth = ImGui::GetContentRegionAvail().x;

			const float rightGroupWidth =
				createButtonWidth +
				spacing +
				cancelButtonWidth;
			//ImGui::SameLine();

			//Browse folder button and native file dialog
			if (ImGui::Button(browseLabel, ImVec2(browseButtonWidth, 0))) {
				nfdu8char_t* outPath;
				nfdresult_t result = NFD_PickFolderU8(&outPath, "");
				if (result == NFD_OKAY) {
					/* Path entered */
					fmt::println("NFD got path: {:s}", outPath);
					std::strncpy(projectPath, outPath, sizeof(projectPath));
					projectPath[sizeof(projectPath) - 1] = '\0';
					// Remember to free the allocated path buffer when finished!
					NFD_FreePathU8(outPath);
				}
				else if (result == NFD_CANCEL) {
					fmt::println("NFD user pressed cancel");
				}
				else {
					fmt::println("NFD error occurred");
				}
			}

			ImGui::SameLine();

			const float rightGroupStartX =
				ImGui::GetCursorPosX() +
				availableWidth -
				browseButtonWidth -
				spacing -
				rightGroupWidth;

			ImGui::SetCursorPosX(rightGroupStartX);

			if (ImGui::Button(createLabel, ImVec2(createButtonWidth, 0))) {
				if (context.projectManager->NewProject(
					std::string(projectName), std::filesystem::path(projectPath)))
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