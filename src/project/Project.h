#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include <fmt/core.h>

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

inline void to_json(json& j, const TreeActionDescriptor& treeAction) {
	j = json{
		{ "action", treeAction.action },
		{ "displayName", treeAction.displayName }
	};
}

inline void from_json(const json& j, TreeActionDescriptor& treeAction) {
	j.at("action").get_to(treeAction.action);
	j.at("displayName").get_to(treeAction.displayName);
}

struct FileTypeDescriptor {
	std::string id;	//path-style id
	std::string displayName;
	std::string extension;
	FileType type;
	//todo icon
	//std::string defaultOpenCommand;
};

inline void to_json(json& j, const FileTypeDescriptor& fileType) {
	j = json{
		{ "id", fileType.id },
		{ "displayName", fileType.displayName },
		{ "extension", fileType.extension },
		{ "type", ToString(fileType.type) }
	};
}

inline void from_json(const json& j, FileTypeDescriptor& fileType) {
	j.at("id").get_to(fileType.id);
	j.at("displayName").get_to(fileType.displayName);
	j.at("extension").get_to(fileType.extension);
	j.at("type").get_to<FileType>(fileType.type);
}

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

inline void to_json(json& j, const FileObject& fileObject) {
	j = json{
		{ "id", fileObject.id },
		{ "parentId", fileObject.parentId },
		{ "name", fileObject.name },
		{ "extension", fileObject.extension },
		{ "path", fileObject.path },
		{ "type", ToString(fileObject.type) },
		{ "documentId", fileObject.documentId },
		{ "generated", fileObject.generated },
		{ "readOnly", fileObject.readOnly }
	};
}

inline void from_json(const json& j, FileObject& fileObject) {
	j.at("id").get_to(fileObject.id);
	j.at("parentId").get_to(fileObject.parentId);
	j.at("name").get_to(fileObject.name);
	j.at("extension").get_to(fileObject.extension);
	j.at("path").get_to(fileObject.path);
	j.at("type").get_to<FileType>(fileObject.type);
	j.at("documentId").get_to(fileObject.documentId);
	j.at("generated").get_to(fileObject.generated);
	j.at("readOnly").get_to(fileObject.readOnly);
}

//folder tree struct definitions
struct FolderProperties {
	bool canAddFolders = true;
	bool canAddFiles = true;
	bool canRename = true;
};

inline void to_json(json& j, const FolderProperties& props) {
	j = json{
		{ "canAddFolders", props.canAddFolders },
		{ "canAddFiles", props.canAddFiles },
		{ "canRename", props.canRename }
	};
}

inline void from_json(const json& j, FolderProperties& props) {
	j.at("canAddFolders").get_to(props.canAddFolders);
	j.at("canAddFiles").get_to(props.canAddFiles);
	j.at("canRename").get_to(props.canRename);
}

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

inline void to_json(json& j, const FolderObject& folder) {
	j = json{
		{ "id", folder.id },
		{ "parentId", folder.parentId },
		{ "name", folder.name },
		{ "path", folder.path },
		{ "properties", folder.properties },
		{ "type", ToString(folder.type) },
		{ "childFolders", folder.childFolders },
		{ "childFiles", folder.childFiles }
	};
}

inline void from_json(const json& j, FolderObject& folder) {
	j.at("id").get_to(folder.id);
	j.at("parentId").get_to(folder.parentId);
	j.at("name").get_to(folder.name);
	j.at("path").get_to(folder.path);
	j.at("properties").get_to(folder.properties);
	j.at("type").get_to<FolderKind>(folder.type);
	j.at("childFolders").get_to(folder.childFolders);
	j.at("childFiles").get_to(folder.childFiles);
}