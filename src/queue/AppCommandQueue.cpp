#include "AppCommandQueue.h"

void AppCommandQueue::Push(AppCommand command) {
    fmt::println("Pushing app command - no id");
    Push({ command, "" });
}

void AppCommandQueue::Push(AppCommandRequest request) {
    fmt::println("Pushing app request");
    //std::lock_guard<std::mutex> lock(mutex_); //do these need to be released?
    queue_.push(request);
    fmt::println("Pushing app request");
}

std::optional<AppCommandRequest> AppCommandQueue::TryPop() {
    //std::lock_guard<std::mutex> lock(mutex_);
    fmt::println("Trying to pop next UI request");

    if (queue_.empty()) {
        fmt::println("Request queue is empty");
        return std::nullopt;
    }

    AppCommandRequest command = std::move(queue_.front());
    fmt::println("Got command from queue: {:s}", ToString(command.command));

    if (queue_.empty()) {
        fmt::println("Request queue is empty after reading?");
    }


    queue_.pop();
    fmt::println("Popped request queue");

    return command;
}

bool AppCommandQueue::Empty() const {
    //std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}