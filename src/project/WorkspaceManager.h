#pragma once

//todo add separate file with struct for all saveable state in workspace, 
//e.g. open documents, selected items, etc. and serialize to disk on close and load on open

class WorkspaceManager
{
private:
	UUID lastFolderInTree = 0;
	UUID lastFileInTree = 0;
	//todo field for last selected editor window 
	//todo fields for open documents
public:
	WorkspaceManager() = default;
	const UUID GetSelectedFolder() const;
	const UUID GetSelectedFile() const;
	void SetSelectedFolder(UUID folderId);
	void SetSelectedFile(UUID fileId);
	//todo methods for opening documents etc

};