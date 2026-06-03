#pragma once
#include "MenuData.h"
#include "../../queue/AppCommandQueue.h"
#include "../../enumAppCommands.h"

#ifdef __APPLE__
extern "C" void MacMenu_Update(const MenuBarModel& menu);

class NativeCommandBridge 
{
private:
    static AppCommandQueue* commandQueue;
public:
    static void SetCommandQueue(AppCommandQueue* queue);
    static void PushCommand(AppCommandRequest request);
};
#endif