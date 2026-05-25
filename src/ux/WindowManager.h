#pragma once
#include "_UxInterface.h"
#include "../AppContext.h"

class WindowManager
{
public:
	WindowManager();
	template <typename T, typename... Args>
	T& AddModal(std::string id, Args&&... args);
	template <typename T, typename... Args>
	T& AddWindow(Args&&... args);

	void DrawAll(AppContext& context);
	void DrawModals(AppContext& context);
	void DrawWindows(AppContext& context);

	void OpenModal(const std::string& id);

	void DrawMenuBarItems();
	void DrawWindowMenuItems();
private:
	std::unordered_map<std::string, std::unique_ptr<IModal>> modals;
	std::vector<std::unique_ptr<IWindow>> windows;
};