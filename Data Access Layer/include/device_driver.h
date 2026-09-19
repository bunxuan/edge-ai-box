#pragma once

#include "device_types.h"

#include <string>

class DeviceDriver {
public:
    virtual ~DeviceDriver() = default;

    virtual bool init(const std::string& config_json) = 0;
    virtual DriverResult read_nonblock(int device_id, DeviceStatus* out) = 0;
    virtual DriverResult write_command(int device_id, CmdType cmd_type,
                                       const std::vector<int>& params,
                                       CmdResult* out) = 0;
    virtual CommInterface get_bus_interface() const = 0;
    virtual std::string driver_name() const = 0;
};

class GPIODriver : public DeviceDriver {
public:
    bool init(const std::string& config_json) override;
    DriverResult read_nonblock(int device_id, DeviceStatus* out) override;
    DriverResult write_command(int device_id, CmdType cmd_type,
                              const std::vector<int>& params,
                              CmdResult* out) override;
    CommInterface get_bus_interface() const override { return CommInterface::GPIO; }
    std::string driver_name() const override { return "GPIO"; }
};

class ModbusRTUDriver : public DeviceDriver {
public:
    bool init(const std::string& config_json) override;
    DriverResult read_nonblock(int device_id, DeviceStatus* out) override;
    DriverResult write_command(int device_id, CmdType cmd_type,
                              const std::vector<int>& params,
                              CmdResult* out) override;
    CommInterface get_bus_interface() const override { return CommInterface::RS485; }
    std::string driver_name() const override { return "ModbusRTU"; }
};

class I2CDriver : public DeviceDriver {
public:
    bool init(const std::string& config_json) override;
    DriverResult read_nonblock(int device_id, DeviceStatus* out) override;
    DriverResult write_command(int device_id, CmdType cmd_type,
                              const std::vector<int>& params,
                              CmdResult* out) override;
    CommInterface get_bus_interface() const override { return CommInterface::I2C; }
    std::string driver_name() const override { return "I2C"; }
};

class SPIDriver : public DeviceDriver {
public:
    bool init(const std::string& config_json) override;
    DriverResult read_nonblock(int device_id, DeviceStatus* out) override;
    DriverResult write_command(int device_id, CmdType cmd_type,
                              const std::vector<int>& params,
                              CmdResult* out) override;
    CommInterface get_bus_interface() const override { return CommInterface::SPI; }
    std::string driver_name() const override { return "SPI"; }
};
