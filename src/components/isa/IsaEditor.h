#pragma once

#include "../_IAppComponent.h"
#include "../../core/isa/IsaArchitecture.h"

#include "../project/ProjectManager.h"
//#include "../../AppContext.h"

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

	bool HandleCommand(AppContext& context, AppCommandRequest command)
	{
		std::string commandAction = command.id.substr(
			command.id.find_last_of(".") + 1);

		//todo get file id from command
		if (commandAction == "new_file") {
			return NewDocument(context, command.targetId, command.path);
		}

		if (commandAction == "open_file") {
			return OpenDocument(context, command.targetId, command.path);
		}
		return false;
	}

	bool NewDocument(AppContext& context, UUID fileId, std::string filePath) {
		fmt::println("[IsaEditor] Creating new ISA file {} at path: {:s}", 
			fileId, filePath);

		fmt::println("Project: {:s}", context.projectManager->GetProjectName());
		
		if (!context.projectManager->GetCurrentProject()->
			files.contains(fileId))
		{
			fmt::println("[IsaEditor] Error: File with id {} not found in project.", fileId);
			return false;
		}

		//fmt::println("[IsaEditor] Get file object");
		//FileObject* file = &context.projectManager->
		//	GetCurrentProject()->files.at(fileId);

		std::string fileName = context.projectManager->
			GetCurrentProject()->files.at(fileId).name;

		
		//fmt::println("[IsaEditor] Creating new ISA definition document for file '{:s}' at ''",
		//	fileName);

		UUID documentId = context.projectManager->GetNextUUID();

		//fmt::println("[IsaEditor] Got new ID: {}", documentId);

		//context.projectManager->
		//	GetCurrentProject()->files[fileId].documentId = documentId;
		context.projectManager->SetDocumentIdInFile(fileId, documentId);

		//fmt::println("[IsaEditor] Set documentId: {}", 
		//	context.projectManager->
		//	GetCurrentProject()->files.at(fileId).documentId);

		IsaDefinition document = IsaDefinition();
		document.header.createdUtc = "now"; //todo get current time
		document.header.modifiedUtc = document.header.createdUtc;
		document.header.id = documentId;

		//fmt::println("[IsaEditor] Get file object");

		//put document in editor state
		context.componentContext->
			GetIsaEditorState().openDocuments.emplace(documentId, document);

		//fmt::println("[IsaEditor] Push open document to state");
		//todo open isa editor window - command?

		context.projectManager->SaveProject();

		return true;
	}

	bool OpenDocument(AppContext& context, UUID documentId, std::string filePath) {
		fmt::println("[IsaEditor] Opening ISA document {} at path: {:s}",
			documentId, filePath);
		//todo open file in editor
		if (context.componentContext->GetIsaEditorState().openDocuments.find(documentId)
			== context.componentContext->GetIsaEditorState().openDocuments.end()) 
		{
			fmt::println("[IsaEditor] Document not found. TODO try open from file");
			//todo
			return false;
		}
		
		//document is open
		//push app command to open new vieweditor window with selected document

		return true;
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
private:
	//std::vector<IsaDefinition> openDocuments;
};