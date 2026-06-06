#include "WindowManager.h"

#include "core/ModalNewProject.h"
#include "core/ModalOpenProject.h"
#include "core/ModalNewFolder.h"
#include "core/ModalNewFile.h"

#include "modules/ViewProjectTree.h"
#include "modules/ViewIsaEditor.h"

#ifdef __APPLE__
#include "menubar/MacOsNative.h"
#endif

WindowManager::WindowManager(AppContext& context) //todo remove context, not needed
{
	//add available windows here, can also be added during render/runtime
	//ViewProjectTree* viewProjectTree = new ViewProjectTree();
	ViewProjectTree viewProjectTree = ViewProjectTree(*this);
	AddWindow<ViewProjectTree>(viewProjectTree.GetId(), viewProjectTree);

	/* DEBUG todo remove */
	//ViewIsaEditor test1 = ViewIsaEditor(*this);
	//test1.SetOpen(false);
	//AddWindow<ViewIsaEditor>(context, test1.GetId(), test1);

	//add modals
	ModalNewProject modalNewProject = ModalNewProject();
	AddModal<ModalNewProject>(modalNewProject.GetId(), modalNewProject);
	ModalOpenProject modalOpenProject = ModalOpenProject();
	AddModal<ModalOpenProject>(modalOpenProject.GetId(), modalOpenProject);
	ModalNewFolder modalNewFolder = ModalNewFolder();
	AddModal<ModalNewFolder>(modalNewFolder.GetId(), modalNewFolder);
	ModalNewFile modalNewFile = ModalNewFile();
	AddModal<ModalNewFile>(modalNewFile.GetId(), modalNewFile);

	//print all added windows for debugging before leaving
	for (auto& window : windows) {
		fmt::println("Added window: {:s}", window.second->GetTitle());
	}
	for (auto& modal : modals) {
		fmt::println("Added modal: {:s}", modal.second->GetTitle());
	}

#ifdef __APPLE__
	NativeCommandBridge::SetCommandQueue(context.appCommandQueue.get());
#endif

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
T& WindowManager::AddWindow(std::string id, Args&&... args)
{
	// Create a new window of type T with the provided arguments
	auto window = std::make_unique<T>(std::forward<Args>(args)...);
	T& ref = *window; // Get a reference to the newly created window
	windows.emplace(std::move(id), std::move(window)); // Store the window in the vector
	return ref; // Return the reference to the caller
}

bool WindowManager::RemoveWindow(std::string id) 
{
	if (windows.find(id) == windows.end()) {
		fmt::println("[WindowManager] No window to remove '{:s}'", id);
		//no window to remove
		return false;
	}

	fmt::println("[WindowManager] Removing window '{:s}'", id);

	windows.erase(id);
	return true;
}

bool WindowManager::OpenIsaEditor(AppContext& context, UUID documentId) {
	std::string id = "main.isa_editor_" + std::to_string(documentId);

	fmt::println("Opening window '{:s}'", id);

	if (windows.find(id) == windows.end()) {
		//open window
		ViewIsaEditor newEditor = ViewIsaEditor(documentId, *this, id);
		AddWindow<ViewIsaEditor>(newEditor.GetId(), newEditor);
		return true;
	}

	//open window if closed
	if (!windows.at(id)->IsOpen()) {
		windows.at(id)->SetOpen(true);
		return true;
	}

	//window is open
	return false;
}

bool WindowManager::HandleOpenDocument(UUID documentId) {
	//open or add new window for selected document
	//todo remove??
	fmt::println("[WindowManager] Opening document {}", documentId);
	return false;
}

/* Main entrypoint during rendering, draw all windows, modals, etc */
void WindowManager::DrawAll(AppContext& context) {
	//note: do not call RenderMainMenuBar here!! handled by UI.cpp
	DrawWindows(context);
	DrawModals(context);
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
		if (window.second->IsOpen()) {
			window.second->Render(context);
		}
	}
}

void WindowManager::OpenModal(AppContext& context, const std::string& id)
{
	auto modal = modals.find(id);
	if (modal != modals.end()) {
		modal->second->Show();
		fmt::println("Open modal '{:s}'", id);
		fmt::println("showing: {}", modal->second->IsShowing());
	}
	else {
		fmt::println("Modal with id '{:s}' not found!", id);
	}
}

void WindowManager::ToggleWindowVisibility(std::string windowId) {
	auto window = windows.find(windowId);
	if(window != windows.end()) {
		window->second->SetOpen(!window->second->IsOpen());
		fmt::println("Open window '{:s}'", windowId);
	}
}

/*
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
}*/

std::unordered_map<std::string, std::unique_ptr<IWindow>>& WindowManager::GetWindows() {
	return windows;
}