//include libraries
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <fmt/core.h>

//include header
#include "CPUForge.h"

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
        AppCommandQueue(),
        ProjectManager(), 
        WorkspaceManager()
    );

    AppCommandDispatcher dispatcher = AppCommandDispatcher();

    std::optional<std::filesystem::path> lastProjectPath = appContext.projectManager.LoadProjectContext();
    if (lastProjectPath.has_value()) {
        appContext.projectManager.OpenProject(lastProjectPath.value());
    }

    fmt::println("Loaded project: {}", appContext.projectManager.HasActiveProject());

	//projectManager.NewProject("default");

    //create ui
    WindowManager windowManager = WindowManager(appContext);
	UI ui = UI(windowManager);
	ui.Init(appContext);

    bool running = true;
    while (running)
    {
        //event loop - like in game engines
        
		//todo - update simulation here when running
        running = ui.Render(appContext);

        if(appContext.requestQuit) {
            break;
        }
        //handle events from ui
        while(!appContext.commandQueue.Empty()) {
            std::optional<AppCommandRequest> request = appContext.commandQueue.TryPop();
            fmt::println("Got request");
            if(!request.has_value()) { break; }
            if(!dispatcher.CanDispatch(request->command, appContext)) { continue; }
            dispatcher.Dispatch(
                request.value(), 
                ui.GetWindowManager(), 
                appContext
            );
        }
    }

	ui.Close();
    return 0;
}
