#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "enumProjectDef.h"

//header struct definitions
/* Reusable header for all 'document' type objects in project */
struct DocumentHeader {
	UUID id = 0;		//unique document id
	std::string title;
	std::string description;

	uint32_t schemaVersion = 1;	//for future compatibility
	bool dirty = false;		//true if unsaved changes

	std::string createdUtc;
	std::string modifiedUtc;

	//todo uuid pointer to file object? is it needed?

	uint64_t contentRevision = 0;	//incremented on each change, used for version tracking
};

/* Reusable header for all 'item' type objects in project */
/*struct ItemHeader {
	UUID id = 0;
	std::string name;
	std::string description;
};*/

struct FileTypeDescriptor {
	std::string id;	//path-style id
	std::string displayName;
	std::vector<std::string> extensions;
	//todo icon
	std::string defaultOpenCommand;
};

/* Definitions for file objects (pointers) in project tree */
struct FileObject {
	UUID id = 0;			//unique id
	UUID parentId = 0;

	std::string name;
	std::string path;	//path within project
	FileType type = FileType::Unknown;

	UUID documentId = 0;	//pointer to doc represented by file

	bool generated = false;		//false if user created
	bool readOnly = false;
};

//folder tree struct definitions
struct FolderProperties {
	bool canAddFolders = true;
	bool canAddFiles = true;
	bool canRename = true;
};

/* Definitions for folder tree mgmt */
struct FolderObject
{
	UUID id = 0;		//unique folder id
	UUID parentId = 0;	//parent folder id, 0 if root

	std::string name;	//folder name in os
	std::string path;	//path within project

	FolderProperties properties;
	FolderType type = FolderType::General;

	//objects in folder
	std::vector<UUID> childFolders;
	std::vector<UUID> childFiles;
};