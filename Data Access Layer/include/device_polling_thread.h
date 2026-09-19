#pragma once

#include "command_queue.h"
#include "device_config_manager.h"
#include "device_status_cache.h"
#include "interrupt_event_handler.h"
#include "upstream_message_queue.h"

#include <atomic>
#include <thread>
#include <vector>

struct PollingEntry {
    int device_id = -1;
    std::unique_ptr<DeviceDriver> driver;
    int interval_ms = 1000;
    std::uint64_t next_poll_time = 0;
    int timeout_count = 0;
};

class DevicePollingThread {
public:
    DevicePollingThread(CommandQueue* command_queue,
                       DeviceStatusCache* device_cache,
                       UpstreamMessageQueue* upstream_queue,
                       DeviceConfigManager* config_manager,
                       InterruptEventHandler* interrupt_handler);

    void start();
    void stop();
    void run_once();

private:
    void rebuild_time_wheel();
    void execute_command_if_available();
    void handle_poll_success(int device_id, const DeviceStatus& status);
    void handle_poll_timeout(int device_id);

    CommandQueue* command_queue_;
    DeviceStatusCache* device_cache_;
    UpstreamMessageQueue* upstream_queue_;
    DeviceConfigManager* config_manager_;
    InterruptEventHandler* interrupt_handler_;

    std::vector<PollingEntry> time_wheel_;
    std::thread worker_;
    std::atomic<bool> running_{false};
};
