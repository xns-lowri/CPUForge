#pragma once
#include <unordered_map>
#include "../project/enumProjectDef.h"
#include "../project/Project.h"

#include "isa/IsaDocumentStore.h"

class ComponentContext {
public:
	ComponentContext();
	void SetTreeActionsFolder(FolderKind folderKind, std::vector<TreeActionDescriptor> actions);
	void SetTreeActionsFile(FileType folderKind, std::vector<TreeActionDescriptor> actions);
	std::vector<TreeActionDescriptor> GetTreeActionsFolder(FolderKind folderKind) const;
	std::vector<TreeActionDescriptor> GetTreeActionsFile(FileType fileType) const;

	IsaDocumentStore& GetIsaDocuments();
private:
	std::unordered_map<FolderKind, std::vector<TreeActionDescriptor>> treeActionsByFolder;
	std::unordered_map<FileType, std::vector<TreeActionDescriptor>> treeActionsByFile;

	IsaDocumentStore isaDocuments;
};