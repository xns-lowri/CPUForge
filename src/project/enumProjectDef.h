#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../_types.h"

const std::string PROJECT_FILENAME = "project.cpuforge.json";

//enum class ProjectItemType
//{
//	Folder,
//	File
//};

enum class FolderKind {
	ISA,
	Hardware,
	Source,
	Debug,
	Build,
	Notes,

	General
};

enum class FileType
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

inline FileType FileTypeFromFolderKind(FolderKind folderKind) {
	switch (folderKind)
	{
	case FolderKind::ISA:		return FileType::ISADefinition;
	case FolderKind::Hardware:  return FileType::HardwareSheet;
	case FolderKind::Source:    return FileType::SourceFile;
	//case FolderKind::Debug:     return FileType::SimulationState; //todo separate debug file type?
	//case FolderKind::Build:		return FileType::BuildManifest; //todo separate build file type?
	case FolderKind::Notes:		return FileType::TextDocument;
	default:                    return FileType::Unknown;
	}
}

inline std::string FixedExtensionFromFileType(FileType fileType) {
	switch (fileType)
	{
	case FileType::ISADefinition:   return ".isa";
	case FileType::HardwareSheet:   return ".shw";
	default:                               return "";
	}
}

inline FolderKind FolderTypeFromString(std::string value) {
	if (value == "ISA")			return FolderKind::ISA;
	if (value == "Hardware")	return FolderKind::Hardware;
	if (value == "Source")      return FolderKind::Source;
	if (value == "Debug")		return FolderKind::Debug;
	if (value == "Build")		return FolderKind::Build;
	if (value == "Notes")		return FolderKind::Notes;

	return FolderKind::General;
}
inline std::string ToString(FolderKind type) {
	switch (type)
	{
	case FolderKind::ISA:		return "ISA";
	case FolderKind::Hardware:  return "Hardware";
	case FolderKind::Source:    return "Source";
	case FolderKind::Debug:     return "Debug";
	case FolderKind::Build:		return "Build";
	case FolderKind::Notes:		return "Notes";

	default:                    return "General";
	}
}

inline FileType FileTypeFromString(std::string value) {
	if (value == "ISADefinition")   return FileType::ISADefinition;
	if (value == "HardwareSheet")   return FileType::HardwareSheet;
	if (value == "SourceFile")      return FileType::SourceFile;
	if (value == "BinaryFile")      return FileType::BinaryFile;
	if (value == "BuildManifest")   return FileType::BuildManifest;
	if (value == "SimulationState") return FileType::SimulationState;
	if (value == "ToolScript")      return FileType::ToolScript;
	if (value == "TextDocument")    return FileType::TextDocument;

	return FileType::Unknown;
}
inline std::string ToString(FileType type) {
	switch (type)
	{
	case FileType::ISADefinition:   return "ISADefinition";
	case FileType::HardwareSheet:   return "HardwareSheet";
	case FileType::SourceFile:      return "SourceFile";
	case FileType::BinaryFile:      return "BinaryFile";
	case FileType::BuildManifest:   return "BuildManifest";
	case FileType::SimulationState: return "SimulationState";
	case FileType::ToolScript:      return "ToolScript";
	case FileType::TextDocument:    return "TextDocument";
	default:                               return "Unknown";
	}
}
inline std::string ToString(bool b) {
	return b ? "true" : "false";
}