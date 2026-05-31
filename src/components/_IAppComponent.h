#pragma once
#include <string>
#include <vector>
#include "../core/AppComponentRegistry.h"

class IComponent {
public:
	virtual ~IComponent() = default; //default destructor

	//interface methods
	virtual std::string GetID();
	virtual void Register(AppComponentRegistry& registry);
	
};

class ComponentBase : IComponent
{
public:
	ComponentBase(std::string id) : id(std::move(id)) {}
	std::string GetID() { return id; }
	//todo
protected:
	//todo
	const std::string id;
};