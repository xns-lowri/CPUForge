#pragma once

#include <queue>
#include <mutex>
#include <optional>
#include "../enumAppCommands.h"

class AppCommandQueue {
public:
    AppCommandQueue() = default;

    void Push(AppCommand command) {
        Push({command, ""});
    }

    void Push(AppCommandRequest request) {
        //std::lock_guard<std::mutex> lock(mutex_); //do these need to be released?
        queue_.push(request);
    }

    std::optional<AppCommandRequest> TryPop() {
        //std::lock_guard<std::mutex> lock(mutex_);

        if (queue_.empty()) {
            return std::nullopt;
        }

        AppCommandRequest command = queue_.front();
        queue_.pop();

        return command;
    }

    bool Empty() const {
        //std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

private:
    //std::mutex mutex_;
    std::queue<AppCommandRequest> queue_;
};