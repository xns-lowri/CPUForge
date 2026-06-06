#pragma once
#include <fmt/core.h>
#include <string>
#include <unordered_map>
#include <memory>

#include "../ux/WindowManager.h"

#include "../components/_IAppComponent.h"
#include "../enumAppCommands.h"

#include "../project/ProjectManager.h"
#include "../AppContext.h"

class AppComponentRegistry {
public:
	//AppComponentRegistry() = default;
	//~AppComponentRegistry() = default;

	AppComponentRegistry();

	//bool RegisterComponent(ComponentBase& component);
	bool RegisterFileTypes(
		std::vector<FileTypeDescriptor> descriptors);

	void UpdateComponentContext(AppContext& context);

	std::vector<TreeActionDescriptor> GetTreeActionsFolder(FolderKind folderKind) const;

	//todo get tree actions for file
	std::vector<TreeActionDescriptor> GetTreeActionsFile(FileType fileType) const;
	
	bool HandleCommand(
		AppContext& context, 
		WindowManager& window, 
		AppCommandRequest command);
	//bool HandleOpenProjectFile(const std::string& path);

	//connect to handlers for file explorer tree
	//void RegisterFileType(FileTypeDescriptor descriptor);	
private:
	//
	std::unordered_map<std::string, std::unique_ptr<ComponentBase>> componentsById;
	std::unordered_map<FolderKind, std::string> componentIdsByFolder;

	std::unordered_map<std::string, FileTypeDescriptor> fileTypesByExtension;
};