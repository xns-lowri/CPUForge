
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../_types.h"

const std::string PROJECT_FILENAME = "project.cpuforge.json";

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