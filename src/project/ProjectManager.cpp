#include "ProjectManager.h"


/* Constructors */
ProjectManager::ProjectManager() : 
	projectData(std::nullopt)
{
	fmt::println("ProjectManager initialized with no active project.");
}

ProjectManager::ProjectManager(const ProjectData& data) :
	projectData(data)
{
	fmt::println("ProjectManager initialized with project: {}", 
		data.name);
}

/* Project data getter/setter definitions */
/* Check if project data is active */
bool ProjectManager::HasActiveProject() const
{
	return projectData.has_value();
}

/* Project data getters - read only and w/ write access*/
std::optional<ProjectData> ProjectManager::GetCurrentProject()
{
	return projectData;
}

const std::optional<ProjectData> ProjectManager::GetCurrentProject() const
{
	return projectData;
}

void ProjectManager::CloseProject() {
	projectData = std::nullopt;
	SaveProjectContext(); //update project context
}

/* Dirty getters/setters */
bool ProjectManager::IsDirty() const
{
	if (projectData.has_value())
		return projectData->isDirty;
	else
		return false;
}

void ProjectManager::MarkProjectDirty()
{
	if (projectData.has_value())
		projectData->isDirty = true;
}

const UUID ProjectManager::GetNextUUID()
{
	return projectData->nextId++;
}

/* Project data/state management */
/* Create new project with structure */
bool ProjectManager::NewProject(const std::string& name, const std::filesystem::path& path) {
	//todo properly close last project if open, including save dialog if dirty

	if (path.empty() || !path.is_absolute()) {
		//invalid path
		fmt::println("Invalid path: '{}'", path.string());
		return false; //early return
	}

	std::filesystem::path projectPath = path / name;

	fmt::println("Creating new project '{}' at path '{}'", name, projectPath.string());

	//fmt::println("Stem: '{}'", fsPath.stem().string());

	projectData = ProjectData();

	time_t now = std::time(nullptr);
	projectData->nextId = static_cast<UUID>(now); //unique id base for new project
	projectData->id = GetNextUUID();
	projectData->name = name;

	projectData->path = projectPath;
	//return false if path is invalid or read-only

	//todo default folder structure
	NewFolder("isa", 0);
	NewFolder("hardware", 0);
	UUID sources = NewFolder("sources", 0);
	NewFolder("host", sources);
	NewFolder("target", sources);
	NewFolder("debug", 0);
	NewFolder("build", 0);
	NewFolder("notes", 0);

	//save project file
	SaveProject();
	fmt::println("Created project '{}' with id {}", name, projectData->id);
	return true;
}

/* Save project.json file in current working directory*/
bool ProjectManager::SaveProject() {
	//todo save dirty files first?
	SaveProjectContext(); //update project context

	json fileData = SerialiseProject();
	std::filesystem::path filePath = projectData->path / PROJECT_FILENAME;
	ProjectResult result = FileHandler::Save(filePath, fileData);
	return result.ok;
}

std::filesystem::path ProjectManager::GetProjectContextPath() {
	std::filesystem::path path = std::filesystem::current_path() / "project-context.json";
	return path;
}

bool ProjectManager::SaveProjectContext() {
	json contextData = {};
	if (projectData.has_value()) {
		contextData.emplace("lastOpenProject", projectData->path);
	}
	else {
		contextData.emplace("lastOpenProject", NULL);
	}
	ProjectResult result = FileHandler::Save(GetProjectContextPath(), contextData);
	fmt::println("Saved project context: {}", result.ok);
	return result.ok;
}

std::optional<std::filesystem::path> ProjectManager::LoadProjectContext() {
	auto result = FileHandler::Load<json>(GetProjectContextPath());
	fmt::println("Opened project context: {}", result.ok);
	if (!result.ok) {
		return std::nullopt;
	}
	
	std::optional<std::filesystem::path> retval = std::nullopt;
	if (result.value.at("lastOpenProject") != 0) {
		retval = result.value.at("lastOpenProject");
	}
	return retval;
}

/* Get json object representing project metadata */
json ProjectManager::SerialiseProject() {
	json fileOut = {};
	fileOut.emplace("id", projectData->id);
	fileOut.emplace("name", projectData->name);
	fileOut.emplace("description", projectData->description);
	fileOut.emplace("path", projectData->path.string());
	fileOut.emplace("nextId", projectData->nextId);

	json folderList = {};
	for (auto& folder : projectData->folders) {
		json folderData;
		folderData.emplace("id", folder.second.id);
		folderData.emplace("parentId", folder.second.parentId);
		folderData.emplace("name", folder.second.name);
		folderData.emplace("childFolders", folder.second.childFolders);
		folderData.emplace("childFiles", folder.second.childFiles);
		folderList.push_back(folderData);
	}
	fileOut.emplace("folders", folderList);

	json fileList = {};
	for (auto& file : projectData->files) {
		json fileData;
		fileData.emplace("id", file.second.id);
		fileData.emplace("parentId", file.second.parentId);
		fileData.emplace("name", file.second.name);
		fileData.emplace("type", ToString(file.second.type));
		fileData.emplace("documentId", file.second.documentId);
		fileData.emplace("generated", ToString(file.second.generated));
		fileData.emplace("readOnly", ToString(file.second.readOnly));
		fileList.push_back(fileData);
	}
	fileOut.emplace("files", fileList);

	return fileOut;
}

