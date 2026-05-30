#include "AppCommandQueue.h"

void AppCommandQueue::Push(AppCommand command) {
    fmt::println("Pushing app command - no id");
    Push({ command, "" });
}

void AppCommandQueue::Push(AppCommandRequest request) {
    fmt::println(
        "- Push commandQueue address: {}",
        static_cast<void*>(this)
    );
    fmt::println("Pushing app request");
    std::lock_guard<std::mutex> lock(mutex_);
    //queue_.push(request);
    queue_.push(request);
    fmt::println("Pushed app request");
}

std::optional<AppCommandRequest> AppCommandQueue::TryPop() {
    std::lock_guard<std::mutex> lock(mutex_);
    //fmt::println("Trying to pop next UI request");

    if (queue_.empty()) {
        //fmt::println("Request queue is empty");
        return std::nullopt;
    }

    //fmt::println(
    //    "- TryPop commandQueue address: {}",
    //    static_cast<void*>(this)
    //);

    //fmt::println("Items in queue: {}", queue_.size());

    AppCommandRequest command = queue_.front();
    //fmt::println("Got command from queue: {:s}", ToString(command.command));

    /*if (queue_.empty()) {
        fmt::println("Request queue is empty after reading?");
    }*/

    //fmt::println("Items in queue before pop: {}", queue_.size());
    //fmt::println(
    //    "size={}, begin==end? {}",
    //    queue_.size(),
    //    queue_.begin() == queue_.end()
    //);
    queue_.pop();
    //queue_.erase(queue_.begin());
    fmt::println("Popped request queue");

    return command;
}

bool AppCommandQueue::Empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}