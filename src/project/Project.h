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

inline void to_json(json& j, const DocumentHeader& header) {
	j = json{
		{ "id", header.id },
		{ "title", header.title },
		{ "description", header.description },
		{ "schemaVersion", header.schemaVersion },
		//{ "dirty", header.dirty },
		{ "createdUtc", header.createdUtc },
		{ "modifiedUtc", header.modifiedUtc },
		{ "contentRevision", header.contentRevision }
	};
}

inline void from_json(const json& j, DocumentHeader& header) {
	j.at("id").get_to(header.id);
	j.at("title").get_to(header.title);
	j.at("description").get_to(header.description);
	j.at("schemaVersion").get_to(header.schemaVersion);
	j.at("createdUtc").get_to(header.createdUtc);
	j.at("modifiedUtc").get_to(header.modifiedUtc);
	j.at("contentRevision").get_to(header.contentRevision);
}
/* Reusable header for all 'item' type objects in project */
/*struct ItemHeader {
	UUID id = 0;
	std::string name;
	std::string description;
};*/

struct TreeActionDescriptor {
	std::string action;	//path-style command id
	std::string displayName;
};

struct FileTypeDescriptor {
	std::string id;	//path-style id
	std::string displayName;
	std::string extension;
	FileType type;
	//todo icon
	//std::string defaultOpenCommand;
};

/* Definitions for file objects (pointers) in project tree */
struct FileObject {
	UUID id = 0;			//unique id
	UUID parentId = 0;

	std::string name;
	std::string extension;

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
	FolderKind type = FolderKind::General;

	//objects in folder
	std::vector<UUID> childFolders;
	std::vector<UUID> childFiles;
};