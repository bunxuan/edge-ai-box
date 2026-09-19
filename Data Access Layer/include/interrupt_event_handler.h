#pragma once

#include "device_status_cache.h"
#include "upstream_message_queue.h"

class InterruptEventHandler {
public:
    InterruptEventHandler(UpstreamMessageQueue* upstream_queue,
                          DeviceStatusCache* cache);

    void register_interrupt(int device_id, int gpio_fd);
    void unregister_interrupt(int device_id);
    void handle_interrupt(int gpio_fd);

private:
    UpstreamMessageQueue* upstream_queue_;
    DeviceStatusCache* cache_;
    std::unordered_map<int, int> gpio_to_device_;
    std::unordered_map<int, int> last_value_;
};
