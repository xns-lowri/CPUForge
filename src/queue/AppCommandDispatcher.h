#pragma once
#include "../enumAppCommands.h"
#include "../ux/WindowManager.h"

#include "../AppContext.h"
//#include "../core/AppComponentRegistry.h"
//struct AppContext;

class AppCommandDispatcher {
public:
    void Dispatch(
        AppCommandRequest command, 
        WindowManager& window, 
        AppContext& context);

    static bool CanDispatch(
        AppCommand command, 
        const AppContext& context);
};