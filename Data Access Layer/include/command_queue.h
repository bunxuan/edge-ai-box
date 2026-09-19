#pragma once

#include "device_types.h"

#include <condition_variable>
#include <mutex>
#include <queue>

class CommandQueue {
public:
    void push(const DeviceCommand& cmd);
    bool try_pop(DeviceCommand* out);
    void complete(const DeviceCommand& cmd, const CmdResult& result);
    bool empty() const;

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<DeviceCommand> queue_;
};
