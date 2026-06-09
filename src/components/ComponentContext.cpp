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

std::vector<TreeActionDescriptor> ComponentContext::GetTreeActionsFolder(
	FolderKind folderKind) const 
{
	return treeActionsByFolder.at(folderKind);
}
std::vector<TreeActionDescriptor> ComponentContext::GetTreeActionsFile(
	FileType fileType) const 
{
	return treeActionsByFile.at(fileType);
}