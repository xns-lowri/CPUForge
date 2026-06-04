#include "AppComponentRegistry.h"

#include "../components/isa/IsaEditor.h"

AppComponentRegistry::AppComponentRegistry() {
	//fmt::println("***** Initializing AppComponentRegistry *****");
	componentsById = std::unordered_map<
		std::string, 
		std::unique_ptr<ComponentBase>
	>();

	fileTypesByExtension = std::unordered_map <
		std::string,
		FileTypeDescriptor
	>();

	//register isa editor module
	//std::unique_ptr<IsaEditor> isaEditor = std::make_unique<IsaEditor>();
	componentsById.emplace("isa_editor", std::make_unique<IsaEditor>());
	componentIdsByFolder.emplace(FolderKind::ISA, "isa_editor");

	std::vector<FileTypeDescriptor> descriptors = 
		componentsById["isa_editor"]->GetSupportedFileTypes();
	RegisterFileTypes(descriptors);

	//todo add other modules

	//todo remove debug print
	for (auto& component : componentsById) {
		fmt::println("Registered component: {:s}", component.second->GetID());
	}

	for (auto& file : fileTypesByExtension) {
		fmt::println("Registered file type: {:s} ({:s})",
			file.second.displayName, file.second.extension);
	}
}

bool AppComponentRegistry::RegisterFileTypes(
	std::vector<FileTypeDescriptor> descriptors) 
{
	for (const FileTypeDescriptor& descriptor : descriptors) {
		if (fileTypesByExtension.find(descriptor.extension) 
			!= fileTypesByExtension.end()) 
		{
			fmt::println("Error: File extension '{}' is already registered.", descriptor.extension);
			return false;
		}

		fileTypesByExtension.emplace(descriptor.extension, descriptor);
		
	}
	return true;
}

std::vector<TreeActionDescriptor> AppComponentRegistry::GetTreeActionsFolder(FolderKind folderKind) const 
{
	std::vector<TreeActionDescriptor> returnList
		= std::vector<TreeActionDescriptor>();

	if (componentIdsByFolder.find(folderKind) == componentIdsByFolder.end()) {
		fmt::println("Warning: No component registered for folder kind '{:s}'",
			ToString(folderKind));
		return returnList;
	}
	if (componentsById.find(componentIdsByFolder.at(folderKind)) == componentsById.end()) {
		fmt::println("Error: No component registered with id '{:s}' for folder kind '{:s}'",
			componentIdsByFolder.at(folderKind), ToString(folderKind));
		return returnList;
	}

	ComponentBase& component = *componentsById.at(componentIdsByFolder.at(folderKind));

	return component.GetTreeActionsFolder();
}

//todo get tree actions for file
std::vector<TreeActionDescriptor> AppComponentRegistry::GetTreeActionsFile(FileType fileType) const 
{
	std::vector<TreeActionDescriptor> returnList
		= std::vector<TreeActionDescriptor>();
	//todo
	return returnList;
}

//bool AppComponentRegistry::RegisterComponent(ComponentBase& component) {
//	const std::string& id = component.GetID();
//	if (componentsById.find(id) != componentsById.end()) {
//		fmt::println("Error: Component with ID '{}' is already registered.", id);
//		return false;
//	}
//	componentsById[id] = component;
//	return true;
//};
bool AppComponentRegistry::HandleCommand(
	const std::string& command,
	const std::string& path) 
{
	//todo delegation to modules
	fmt::println("AppComponentRegistry: {:s} at path '{:s}'", command, path);

	std::string componentId = command.substr(0, command.find('.'));
	fmt::println("ComponentId: {:s}", componentId);

	if (componentsById.find(componentId) == componentsById.end()) {
		fmt::println("Error: No component registered with id '{:s}' for command '{:s}'",
			componentId, command);
		return false;
	}

	componentsById[componentId]->HandleCommand(command, path);

	return true;
}


