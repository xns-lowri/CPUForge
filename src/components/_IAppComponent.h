#pragma once
#include <string>
#include <vector>
#include "../core/AppComponentRegistry.h"

class IAppComponent {
public:
	virtual ~IAppComponent() = default; //default destructor

	//interface methods
	virtual const std::string& GetID() const = 0;
	//virtual void Register(AppComponentRegistry& registry) = 0;
	virtual bool OpenFile(std::string filePath) = 0;
	
};

class ComponentBase : IAppComponent
{
public:
	ComponentBase(std::string id) : id(std::move(id)) {}
	const std::string& GetID() const { return id; }
	//todo
protected:
	//todo
	const std::string id;
};