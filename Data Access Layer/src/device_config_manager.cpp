#include "device_config_manager.h"

#include <iostream>

DeviceConfigManager::DeviceConfigManager(const std::string& config_path)
    : config_path_(config_path) {}

bool DeviceConfigManager::load_config() {
    devices_.clear();
    devices_.push_back(DeviceDescriptor{1, "door_sensor", DeviceType::GPIO_SENSOR, CommInterface::GPIO, "GPIO_45", 500, 100, 3});
    devices_.push_back(DeviceDescriptor{2, "temp_humidity", DeviceType::MODBUS_SENSOR, CommInterface::RS485, "0x01", 5000, 1000, 5});
    devices_.push_back(DeviceDescriptor{3, "i2c_temp", DeviceType::I2C_SENSOR, CommInterface::I2C, "0x40", 3000, 200, 3});
    std::cout << "[config] loaded " << devices_.size() << " device entries\n";
    return true;
}

bool DeviceConfigManager::reload_config() {
    return load_config();
}

std::vector<DeviceDescriptor> DeviceConfigManager::get_all_devices() const {
    return devices_;
}

std::unique_ptr<DeviceDriver> DeviceConfigManager::create_driver(const DeviceDescriptor& desc) const {
    switch (desc.iface) {
        case CommInterface::GPIO:
            return std::make_unique<GPIODriver>();
        case CommInterface::RS485:
            return std::make_unique<ModbusRTUDriver>();
        case CommInterface::I2C:
            return std::make_unique<I2CDriver>();
        case CommInterface::SPI:
            return std::make_unique<SPIDriver>();
        default:
            return nullptr;
    }
}
