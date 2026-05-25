#include "../AppContext.h"
#include "WorkspaceManager.h"

const UUID WorkspaceManager::GetSelectedFolder() const {
	return lastFolderInTree;
}

const UUID WorkspaceManager::GetSelectedFile() const {
	return lastFileInTree;
}

void WorkspaceManager::SetSelectedFolder(UUID folderId) {
	//todo check folder exists
	lastFolderInTree = folderId;
}

void WorkspaceManager::SetSelectedFile(UUID fileId) {
	//todo check file exists
	lastFileInTree = fileId;
}