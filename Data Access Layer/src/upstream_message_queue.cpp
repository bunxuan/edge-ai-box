#include "upstream_message_queue.h"

void UpstreamMessageQueue::push(const LayerMessage& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(msg);
    cv_.notify_one();
}

bool UpstreamMessageQueue::pop(LayerMessage* out) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    *out = queue_.front();
    queue_.pop();
    return true;
}

bool UpstreamMessageQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}
