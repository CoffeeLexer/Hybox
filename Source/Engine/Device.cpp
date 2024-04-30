#include "Device.h"

#include "Carcass.h"

#include <vulkan/vulkan.h>
#include <cstdio>
#include <vector>

void CreateDevice(Carcass &carcass)
{
    uint32_t deviceCount;
    std::vector<VkPhysicalDevice> devices{};
    vkEnumeratePhysicalDevices(carcass.instance, &deviceCount, nullptr);
    devices.resize(deviceCount);
    vkEnumeratePhysicalDevices(carcass.instance, &deviceCount, devices.data());
    printf("Device count: %u\n", deviceCount);

    for(const auto& device : devices)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        printf("PhyDevice: %u - %s\n", properties.apiVersion, properties.deviceName);
    }
}
