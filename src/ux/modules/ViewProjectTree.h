#pragma once
#include <fmt/core.h>
#include "../_UxComponent.h"
#include "../../AppContext.h"
#include "../WindowManager.h"

class ViewProjectTree : public WindowBase {
public:
	//constructor
	ViewProjectTree(WindowManager& manager) :
		WindowBase("main.project_tree", "Project Tree", true), manager(manager) {}

	void Render(AppContext& context) {
		ImGui::Begin(title.c_str(), nullptr);
		if (!context.projectManager->HasActiveProject()) {
			ImGui::Text("Open or start a new project to begin!");
			ImGui::End();
			return;	//exit early if no project data
		}

		//fmt::println("Rendering window: {:s} {:s}",
		//	title,
		//	context.projectManager.GetCurrentProject()->name);

		ImGui::Text(fmt::format("Project '{:s}'",
			context.projectManager->GetCurrentProject()->name).c_str());

		//std::filesystem::path path = context.projectManager.GetCurrentProject()->path;
		//ImGui::Text(fmt::format("Root: {:s}", path.stem().string()).c_str());

		//fmt::println("Project path: {:s}", path.string());
		//fmt::println("Project root: {:s}", path.stem().string());

		ImGui::Separator();

		if (ImGui::TreeNodeEx(
			fmt::format("root: {:s}", 
				context.projectManager->GetCurrentProject()->path.stem().string()
			).c_str(),
			ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			//todo draw project tree here
			RenderTreeFolders(context, 
				context.projectManager->GetCurrentProject()->folders, 
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
		std::map<UUID, FolderObject>& folders, 
		bool draw_children)
	{
		for (auto& folder : folders) {
			if ((!draw_children && folder.second.parentId != 0) || folder.second.id == 0)
			{
				//don't draw child folders on first pass
				//also don't re-draw root folder
				continue;
			}

			//start tree node for current folder
			bool treeOpen = ImGui::TreeNodeEx(
				folder.second.name.c_str(),
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_SpanAvailWidth
			);

			//context menu for folder
			if (folder.second.properties.canRename ||
				folder.second.properties.canAddFolders ||
				folder.second.properties.canAddFiles)
			{
				if (ImGui::BeginPopupContextItem()) {
					/* TODO BIG REFACTOR NEEDED */
					//add context menu from module??

					if (folder.second.properties.canRename) {
						if (ImGui::MenuItem("Rename")) {
							fmt::println("Rename folder: {:s}", folder.second.name);

							//todo handle rename
							/* command queue */
						}
					}

					if (folder.second.properties.canRename && (
						folder.second.properties.canAddFolders ||
						folder.second.properties.canAddFiles))
					{
						ImGui::Separator();
					}

					if (folder.second.properties.canAddFolders) {
						if (ImGui::MenuItem("New Folder")) {
							fmt::println("New folder in folder: {:s}", folder.second.name);
							context.workspaceManager->SetSelectedFolder(folder.second.id);
							manager.OpenModal(context, "modal.new_folder");
							//context.projectManager.NewFolder("New Folder", folder.second.id);
							//todo handle new folder
						}
					}

					if (folder.second.properties.canAddFiles) {
						for (auto& action : context.componentContext->GetTreeActionsFolder(folder.second.type)) {
							//todo may need to refactor if tree actions don't all create new file...
							if (ImGui::MenuItem(action.displayName.c_str())) {
								fmt::println("Action '{:s}' in folder: {:s}",
									action.action, folder.second.path);
								//action.action(context, folder.second.id);
								//context.appCommandQueue->Push(AppCommandRequest{
								//	.command = AppCommand::NewFile,
								//	.id = action.action,
								//	.path = folder.second.path	
								//});
								std::string componentId = action.action.substr(0, action.action.find('.'));
								std::string actionId = action.action.substr(action.action.find_last_of('.') + 1);
								context.workspaceManager->SetSelectedFolder(folder.second.id);
								context.workspaceManager->SetAction(action.action);
								context.workspaceManager->SetPath(folder.second.path);

								//fmt::println(" ***** Action: {:s}", actionId);

								if (actionId == "new_file") {
									//todo check if action needs modal, else push command
									manager.OpenModal(context, "modal." + actionId);
								}
								else {
									//todo
								}
							}
						}
						//if (ImGui::MenuItem("New File")) {
							//fmt::println("New file in folder: {:s}", folder.second.name);
							//todo handle new file
						//}
					}

					ImGui::EndPopup();
				}
			}
			//end context menu for current folder

			//render tree if open
			if (treeOpen) 
			{
				//todo draw project tree here
				//render folders recursively
				//fmt::println("Draw {} child folders in {}: ", 
				//	folder.second.childFolders.size(), 
				//	folder.second.name);
				std::map<UUID, FolderObject> childFolders;

				for (auto& childId : folder.second.childFolders) {
					childFolders.insert({ childId, 
						context.projectManager->GetCurrentProject()->
						folders.find(childId)->second 
					});
				}

				RenderTreeFolders(context, childFolders, true);

				//render files
				for (auto& fileId : folder.second.childFiles) {
					//FileObject file = 
					//	context.projectManager->
					//	GetCurrentProject()->
					//	files.at(fileId);
					FileObject curFile = context.projectManager->
						GetCurrentProject()->
						files.at(fileId);

					bool leafOpen = ImGui::TreeNodeEx(
						curFile.name.c_str(),
						ImGuiTreeNodeFlags_SpanAvailWidth |
						ImGuiTreeNodeFlags_Leaf);

					if (ImGui::BeginPopupContextItem()) {
						//file context menu
						for (auto& action : context.componentContext->GetTreeActionsFile(curFile.type)) {
							//todo may need to refactor if tree actions don't all create new file...
							if (ImGui::MenuItem(action.displayName.c_str())) {
								fmt::println("Action '{:s}' in folder: {:s}",
									action.action, folder.second.path);
								//action.action(context, folder.second.id);
								//context.appCommandQueue->Push(AppCommandRequest{
								//	.command = AppCommand::NewFile,
								//	.id = action.action,
								//	.path = folder.second.path	
								//});
								std::string componentId = action.action.substr(0, action.action.find('.'));
								std::string actionId = action.action.substr(action.action.find_last_of('.') + 1);
								context.workspaceManager->SetSelectedFolder(folder.second.id);
								context.workspaceManager->SetAction(action.action);
								context.workspaceManager->SetPath(curFile.path);

								//fmt::println(" ***** Action: {:s}", actionId);
								context.appCommandQueue->Push(AppCommandRequest{
									.command = AppCommand::OpenFile,
									.id = context.workspaceManager->GetAction(),
									.path = context.workspaceManager->GetPath(),
									.targetId = curFile.documentId
								});
							}
						}

						ImGui::EndPopup();
					}

					if (leafOpen) 
					{
						  ImGui::TreePop();
					}

				}
					
				ImGui::TreePop();
			}
		}
	}

	/* TODO render files in project tree viewer */
private:
	WindowManager& manager;
};