#include "command_queue.h"

void CommandQueue::push(const DeviceCommand& cmd) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(cmd);
    cv_.notify_one();
}

bool CommandQueue::try_pop(DeviceCommand* out) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    *out = queue_.front();
    queue_.pop();
    return true;
}

void CommandQueue::complete(const DeviceCommand& cmd, const CmdResult& result) {
    (void)cmd;
    (void)result;
}

bool CommandQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}
