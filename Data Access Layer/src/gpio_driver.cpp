#include "device_driver.h"

bool GPIODriver::init(const std::string& config_json) {
    (void)config_json;
    return true;
}

DriverResult GPIODriver::read_nonblock(int device_id, DeviceStatus* out) {
    if (!out) return DriverResult::ERROR;
    *out = DeviceStatus{device_id, DeviceType::GPIO_SENSOR, CommInterface::GPIO, true, 0.0, false, 0};
    return DriverResult::OK;
}

DriverResult GPIODriver::write_command(int device_id, CmdType cmd_type,
                                       const std::vector<int>& params,
                                       CmdResult* out) {
    (void)device_id;
    (void)cmd_type;
    (void)params;
    if (out) {
        out->success = true;
        out->error = "ok";
    }
    return DriverResult::OK;
}
