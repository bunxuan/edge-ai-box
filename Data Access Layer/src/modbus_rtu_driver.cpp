#include "device_driver.h"

bool ModbusRTUDriver::init(const std::string& config_json) {
    (void)config_json;
    return true;
}

DriverResult ModbusRTUDriver::read_nonblock(int device_id, DeviceStatus* out) {
    if (!out) return DriverResult::ERROR;
    *out = DeviceStatus{device_id, DeviceType::MODBUS_SENSOR, CommInterface::RS485, true, 0.0, false, 0};
    return DriverResult::OK;
}

DriverResult ModbusRTUDriver::write_command(int device_id, CmdType cmd_type,
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
