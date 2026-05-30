#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include "../_types.h"
//#include "../ux/_UxInterface.h"


//todo add separate file with struct for all saveable state in workspace, 
//e.g. open documents, selected items, etc. and serialize to disk on close and load on open

class WorkspaceManager
{
private:
	UUID lastFolderInTree = 0;
	UUID lastFileInTree = 0;
	//todo field for last selected editor window 
	//todo fields for open documents
	//std::unordered_map<std::string, std::unique_ptr<IModal>> modals;
	//std::unordered_map<std::string, std::unique_ptr<IWindow>> windows;
public:
	WorkspaceManager() = default;
	const UUID GetSelectedFolder() const;
	const UUID GetSelectedFile() const;
	void SetSelectedFolder(UUID folderId);
	void SetSelectedFile(UUID fileId);
	//void ToggleWindowVisibility(std::string windowId);
	//todo methods for opening documents etc
	//std::unordered_map<std::string, std::unique_ptr<IModal>>& GetModalMap();
	//std::unordered_map<std::string, std::unique_ptr<IWindow>>& GetWindowMap();
};