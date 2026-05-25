#pragma once
#include <fmt/core.h>
#include "../_UxInterface.h"
#include "../../AppContext.h"
#include "../WindowManager.h"

class ViewProjectTree : public WindowBase
{
public:
	//constructor
	ViewProjectTree(WindowManager& manager) :
		WindowBase("main.project_tree", "Project Tree", true), manager(manager) {}

	void Render(AppContext& context) {
		ImGui::Begin(title.c_str(), nullptr);
		if (!context.projectManager.HasActiveProject()) {
			ImGui::Text("Open or start a new project to begin!");
			ImGui::End();
			return;	//exit early if no project data
		}

		//fmt::println("Rendering window: {:s} {:s}",
		//	title,
		//	context.projectManager.GetCurrentProject()->name);

		ImGui::Text(fmt::format("Project '{:s}'",
			context.projectManager.GetCurrentProject()->name).c_str());

		//std::filesystem::path path = context.projectManager.GetCurrentProject()->path;
		//ImGui::Text(fmt::format("Root: {:s}", path.stem().string()).c_str());

		//fmt::println("Project path: {:s}", path.string());
		//fmt::println("Project root: {:s}", path.stem().string());

		ImGui::Separator();

		if (ImGui::TreeNodeEx(
			fmt::format("root: {:s}", 
				context.projectManager.GetCurrentProject()->path.stem().string()
			).c_str(),
			ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			//todo draw project tree here
			RenderTreeFolders(context, 
				context.projectManager.GetCurrentProject()->folders, 
				false);
			//render folders recursively
			//RenderTreeFolder(folder.second.childFolders, draw_children)
			
			//render files
			//todo files in root folder? - strays

			ImGui::TreePop();
		}

		/*for (auto& folder : context.projectManager.GetCurrentProject()->folders) {
			ImGui::Text(fmt::format("Folder: {:s}", folder.second.name).c_str());
			for (auto& file : folder.second.childFiles) {
				ImGui::Text(fmt::format("File: {:s}", file.second.name).c_str());
			}
		}*/

		ImGui::End();
	}
	void RenderTreeFolders(
		AppContext& context,
		std::unordered_map<UUID, FolderObject>& folders, 
		bool draw_children)
	{
		for (auto& folder : folders) {
			//if (draw_children) {
			//	fmt::println("Got folder: {:s}, id: {}, parent: {}",
			//		folder.second.name,
			//		folder.second.id,
			//		folder.second.parentId
			//	);
			//}

			if ((!draw_children && folder.second.parentId != 0) || folder.second.id == 0)
			{
				//don't draw child folders on first pass
				//also don't re-draw root folder
				continue;
			}

			//fmt::println("Draw folder: {:s}, id: {}, parent: {}", 
			//	folder.second.name,
			//	folder.second.id,
			//	folder.second.parentId
			//);

			//start tree node for current folder
			bool treeOpen = ImGui::TreeNodeEx(
				folder.second.name.c_str(),
				ImGuiTreeNodeFlags_DefaultOpen);

			//context menu for folder
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Rename")) {
					fmt::println("Rename folder: {:s}", folder.second.name);

					//todo handle new folder
				}
				ImGui::Separator();
				if (ImGui::MenuItem("New Folder")) {
					fmt::println("New folder in folder: {:s}", folder.second.name);
					context.workspaceManager.SetSelectedFolder(folder.second.id);
					manager.OpenModal("modal.new_folder");
					//context.projectManager.NewFolder("New Folder", folder.second.id);
					//todo handle new folder
				}
				if (ImGui::MenuItem("New File")) {
					fmt::println("New file in folder: {:s}", folder.second.name);
					//todo handle new file
				}
				ImGui::EndPopup();
			}

			//render tree if open
			if (treeOpen) 
			{
				//todo draw project tree here
				//render folders recursively
				//fmt::println("Draw {} child folders in {}: ", 
				//	folder.second.childFolders.size(), 
				//	folder.second.name);
				std::unordered_map<UUID, FolderObject> childFolders;

				for (auto& childId : folder.second.childFolders) {
					childFolders.insert({ childId, 
						context.projectManager.GetCurrentProject()->folders.find(childId)->second });
				}

				RenderTreeFolders(context, childFolders, true);

				//todo render files

				ImGui::TreePop();
			}
		}
	}

	/* TODO render files in project tree viewer */
private:
	WindowManager& manager;
};