bool ProjectManager::DeserialiseProject(json fileData) {
	projectData = ProjectData(); //clear

	projectData->id = fileData.at("id");
	projectData->name = fileData.at("name");
	projectData->description = fileData.at("description");
	std::filesystem::path projPath = fileData.at("path");
	projectData->path = projPath;
	projectData->nextId = fileData.at("nextId");

	//auto fileList = fileData.at("files").get<std::vector<json>>();
	if (!fileData.at("files").is_null()) {
		auto fileList = fileData.at("files").get<std::vector<json>>();
		for (auto& file : fileList) {
			//fmt::println("Got file: {}", file.at("name").get<std::string>());
			FileObject nextFile = FileObject();
			nextFile.id = file.at("id");
			nextFile.parentId = file.at("parentId");
			nextFile.name = file.at("name");
			nextFile.type = FromString(file.at("type"));
			nextFile.documentId = file.at("documentId");
			nextFile.generated = file.at("generated").get<bool>();
			nextFile.readOnly = file.at("readOnly").get<bool>();
			projectData->files.emplace(nextFile.id, nextFile);
		}
	}

	if (!fileData.at("folders").is_null()) {
		auto folderList = fileData.at("folders").get<std::vector<json>>();
		for (auto& folder : folderList) {
			//fmt::println("Got folder: {}", folder.at("name").get<std::string>());
			FolderObject nextFolder = FolderObject();
			nextFolder.id = folder.at("id");
			nextFolder.parentId = folder.at("parentId");
			nextFolder.name = folder.at("name");

			nextFolder.childFolders = folder.at("childFolders").get<std::vector<UUID>>();
			nextFolder.childFiles = folder.at("childFiles").get<std::vector<UUID>>();
			//if (folder.at("childFolders").is_array()) {}
			//if (folder.at("childFiles").is_array()) {}
			projectData->folders.emplace(nextFolder.id, nextFolder);
		}
	}

	return true;
}

bool ProjectManager::OpenProject(const std::filesystem::path& rootPath) {
	//load project data
	std::filesystem::path filePath = rootPath;
	if (rootPath.has_extension()) {
		//strip filename if given
		filePath = rootPath.parent_path();
	}

	fmt::println("Loading project at {}", filePath.string());
	auto result = FileHandler::Load<json>(filePath / PROJECT_FILENAME);
	if (!result.ok) {
		//error loading file, todo handle
		fmt::println("Error loading project file");
		return false;
	}
	fmt::println("Start deserialising project");
	bool deserial = DeserialiseProject(result.value);
	//update path if necessary
	//update last project opened metadata
	SaveProjectContext(); //update project context
	return true;
}

/* Project tree management functions */
UUID ProjectManager::NewFolder(const std::string& name, UUID parentId)
{
	if (!projectData.has_value()) {
		fmt::println("No active project to add folder to.");
		return 0;
	}
	//todo check parentId exists and is folder
	if (parentId != 0 && projectData->folders.find(parentId) == projectData->folders.end()) {
		fmt::println("Parent folder '{}' with id {} does not exist.", name, parentId);
		return 0;
	}

	//create new folder object
	FolderObject newFolder;
	newFolder.id = GetNextUUID();
	newFolder.parentId = parentId;
	newFolder.name = name;


	//push new folder to parent if not in root
	if (parentId != 0) {
		//find parent folder, checked earlier so should exist
		FolderObject* parentFolder = &projectData->folders.find(parentId)->second;
		parentFolder->childFolders.push_back(newFolder.id);

		fmt::println("Added folder '{}' with id {} to parent {} in project '{}'",
			name, newFolder.id, parentFolder->name, projectData->name);

		fmt::println("Child folders in parent '{}': {}", parentFolder->name, parentFolder->childFolders.size());
	}
	//else {
	//	//push new folder to project tree
	//	projectData->folders.emplace(newFolder.id, newFolder);
	//}

	//always push new folder to project tree
	projectData->folders.emplace(newFolder.id, newFolder);

	//build folder path from parent folders
	UUID curFolder = newFolder.parentId;
	std::filesystem::path localPath = newFolder.name;
	while (curFolder != 0) {
		//add path and go to next folder
		FolderObject& curFolderObj = projectData->folders.find(curFolder)->second;
		localPath = curFolderObj.name / localPath;
		curFolder = curFolderObj.parentId;
	}
	std::filesystem::path folderPath = projectData->path / localPath;

	//todo make new folder
	FileHandler::CreateFolder(folderPath);
	SaveProject();

	fmt::println("Created folder '{}' with id {} at path '{}'",
		name, newFolder.id, folderPath.string());

	//todo filesystem

	return newFolder.id;
}