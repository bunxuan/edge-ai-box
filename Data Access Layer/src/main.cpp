#include "device_polling_thread.h"
#include "device_status_cache.h"
#include "command_queue.h"
#include "upstream_message_queue.h"
#include "device_config_manager.h"
#include "interrupt_event_handler.h"

#include <iostream>

int main() {
    std::cout << "[data-access] starting skeleton ...\n";

    CommandQueue command_queue;
    DeviceStatusCache cache;
    UpstreamMessageQueue upstream_queue;
    DeviceConfigManager config_manager("config/devices.json");
    InterruptEventHandler interrupt_handler(&upstream_queue, &cache);

    if (!config_manager.load_config()) {
        std::cerr << "failed to load device config\n";
        return 1;
    }

    DevicePollingThread polling_thread(&command_queue, &cache, &upstream_queue,
                                      &config_manager, &interrupt_handler);
    polling_thread.start();

    std::cout << "[data-access] skeleton running. press enter to stop.\n";
    std::cin.get();

    polling_thread.stop();
    std::cout << "[data-access] stopped.\n";
    return 0;
}
