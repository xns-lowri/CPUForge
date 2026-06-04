#pragma once

#include "../_IAppComponent.h"
#include "../../core/isa/IsaArchitecture.h"

class IsaEditor : public ComponentBase
{
public:
	IsaEditor() : ComponentBase("isa_editor") {};

	std::vector<FileTypeDescriptor> GetSupportedFileTypes() const {
		std::vector<FileTypeDescriptor> returnList = std::vector<FileTypeDescriptor>();

		returnList.push_back(FileTypeDescriptor{
				.id = "isa.definition",
				.displayName = "ISA Definiton",
				.extension = ".isa" ,
				.type = FileType::ISADefinition
			//	//icon
			//	//.defaultOpenCommand = "isa.open"
		});

		return returnList;
	}

	std::vector<TreeActionDescriptor> GetTreeActionsFolder() const {
		std::vector<TreeActionDescriptor> returnList 
			= std::vector<TreeActionDescriptor>();
		returnList.emplace_back(TreeActionDescriptor{
			.action = "isa_editor.new_file",
			.displayName = "New ISA File"
		});
		return returnList;
	}

	std::vector<TreeActionDescriptor> GetTreeActionsFile() const {
		std::vector<TreeActionDescriptor> returnList 
			= std::vector<TreeActionDescriptor>();
		returnList.emplace_back(TreeActionDescriptor{
			.action = "isa_editor.open_file",
			.displayName = "Open"
		});
		return returnList;
	}

	bool HandleCommand(
		const std::string& command,
		const std::string& path)
	{
		if (command == "isa_editor.new_file") {
			return NewFile(path);
		}
		if (command == "isa_editor.open_file") {
			return OpenFile(path);
		}
		return false;
	}

	bool NewFile(std::string filePath) {
		fmt::println("Creating new ISA file at path: {:s}", filePath);
		return false;
	}

	bool OpenFile(std::string filePath) {
		//todo open file in editor
		return false;
	}

	//void Register(AppComponentRegistry& registry) override {
		//register file types
		//registry.RegisterFileType(FileTypeDescriptor{
		//	.id = "isa.definition",
		//	.displayName = "ISA Definiton",
		//	.extensions = { ".isa" },
		//	//icon
		//	//.defaultOpenCommand = "isa.open"
		//});

		//register tree action provider

		//register project template provider?
	//}
};