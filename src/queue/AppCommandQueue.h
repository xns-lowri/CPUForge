#pragma once
#include <fmt/core.h>
#include <queue>
#include <mutex>
#include <optional>
#include "../enumAppCommands.h"

class AppCommandQueue {
public:
    AppCommandQueue() = default;

    void Push(AppCommand command);

    void Push(AppCommandRequest request);

    std::optional<AppCommandRequest> TryPop();

    bool Empty() const;

private:
    //std::mutex mutex_;
    std::queue<AppCommandRequest> queue_;
};