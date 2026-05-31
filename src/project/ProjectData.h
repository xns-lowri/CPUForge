#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "enumProjectDef.h"
#include "Project.h"

#include "../core/isa/Architecture.h"

/* Main project struct */
struct ProjectData
{
	UUID id = 0;		//unique project id
	std::string name;
	std::string description;

	bool isDirty = false;		//true if unsaved changes anywhere in project

	//todo project path, store locally though not here?
	std::filesystem::path path;
	UUID nextId = 0;	//next unique id to use in project

	//project tree
	std::map<UUID, FolderObject> folders;
	std::map<UUID, FileObject> files;

	//typed document storage containers
	std::map<UUID, IsaDefinitionDocument> isaDefinitions;
	//todo other document types 
	// e.g. hardware sheets, source files, build manifests,
	// simulation states, tool scripts, etc
};
