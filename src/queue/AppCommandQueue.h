#pragma once
#include <fmt/core.h>
#include <vector>
#include <queue>
#include <mutex>
#include <optional>
#include "../enumAppCommands.h"

class AppCommandQueue {
public:
    AppCommandQueue() = default;
    ~AppCommandQueue() = default;

    AppCommandQueue(const AppCommandQueue&) = delete;
    AppCommandQueue& operator=(const AppCommandQueue&) = delete;
    AppCommandQueue(AppCommandQueue&&) = delete;
    AppCommandQueue& operator=(AppCommandQueue&&) = delete;

    void Push(AppCommand command);
    void Push(AppCommandRequest request);
    std::optional<AppCommandRequest> TryPop();
    bool Empty() const;

private:
    mutable std::mutex mutex_;
    std::queue<AppCommandRequest> queue_;
    //std::vector<AppCommandRequest> queue_;
};