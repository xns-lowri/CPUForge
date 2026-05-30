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

/*
void WorkspaceManager::ToggleWindowVisibility(std::string windowId) {
	auto window = windows.find(windowId);
	if(window != windows.end()) {
		window->second->SetOpen(true);
	}
}
std::unordered_map<std::string, std::unique_ptr<IModal>>& WorkspaceManager::GetModalMap() {
	return modals;
}
std::vector<std::unique_ptr<IWindow>>& WorkspaceManager::GetWindowMap() {
	return windows;
}*/