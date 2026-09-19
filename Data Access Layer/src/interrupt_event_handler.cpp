#include "interrupt_event_handler.h"

#include <iostream>

InterruptEventHandler::InterruptEventHandler(UpstreamMessageQueue* upstream_queue,
                                           DeviceStatusCache* cache)
    : upstream_queue_(upstream_queue), cache_(cache) {}

void InterruptEventHandler::register_interrupt(int device_id, int gpio_fd) {
    gpio_to_device_[gpio_fd] = device_id;
    last_value_[gpio_fd] = 0;
    std::cout << "[interrupt] register device " << device_id << " fd=" << gpio_fd << "\n";
}

void InterruptEventHandler::unregister_interrupt(int device_id) {
    for (auto it = gpio_to_device_.begin(); it != gpio_to_device_.end(); ++it) {
        if (it->second == device_id) {
            gpio_to_device_.erase(it);
            last_value_.erase(it->first);
            break;
        }
    }
}

void InterruptEventHandler::handle_interrupt(int gpio_fd) {
    auto it = gpio_to_device_.find(gpio_fd);
    if (it == gpio_to_device_.end()) {
        return;
    }

    int device_id = it->second;
    int value = 1 - last_value_[gpio_fd];
    last_value_[gpio_fd] = value;

    DeviceStatus status;
    status.device_id = device_id;
    status.online = true;
    status.value = value;
    status.switch_state = value != 0;
    cache_->update(status);

    LayerMessage msg;
    msg.device_id = device_id;
    msg.event = EventType::STATE_CHANGED;
    msg.payload = "interrupt_state_change";
    upstream_queue_->push(msg);

    std::cout << "[interrupt] device " << device_id << " changed to " << value << "\n";
}
