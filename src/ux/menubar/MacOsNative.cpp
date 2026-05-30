#include "MacOsNative.h"

AppCommandQueue* NativeCommandBridge::commandQueue = nullptr;

void NativeCommandBridge::SetCommandQueue(AppCommandQueue* queue) {
    commandQueue = queue;
}

void NativeCommandBridge::PushCommand(AppCommandRequest request) {
    fmt::println(
        "Got macos native command: {:s} from {:s}", 
        ToString(request.command), 
        request.id);

    if(!commandQueue) {
        return;
    }

    commandQueue->Push(std::move(request));
}