#include "device_status_cache.h"

void DeviceStatusCache::update(const DeviceStatus& status) {
    std::lock_guard<std::mutex> lock(mutex_);
    status_map_[status.device_id] = status;
}

bool DeviceStatusCache::get(int device_id, DeviceStatus* out) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = status_map_.find(device_id);
    if (it == status_map_.end()) {
        return false;
    }
    *out = it->second;
    return true;
}

void DeviceStatusCache::mark_offline(int device_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = status_map_.find(device_id);
    if (it != status_map_.end()) {
        it->second.online = false;
    }
}

bool DeviceStatusCache::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return status_map_.empty();
}
