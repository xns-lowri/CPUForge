#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

//#include "ProjectTypes.h"

const std::string PROJECT_FILENAME = "project.cpuforge.json";
using UUID = uint64_t;

enum class ProjectItemType
{
	Folder,
	File
};

enum class ProjectFileType
{
	//design files
	ISADefinition,
	HardwareSheet,
	SourceFile,
	//output files
	BinaryFile,		//compiles software file
	BuildManifest,	//hardware build output w/ version control
	//simulator states and outputs??
	SimulationState,
	//other file types
	ToolScript,			//tool file	
	TextDocument,		//other text e.g. documentation
	Unknown			//unknown file type - error catching
};
inline ProjectFileType FromString(std::string value) {
	if (value == "ISADefinition")   return ProjectFileType::ISADefinition;
	if (value == "HardwareSheet")   return ProjectFileType::HardwareSheet;
	if (value == "SourceFile")      return ProjectFileType::SourceFile;
	if (value == "BinaryFile")      return ProjectFileType::BinaryFile;
	if (value == "BuildManifest")   return ProjectFileType::BuildManifest;
	if (value == "SimulationState") return ProjectFileType::SimulationState;
	if (value == "ToolScript")      return ProjectFileType::ToolScript;
	if (value == "TextDocument")    return ProjectFileType::TextDocument;

	return ProjectFileType::Unknown;
}
inline std::string ToString(ProjectFileType type) {
	switch (type)
	{
	case ProjectFileType::ISADefinition:   return "ISADefinition";
	case ProjectFileType::HardwareSheet:   return "HardwareSheet";
	case ProjectFileType::SourceFile:      return "SourceFile";
	case ProjectFileType::BinaryFile:      return "BinaryFile";
	case ProjectFileType::BuildManifest:   return "BuildManifest";
	case ProjectFileType::SimulationState: return "SimulationState";
	case ProjectFileType::ToolScript:      return "ToolScript";
	case ProjectFileType::TextDocument:    return "TextDocument";
	default:                               return "Unknown";
	}
}
inline std::string ToString(bool b) {
	return b ? "true" : "false";
}

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
	std::unordered_map<UUID, FolderObject> folders;
	std::unordered_map<UUID, FileObject> files;

	//typed document storage containers
	std::unordered_map<UUID, ISADefinitionDocument> isaDefinitions;
	//todo other document types 
	// e.g. hardware sheets, source files, build manifests,
	// simulation states, tool scripts, etc
};
