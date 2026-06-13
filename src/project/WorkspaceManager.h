#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include "../_types.h"
#include "enumProjectDef.h"
//#include "../ux/_UxInterface.h"


//todo add separate file with struct for all saveable state in workspace, 
//e.g. open documents, selected items, etc. and serialize to disk on close and load on open

struct WorkspaceManager {
	UUID lastFolderInTree = 0;
	UUID lastFileInTree = 0;

	UUID activeDocument = 0;
	std::string lastEditor = "";

	std::string lastAction = "";
	std::string lastPath = "";
};

/*
class WorkspaceManager
{
private:
	UUID lastFolderInTree = 0;
	UUID lastFileInTree = 0;
	UUID activeDocument = 0;
	//FolderKind lastFolderKind = FolderKind::General;
	//FileType lastFileType = FileType::Unknown;
	std::string lastAction = "";
	std::string lastPath = "";
	//AppCommand lastCommand = AppCommand::About;

	//todo field for last selected editor window 
	//todo fields for open documents
	//std::unordered_map<std::string, std::unique_ptr<IModal>> modals;
	//std::unordered_map<std::string, std::unique_ptr<IWindow>> windows;
public:
	WorkspaceManager() = default;

	const UUID GetSelectedFolder() const;
	const UUID GetSelectedFile() const;
	const UUID GetActiveDocument() const;
	const std::string GetAction() const;
	const std::string GetPath() const;

	void SetSelectedFolder(UUID folderId);
	void SetSelectedFile(UUID fileId);
	void SetActiveDocument(UUID documentId);
	void SetAction(std::string action);
	void SetPath(std::string path);
	//void ToggleWindowVisibility(std::string windowId);
	//todo methods for opening documents etc
	//std::unordered_map<std::string, std::unique_ptr<IModal>>& GetModalMap();
	//std::unordered_map<std::string, std::unique_ptr<IWindow>>& GetWindowMap();
};*/