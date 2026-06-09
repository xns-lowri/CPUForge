#pragma once

#include "../_IAppComponent.h"
#include "../../core/isa/IsaDefinition.h"

#include "../project/ProjectManager.h"
#include "../../project/FileHandler.h"
//#include "../../AppContext.h"

#include "../../ux/modules/ViewIsaEditor.h"

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
		AppContext& context, 
		WindowManager& window, 
		AppCommandRequest command)
	{
		std::string commandAction = command.id.substr(
			command.id.find_last_of(".") + 1);

		//todo get file id from command
		if (commandAction == "new_file") {
			return NewDocument(context, command.targetId, command.path);
		}

		if (commandAction == "open_file") {
			return OpenDocument(context, window, command.targetId, command.path);
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

		std::filesystem::path fileFullPath = context.projectManager->
			GetCurrentProject()->files.at(fileId).path;

		fileFullPath = context.projectManager->GetCurrentProject()->path / fileFullPath;
		
		//fmt::println("[IsaEditor] Creating new ISA definition document for file '{:s}' at ''",
		//	fileName);
		fmt::println("[IsaEditor] Got file path: {:s}",
			fileFullPath.string());

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

		document.architectureName = fileName;

		//fmt::println("[IsaEditor] Get file object");

		//put document in editor state
		context.componentContext->
			GetIsaDocuments().openDocuments.emplace(documentId, document);

		//fmt::println("[IsaEditor] Push open document to state");
		//todo open isa editor window - command?

		context.projectManager->SaveProject();

		//TODO save file with document data
		//json j = document;
		//context.projectManager->SaveFile();
		ProjectResult<void> result = FileHandler::Save(fileFullPath, document);

		return true;
	}

	bool OpenDocument(
		AppContext& context,
		WindowManager& window,
		UUID fileId, 
		std::string filePath) 
	{
		fmt::println("[IsaEditor] Opening file {} at path: {:s}",
			fileId, filePath);
		//todo open file in editor
		if (!context.projectManager->GetCurrentProject()->files.contains(fileId))
		{
			fmt::println("[IsaEditor] Error: File with id {} not found in project.", fileId);
			return false;
		}

		UUID documentId = context.projectManager->
			GetCurrentProject()->files.at(fileId).documentId;

		if (context.componentContext->GetIsaDocuments().openDocuments.find(documentId)
			== context.componentContext->GetIsaDocuments().openDocuments.end()) 
		{
			fmt::println("[IsaEditor] Try open from file");
			//todo

			std::filesystem::path fileFullPath = context.projectManager->
				GetCurrentProject()->files.at(fileId).path;

			fileFullPath = context.projectManager->GetCurrentProject()->path / fileFullPath;

			fmt::println("[IsaEditor] Opening file {}", fileFullPath.string());
			auto result = FileHandler::Load<IsaDefinition>(fileFullPath);

			if (result.ok) {
				//put document in editor state
				context.componentContext->
					GetIsaDocuments().openDocuments.emplace(documentId, result.value);

				fmt::println("[IsaEditor] Got file {}", documentId);

				fmt::println("Endianness: {}", ToString(result.value.endianness));
			}
		}

		//retry load
		if (context.componentContext->GetIsaDocuments().openDocuments.find(documentId)
			== context.componentContext->GetIsaDocuments().openDocuments.end())
		{
			fmt::println("[IsaEditor] Error: Document not found - cannot open");
			//todo
			return false;
		}
		
		//document is open
		//TODO push app command to open new vieweditor window with selected document

		std::string title = "IsaEditor - " + context.projectManager->
			GetCurrentProject()->files.at(fileId).name;

		fmt::println("[IsaEditor] Opening window");
		bool windowOpened = window.OpenIsaEditor(context, documentId, title);

		return windowOpened;
	}

private:
	//std::vector<IsaDefinition> openDocuments;
};