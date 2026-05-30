#pragma once
#include <imgui.h>
#include <string>
#include <nfd.h>
#include <fmt/core.h>
#include "../_UxInterface.h"

class ModalOpenProject : public ModalBase
{
public:
	ModalOpenProject() : ModalBase("modal.open_project", "Open Project") {}
	void Render(AppContext& context) override {
		if (showModal) {
			//ImGui::OpenPopup(title.c_str());

			nfdu8char_t* inPath;
			nfdu8filteritem_t filters[2] = { { "Project Files", "json" } };
			nfdopendialogu8args_t args = { 0 };
			args.filterList = filters;
			args.filterCount = 1;
			nfdresult_t result = NFD_OpenDialogU8_With(&inPath, &args);

			if (result == NFD_OKAY) {
				/* Path entered */
				fmt::println("NFD got path: {:s}", inPath);
				std::filesystem::path projectFilePath(inPath);
				context.projectManager.OpenProject(projectFilePath);
				// Remember to free the allocated path buffer when finished!
				NFD_FreePathU8(inPath);
			}
			else if (result == NFD_CANCEL) {
				fmt::println("NFD user pressed cancel");
			}
			else {
				fmt::println("NFD error occurred");
			}

			showModal = false;
		}
	}
};