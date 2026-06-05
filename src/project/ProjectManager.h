#pragma once
#include <string>
#include <optional>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <unordered_map>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include "FileHandler.h"

#include "ProjectData.h" //includes architecture, instruction

class ProjectManager
{
protected:
	std::optional<ProjectData> projectData;
	//std::string currentWorkingDirectory;

public:
	//constructors
	ProjectManager();
	//ProjectManager(const ProjectData& data, const std::string& dir);
	ProjectManager(const ProjectData& data); // : projectData(data) {}

	//project data getters/setters
	std::optional<ProjectData> GetCurrentProject();	//write access for other workflow components
	const std::optional<ProjectData> GetCurrentProject() const;	//read only access e.g. UI
	bool HasActiveProject() const;
	bool IsDirty() const;
	void MarkProjectDirty();

	const std::string GetProjectName() const;

	const UUID GetNextUUID();

	//project state management
	bool NewProject(
		const std::string name, 
		const std::filesystem::path path);

	json SerialiseProject();
	bool DeserialiseProject(json fileData);

	bool SaveProjectContext();
	std::optional<std::filesystem::path> LoadProjectContext();
	std::filesystem::path GetProjectContextPath();

	bool OpenProject(const std::filesystem::path filePath);
	bool SaveProject();
	bool SaveProjectAs(const std::filesystem::path filePath);
	void CloseProject();

	//project tree management functions
	UUID NewFolder(
		const std::string name, 
		UUID parentId,
		std::optional<FolderProperties> props,
		std::optional<FolderKind> type);

	UUID NewFile(
		const std::string name, 
		const std::string extension, 
		FileType type, 
		UUID parentId);
	//bool deleteItem(UUID itemId);
	//bool renameItem(UUID itemId, const std::string& newName);
	bool SetDocumentIdInFile(UUID fileId, UUID documentId);

	//document management functions
	//bool createISADefinitionDocument(const std::string& name, const std::string& description);
};
