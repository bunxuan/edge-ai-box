#pragma once

#include "device_types.h"

#include <condition_variable>
#include <mutex>
#include <queue>

class UpstreamMessageQueue {
public:
    void push(const LayerMessage& msg);
    bool pop(LayerMessage* out);
    bool empty() const;

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<LayerMessage> queue_;
};
