#pragma once
#include "queue/AppCommandQueue.h"
#include "project/ProjectManager.h"
#include "project/WorkspaceManager.h"

//class AppCommandQueue;
//class ProjectManager;
//class WorkspaceManager;

struct AppContext {
	//constructor
	AppContext(AppCommandQueue queue, ProjectManager proj, WorkspaceManager ws) 
		: commandQueue(queue), projectManager(proj), workspaceManager(ws) {}
	//member variables
	AppCommandQueue& commandQueue;	//command queue in context

    ProjectManager& projectManager; //blank proj
	WorkspaceManager& workspaceManager; //blank workspace

	bool requestQuit = false;
};