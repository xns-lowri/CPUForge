#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "enumProjectDef.h"

//header struct definitions
/* Reusable header for all 'document' type objects in project */
struct DocumentHeader {
	UUID id = 0;		//unique id
	std::string name;
	std::string description;

	uint32_t schemaVersion = 1;	//for future compatibility
	bool dirty = false;		//true if unsaved changes

	std::string createdUtc;
	std::string modifiedUtc;

	uint64_t contentRevision = 0;	//incremented on each change, used for version tracking
};

/* Reusable header for all 'item' type objects in project */
/*struct ItemHeader {
	UUID id = 0;
	std::string name;
	std::string description;
};*/

/* Definitions for file objects (pointers) in project tree */
struct FileObject {
	UUID id = 0;			//unique id
	UUID parentId = 0;

	std::string name;
	ProjectFileType type = ProjectFileType::Unknown;

	UUID documentId = 0;	//pointer to doc represented by file

	bool generated = false;		//false if user created
	bool readOnly = false;
};

//folder tree struct definitions
/* Definitions for folder tree mgmt */
struct FolderObject
{
	UUID id = 0;		//unique folder id
	UUID parentId = 0;	//parent folder id, 0 if root

	std::string name;	//folder name in os

	//objects in folder
	std::vector<UUID> childFolders;
	std::vector<UUID> childFiles;
};

#include "../core/Architecture.h"

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
