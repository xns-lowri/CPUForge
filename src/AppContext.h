#pragma once
#include "project/ProjectManager.h"
#include "project/WorkspaceManager.h"

struct AppContext {
	//constructor
	AppContext(ProjectManager prj, WorkspaceManager ws) 
		: projectManager(prj), workspaceManager(ws) {}
	//member variables
    ProjectManager& projectManager; //blank proj
	WorkspaceManager& workspaceManager; //blank workspace
};