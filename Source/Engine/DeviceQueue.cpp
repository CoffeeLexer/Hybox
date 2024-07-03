#include "DeviceQueue.h"

#include <vector>

#include "Carcass.h"


QueueFamilyIndices FindQueueFamily(Carcass *carcass)
{
    auto &physicalDevice = carcass->physicalDevice;

    uint32_t familyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, nullptr);
    std::vector<VkQueueFamilyProperties> families(familyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, families.data());

    QueueFamilyIndices indices{};
    for (uint32_t i = 0; i < familyCount; i++)
    {
        const uint32_t &flags = families[i].queueFlags;

        if (flags & VK_QUEUE_TRANSFER_BIT)
            indices.transfer = i;

        if (flags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphics = i;

        if (flags & VK_QUEUE_COMPUTE_BIT)
            indices.compute = i;

        if (indices.isComplete())
            break;
    }

    return indices;
}
