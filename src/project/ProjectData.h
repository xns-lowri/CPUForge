#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include <fmt/core.h>

#include "enumProjectDef.h"
#include "Project.h"

#include "../core/isa/IsaArchitecture.h"

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
	//std::map<UUID, IsaDefinition> isaDefinitions;
	//todo other document types 
	// e.g. hardware sheets, source files, build manifests,
	// simulation states, tool scripts, etc
};

inline void to_json(json& j, const ProjectData& data) {
	j = json{
		{ "id", data.id },
		{ "name", data.name },
		{ "description", data.description },
		{ "path", data.path },
		{ "nextId", data.nextId },
		{ "folders", data.folders },
		{ "files", data.files }
	};
}

inline void from_json(const json& j, ProjectData& data) {
	j.at("id").get_to(data.id);
	j.at("name").get_to(data.name);
	j.at("description").get_to(data.description);
	j.at("path").get_to(data.path);
	j.at("nextId").get_to(data.nextId);
	j.at("folders").get_to(data.folders);
	j.at("files").get_to(data.files);
}