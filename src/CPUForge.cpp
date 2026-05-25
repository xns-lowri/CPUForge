//include libraries
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <fmt/core.h>

//include header
#include "CPUForge.h"
#include "AppContext.h"

using namespace std;

//app variables - todo class?
//UI ui;
//ProjectManager projectManager;

int main(int argc, char** argv)
{
	fmt::print("CPUForge is starting...\n");
	//create project manager
    //todo load last project if able
    AppContext appContext = AppContext(
        ProjectManager(), 
        WorkspaceManager()
    );

    std::optional<std::filesystem::path> lastProjectPath = appContext.projectManager.LoadProjectContext();
    if (lastProjectPath.has_value()) {
        appContext.projectManager.OpenProject(lastProjectPath.value());
    }

    fmt::println("Loaded project: {}", appContext.projectManager.HasActiveProject());

	//projectManager.NewProject("default");

    //create ui
	UI ui = UI();
	ui.Init();

    bool running = true;
    while (running)
    {
        //event loop - like in game engines
        
		//todo - update simulation here when running
        running = ui.Render(appContext);
        //handle events from ui
    }

	ui.Close();
    return 0;
}
