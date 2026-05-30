#pragma once
#include "../enumAppCommands.h"
#include "../ux/WindowManager.h"

#include "../AppContext.h"
//struct AppContext;

class AppCommandDispatcher {
public:
    void Dispatch(
        AppCommandRequest command, 
        WindowManager& window, 
        AppContext& ctx);

    static bool CanDispatch(
        AppCommand command, 
        const AppContext& ctx);
};