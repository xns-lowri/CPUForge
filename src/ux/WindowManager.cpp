#include "WindowManager.h"

#include "modules/ViewProjectTree.h"
#include "core/ModalNewProject.h"
#include "core/ModalOpenProject.h"
#include "core/ModalNewFolder.h"

WindowManager::WindowManager(AppContext& context)
{
	//add available windows here, can also be added during render/runtime
	//ViewProjectTree* viewProjectTree = new ViewProjectTree();
	ViewProjectTree viewProjectTree = ViewProjectTree(*this);
	AddWindow<ViewProjectTree>(context, viewProjectTree.GetId(), viewProjectTree);

	//add modals
	ModalNewProject modalNewProject = ModalNewProject();
	AddModal<ModalNewProject>(context, modalNewProject.GetId(), modalNewProject);
	ModalOpenProject modalOpenProject = ModalOpenProject();
	AddModal<ModalOpenProject>(context, modalOpenProject.GetId(), modalOpenProject);
	ModalNewFolder modalNewFolder = ModalNewFolder();
	AddModal<ModalNewFolder>(context, modalNewFolder.GetId(), modalNewFolder);

	//print all added windows for debugging before leaving
	for (auto& window : windows) {
		fmt::println("Added window: {:s}", window.second->GetTitle());
	}
	for (auto& modal : modals) {
		fmt::println("Added modal: {:s}", modal.second->GetTitle());
	}

#ifdef __APPLE__
	NativeCommandBridge::SetCommandQueue(&context.commandQueue);
#endif

	fmt::println("WindowManager initialized");
}

template <typename T, typename... Args>
T& WindowManager::AddModal(AppContext& context, std::string id, Args&&... args)
{
	// Create a new window of type T with the provided arguments
	auto modal = std::make_unique<T>(std::forward<Args>(args)...);
	T& ref = *modal; // Get a reference to the newly created window
	modals.emplace(std::move(id), std::move(modal)); 
		// Store the modal in the map
	return ref; // Return the reference to the caller
}

template <typename T, typename... Args>
T& WindowManager::AddWindow(AppContext& context, std::string id, Args&&... args)
{
	// Create a new window of type T with the provided arguments
	auto window = std::make_unique<T>(std::forward<Args>(args)...);
	T& ref = *window; // Get a reference to the newly created window
	windows.emplace(std::move(id), std::move(window)); // Store the window in the vector
	return ref; // Return the reference to the caller
}

//todo remove
/*
bool WindowManager::RenderMainMenuBar(AppContext& context) {
	bool running = true;
	if (!ImGui::BeginMainMenuBar()) { return running; }

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("New Project")) {
			//context.projectManager.NewProject("New Project");
			//open modal somehow??
			OpenModal(context, "modal.new_project");
		}
		if (ImGui::MenuItem("Open Project", "Ctrl+O")) {
			OpenModal(context, "modal.open_project");
		}
		ImGui::Separator();
		//disable save options if no project loaded
		bool hadProject = context.projectManager.HasActiveProject();
		if (!hadProject)
		{
			ImGui::BeginDisabled();
		}

		//if (ImGui::MenuItem("Save Project")) {}
		//ImGui::Separator();

		if (ImGui::MenuItem("Save", "Ctrl+S")) {
		}
		if (ImGui::MenuItem("Save as..")) {
		}
		if (ImGui::MenuItem("Save All", "Ctrl+Shift+S")) {
		}
		if (ImGui::MenuItem("Close Project")) {
			context.projectManager.CloseProject();
		}

		if (!hadProject)
		{
			//must not call EndDisabled if project is closed from menu
			ImGui::EndDisabled();
		}

		ImGui::Separator();
		if (ImGui::MenuItem("Quit", "Alt+F4")) {
			running = false;
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit")) {
		//todo check workspace manager for active cuttable/copyable item
		if (!context.projectManager.HasActiveProject()) { ImGui::BeginDisabled(); }
		if (ImGui::MenuItem("Cut", "Ctrl+X")) {
		}
		if (ImGui::MenuItem("Copy", "Ctrl+C")) {
		}
		if (ImGui::MenuItem("Paste", "Ctrl+V")) {
		}
		if (!context.projectManager.HasActiveProject()) { ImGui::EndDisabled(); }
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Window")) {
		//todo add items from windowmanager
		//if (ImGui::MenuItem("ISA Design")) {
		//}
		DrawWindowMenuItems();
		ImGui::EndMenu();
	}
	//if (ImGui::Button("Test Button")) {}
	ImGui::EndMainMenuBar();

	return running;
}*/

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