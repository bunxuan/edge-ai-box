#pragma once

#include "device_types.h"

#include <mutex>
#include <unordered_map>

class DeviceStatusCache {
public:
    void update(const DeviceStatus& status);
    bool get(int device_id, DeviceStatus* out) const;
    void mark_offline(int device_id);
    bool empty() const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<int, DeviceStatus> status_map_;
};
