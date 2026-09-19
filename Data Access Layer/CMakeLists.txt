cmake_minimum_required(VERSION 3.16)
project(edge_ai_box_data_access LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(edge_ai_box_data_access
    src/main.cpp
    src/command_queue.cpp
    src/device_status_cache.cpp
    src/upstream_message_queue.cpp
    src/device_driver.cpp
    src/gpio_driver.cpp
    src/modbus_rtu_driver.cpp
    src/i2c_driver.cpp
    src/spi_driver.cpp
    src/device_config_manager.cpp
    src/interrupt_event_handler.cpp
    src/device_polling_thread.cpp
)
