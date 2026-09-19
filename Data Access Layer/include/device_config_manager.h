#pragma once

#include "device_driver.h"
#include "device_types.h"

#include <memory>
#include <string>
#include <vector>

class DeviceConfigManager {
public:
    explicit DeviceConfigManager(const std::string& config_path);

    bool load_config();
    bool reload_config();
    std::vector<DeviceDescriptor> get_all_devices() const;
    std::unique_ptr<DeviceDriver> create_driver(const DeviceDescriptor& desc) const;

private:
    std::string config_path_;
    std::vector<DeviceDescriptor> devices_;
};
