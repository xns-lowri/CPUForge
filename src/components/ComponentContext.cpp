#include "ComponentContext.h"

ComponentContext::ComponentContext() : 
	treeActionsByFolder(std::unordered_map<FolderKind, std::vector<TreeActionDescriptor>>()),
	treeActionsByFile(std::unordered_map<FileType, std::vector<TreeActionDescriptor>>())
{
	isaDocuments = IsaDocumentStore();
}

void ComponentContext::SetTreeActionsFolder(
	FolderKind folderKind, std::vector<TreeActionDescriptor> actions)
{
	treeActionsByFolder[folderKind] = std::move(actions);
}

void ComponentContext::SetTreeActionsFile(
	FileType folderKind, std::vector<TreeActionDescriptor> actions)
{
	treeActionsByFile[folderKind] = std::move(actions);
}

IsaDocumentStore& ComponentContext::GetIsaDocuments() { 
	return isaDocuments; 
}
IsaDefinition& ComponentContext::GetIsaDocument(const UUID& id) {
	return isaDocuments.openDocuments.at(id);
}

std::vector<TreeActionDescriptor> ComponentContext::GetTreeActionsFolder(
	FolderKind folderKind) const 
{
	if (treeActionsByFolder.find(folderKind) == treeActionsByFolder.end()) {
		return std::vector<TreeActionDescriptor>();
	}
	return treeActionsByFolder.at(folderKind);
}
std::vector<TreeActionDescriptor> ComponentContext::GetTreeActionsFile(
	FileType fileType) const 
{
	if (treeActionsByFile.find(fileType) == treeActionsByFile.end()) {
		return std::vector<TreeActionDescriptor>();
	}
	return treeActionsByFile.at(fileType);
}