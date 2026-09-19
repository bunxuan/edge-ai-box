#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

enum class DeviceType {
    GPIO_SENSOR,
    GPIO_ACTUATOR,
    MODBUS_SENSOR,
    I2C_SENSOR,
    SPI_SENSOR,
    UNKNOWN
};

enum class CommInterface {
    GPIO,
    RS485,
    I2C,
    SPI,
    UNKNOWN_INTERFACE
};

enum class DriverResult {
    OK,
    PENDING,
    ERROR
};

enum class CmdType {
    READ_STATUS,
    OPEN_DOOR,
    START_FAN,
    STOP_FAN,
    WRITE_REGISTER,
    UNKNOWN_CMD
};

enum class EventType {
    DEVICE_OFFLINE,
    DEVICE_RECOVERED,
    STATE_CHANGED,
    UNKNOWN_EVENT
};

struct DeviceStatus {
    int device_id = -1;
    DeviceType type = DeviceType::UNKNOWN;
    CommInterface iface = CommInterface::UNKNOWN_INTERFACE;
    bool online = true;
    double value = 0.0;
    bool switch_state = false;
    std::uint64_t last_update = 0;
};

struct DeviceCommand {
    int device_id = -1;
    CmdType type = CmdType::UNKNOWN_CMD;
    std::vector<int> params;
};

struct CmdResult {
    bool success = false;
    std::string error;
    double value = 0.0;
};

struct LayerMessage {
    int device_id = -1;
    EventType event = EventType::UNKNOWN_EVENT;
    std::uint64_t timestamp = 0;
    std::string payload;
};

struct DeviceDescriptor {
    int device_id = -1;
    std::string name;
    DeviceType type = DeviceType::UNKNOWN;
    CommInterface iface = CommInterface::UNKNOWN_INTERFACE;
    std::string address;
    int poll_interval_ms = 1000;
    int timeout_ms = 200;
    int offline_threshold = 3;
};
