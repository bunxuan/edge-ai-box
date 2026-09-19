#include "device_polling_thread.h"

#include <chrono>
#include <iostream>

DevicePollingThread::DevicePollingThread(CommandQueue* command_queue,
                                         DeviceStatusCache* device_cache,
                                         UpstreamMessageQueue* upstream_queue,
                                         DeviceConfigManager* config_manager,
                                         InterruptEventHandler* interrupt_handler)
    : command_queue_(command_queue),
      device_cache_(device_cache),
      upstream_queue_(upstream_queue),
      config_manager_(config_manager),
      interrupt_handler_(interrupt_handler) {}

void DevicePollingThread::start() {
    running_ = true;
    worker_ = std::thread([this]() {
        rebuild_time_wheel();
        while (running_) {
            run_once();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });
}

void DevicePollingThread::stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void DevicePollingThread::run_once() {
    auto devices = config_manager_->get_all_devices();
    if (time_wheel_.empty()) {
        rebuild_time_wheel();
    }

    for (auto& entry : time_wheel_) {
        auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();

        if (now >= entry.next_poll_time) {
            DeviceStatus status;
            auto res = entry.driver->read_nonblock(entry.device_id, &status);
            if (res == DriverResult::OK) {
                handle_poll_success(entry.device_id, status);
            } else {
                handle_poll_timeout(entry.device_id);
            }
            entry.next_poll_time = now + entry.interval_ms;
        }
    }

    execute_command_if_available();
}

void DevicePollingThread::rebuild_time_wheel() {
    auto devices = config_manager_->get_all_devices();
    time_wheel_.clear();

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();

    for (const auto& dev : devices) {
        auto driver = config_manager_->create_driver(dev);
        if (!driver) {
            continue;
        }
        driver->init("{}" );
        PollingEntry entry;
        entry.device_id = dev.device_id;
        entry.driver = std::move(driver);
        entry.interval_ms = dev.poll_interval_ms;
        entry.next_poll_time = now + 50 * (dev.device_id % 10);
        time_wheel_.push_back(std::move(entry));
    }
}

void DevicePollingThread::execute_command_if_available() {
    DeviceCommand cmd;
    if (!command_queue_->try_pop(&cmd)) {
        return;
    }

    DeviceStatus status;
    auto res = GPIODriver().read_nonblock(cmd.device_id, &status);
    (void)res;
    std::cout << "[polling] executed command for device " << cmd.device_id << "\n";
}

void DevicePollingThread::handle_poll_success(int device_id, const DeviceStatus& status) {
    device_cache_->update(status);
    std::cout << "[polling] success device=" << device_id << " value=" << status.value << "\n";
}

void DevicePollingThread::handle_poll_timeout(int device_id) {
    device_cache_->mark_offline(device_id);
    std::cout << "[polling] timeout device=" << device_id << "\n";
}
