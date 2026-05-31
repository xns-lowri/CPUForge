#pragma once

class AppComponentRegistry {
public:
	AppComponentRegistry() = default;
	~AppComponentRegistry() = default;

	//connect to handlers for file explorer tree
	void RegisterFileType(FileTypeDescriptor descriptor);	
private:
	//
};