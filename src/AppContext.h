#pragma once
#include "queue/AppCommandQueue.h"
#include "project/ProjectManager.h"
#include "project/WorkspaceManager.h"
#include "core/AppComponentRegistry.h"

//class AppCommandQueue;
//class ProjectManager;
//class WorkspaceManager;

struct AppContext {
	//constructor
	//AppContext(AppCommandQueue queue, ProjectManager proj, WorkspaceManager ws)
	//	: commandQueue(queue), projectManager(proj), workspaceManager(ws) {}
	////member variables
	//AppCommandQueue& commandQueue;	//command queue in context
	//
    //ProjectManager& projectManager; //blank proj
	//WorkspaceManager& workspaceManager; //blank workspace

	std::unique_ptr<AppCommandQueue> commandQueue =
		std::make_unique<AppCommandQueue>();

	std::unique_ptr<ProjectManager> projectManager =
		std::make_unique<ProjectManager>();

	std::unique_ptr<WorkspaceManager> workspaceManager =
		std::make_unique<WorkspaceManager>();

	std::unique_ptr<AppComponentRegistry> appComponentRegistry =
		std::make_unique<AppComponentRegistry>();

	bool requestQuit = false;
};