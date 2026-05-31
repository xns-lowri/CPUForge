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

enum class FolderType {
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

inline FolderType FolderTypeFromString(std::string value) {
	if (value == "ISA")			return FolderType::ISA;
	if (value == "Hardware")	return FolderType::Hardware;
	if (value == "Source")      return FolderType::Source;
	if (value == "Debug")		return FolderType::Debug;
	if (value == "Build")		return FolderType::Build;
	if (value == "Notes")		return FolderType::Notes;

	return FolderType::General;
}
inline std::string ToString(FolderType type) {
	switch (type)
	{
	case FolderType::ISA:		return "ISA";
	case FolderType::Hardware:  return "Hardware";
	case FolderType::Source:    return "Source";
	case FolderType::Debug:     return "Debug";
	case FolderType::Build:		return "Build";
	case FolderType::Notes:		return "Notes";

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