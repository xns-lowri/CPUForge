#include <fmt/core.h>
#include <nfd.h>

#include "WindowManager.h"
#include "modules/ViewProjectTree.h"
#include "core/ModalNewProject.h"
#include "core/ModalOpenProject.h"
#include "core/ModalNewFolder.h"

WindowManager::WindowManager()
{
	//add available windows here, can also be added elsewhere
	//ViewProjectTree* viewProjectTree = new ViewProjectTree();
	AddWindow<ViewProjectTree>(ViewProjectTree(*this));

	//add modals
	ModalNewProject modalNewProject = ModalNewProject();
	AddModal<ModalNewProject>(modalNewProject.GetId(), modalNewProject);
	ModalOpenProject modalOpenProject = ModalOpenProject();
	AddModal<ModalOpenProject>(modalOpenProject.GetId(), modalOpenProject);

	ModalNewFolder modalNewFolder = ModalNewFolder();
	AddModal<ModalNewFolder>(modalNewFolder.GetId(), modalNewFolder);

	//print all added windows for debugging before leaving
	for (auto& window : windows) {
		fmt::println("Added window: {:s}", window->GetTitle());
	}
	for (auto& modal : modals) {
		fmt::println("Added modal: {:s}", modal.second->GetTitle());
	}
	fmt::println("WindowManager initialized");
}

template <typename T, typename... Args>
T& WindowManager::AddModal(std::string id, Args&&... args)
{
	// Create a new window of type T with the provided arguments
	auto modal = std::make_unique<T>(std::forward<Args>(args)...);
	T& ref = *modal; // Get a reference to the newly created window
	modals.emplace(std::move(id), std::move(modal)); 
		// Store the modal in the map
	return ref; // Return the reference to the caller
}

template <typename T, typename... Args>
T& WindowManager::AddWindow(Args&&... args)
{
	// Create a new window of type T with the provided arguments
	auto window = std::make_unique<T>(std::forward<Args>(args)...);
	T& ref = *window; // Get a reference to the newly created window
	windows.push_back(std::move(window)); // Store the window in the vector
	return ref; // Return the reference to the caller
}

/* Main entrypoint during rendering, draw all windows, modals, etc */
void WindowManager::DrawAll(AppContext& context) {
	DrawWindows(context);
	DrawModals(context);
	//todo draw modals
}

void WindowManager::DrawModals(AppContext& context)
{
	// Draw all modals here
	for (auto& modal : modals) {
		if (modal.second->IsShowing()) {
			modal.second->Render(context);
		}
	}
}

void WindowManager::DrawWindows(AppContext& context)
{
	// Draw all windows here
	// For example:
	// viewProjectTree.Draw();
	for (auto& window : windows) {
		if (window->IsOpen()) {
			window->Render(context);
		}
	}
}

void WindowManager::OpenModal(const std::string& id)
{
	auto it = modals.find(id);
	if (it != modals.end()) {
		it->second->Show();
		fmt::println("Open modal '{:s}'", id);
		fmt::println("showing: {}", it->second->IsShowing());
	}
	else {
		fmt::println("Modal with id '{:s}' not found!", id);
	}
}

void WindowManager::DrawMenuBarItems()
{
	for (auto& window : windows) {
		window->DrawMenuBarItem();
	}
}

void WindowManager::DrawWindowMenuItems()
{
	for (auto& window : windows) {
		window->DrawWindowMenuItem();
	}
}