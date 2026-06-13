#include "WorkspaceManager.h"

/*
const UUID WorkspaceManager::GetSelectedFolder() const {
	return lastFolderInTree;
}

const UUID WorkspaceManager::GetSelectedFile() const {
	return lastFileInTree;
}

const UUID WorkspaceManager::GetActiveDocument() const {
	return activeDocument;
}

const std::string WorkspaceManager::GetAction() const {
	return lastAction;
}

const std::string WorkspaceManager::GetPath() const {
	return lastPath;
}

void WorkspaceManager::SetSelectedFolder(UUID folderId) {
	//todo check folder exists
	lastFolderInTree = folderId;
}

void WorkspaceManager::SetSelectedFile(UUID fileId) {
	//todo check file exists
	lastFileInTree = fileId;
}

void WorkspaceManager::SetActiveDocument(UUID documentId) {
	//todo check file exists
	activeDocument = documentId;
}

void WorkspaceManager::SetAction(std::string action) {
	lastAction = action;
}

void WorkspaceManager::SetPath(std::string path) {
	lastPath = path;
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