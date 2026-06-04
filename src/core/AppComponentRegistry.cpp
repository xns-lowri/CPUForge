#include "AppComponentRegistry.h"

AppComponentRegistry::AppComponentRegistry() {
	componentsById = std::unordered_map<
		std::string, 
		std::unique_ptr<ComponentBase>
	>();
}

//bool AppComponentRegistry::RegisterComponent(ComponentBase& component) {
//	const std::string& id = component.GetID();
//	if (componentsById.find(id) != componentsById.end()) {
//		fmt::println("Error: Component with ID '{}' is already registered.", id);
//		return false;
//	}
//	componentsById[id] = component;
//	return true;
//};

bool AppComponentRegistry::HandleOpenProjectFile() {
	//todo delegation to modules
	return false;
}

