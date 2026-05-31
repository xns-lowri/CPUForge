#pragma once
#include <fmt/core.h>
#include <nfd.h>
#include "_UxInterface.h"
#include "../AppContext.h"

#ifdef __APPLE__
#include "menubar/MacOsNative.h"
#endif

class WindowManager
{
public:
	WindowManager(AppContext& context);
	template <typename T, typename... Args>
	T& AddModal(AppContext& context, std::string id, Args&&... args);
	template <typename T, typename... Args>
	T& AddWindow(AppContext& context, std::string id, Args&&... args);

	//bool RenderMainMenuBar(AppContext& context);
	void DrawAll(AppContext& context);
	void DrawModals(AppContext& context);
	void DrawWindows(AppContext& context);

	void OpenModal(
		AppContext& context, 
		const std::string& id);

	void OpenWindow(
		AppContext& context, 
		const std::string& id);

	void ToggleWindowVisibility(std::string windowId);

	//void DrawMenuBarItems();
	//void DrawWindowMenuItems();

	std::unordered_map<std::string, std::unique_ptr<IWindow>>& GetWindows();
private:
	std::unordered_map<std::string, std::unique_ptr<IModal>> modals;
	std::unordered_map<std::string, std::unique_ptr<IWindow>> windows;
};