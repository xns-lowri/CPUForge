#pragma once
#include <fmt/core.h>
#include <string>
#include <unordered_map>
#include <memory>
#include "../components/_IAppComponent.h"

class AppComponentRegistry {
public:
	//AppComponentRegistry() = default;
	//~AppComponentRegistry() = default;

	AppComponentRegistry();

	//bool RegisterComponent(ComponentBase& component);

	bool HandleOpenProjectFile();

	//connect to handlers for file explorer tree
	//void RegisterFileType(FileTypeDescriptor descriptor);	
private:
	//
	std::unordered_map<std::string, std::unique_ptr<ComponentBase>> componentsById;
};