#pragma once
#include <string>
#include <vector>
#include "../core/AppComponentRegistry.h"
#include "../project/Project.h"

class IAppComponent {
public:
	virtual ~IAppComponent() = default; //default destructor

	//interface methods
	virtual const std::string& GetID() const = 0;
	//virtual void Register(AppComponentRegistry& registry) = 0;

	virtual std::vector<FileTypeDescriptor> GetSupportedFileTypes() const = 0;

	virtual std::vector<TreeActionDescriptor> GetTreeActionsFolder() const = 0;
	virtual std::vector<TreeActionDescriptor> GetTreeActionsFile() const = 0;

	virtual bool HandleCommand(
		const std::string& command,
		const std::string& path) = 0;

	//virtual bool NewFile(std::string filePath) = 0;
	//virtual bool OpenFile(std::string filePath) = 0;
	
};

class ComponentBase : public IAppComponent
{
public:
	ComponentBase(std::string id) 
		: id(std::move(id)) {}

	const std::string& GetID() const override 
	{ 
		return id; 
	}
	//todo
protected:
	//todo
	const std::string id;
};