#include "Device.h"

#include "Carcass.h"
#include "DeviceQueue.h"

#include <vulkan/vulkan.h>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <tuple>
#include <functional>

static uint32_t RatePhysicalDevice(const VkPhysicalDevice &device)
{
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;

    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);

    uint32_t rating = 0;
    switch(properties.deviceType)
    {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        rating++;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        rating++;
    default:
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        rating++;
    } // 2 bits
    rating <<= 2;

    return rating;
}

static void ListPhysicalDevices(Carcass *carcass)
{
    auto &instance = carcass->instance;

    uint32_t deviceCount;
    std::vector<VkPhysicalDevice> devices{};

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    devices.resize(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for(const auto& device : devices)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        printf("Device: %s - %u\n", properties.deviceName, properties.apiVersion);
    }
}

static void AquirePhysicalDevice(Carcass *carcass)
{
    auto &instance = carcass->instance;
    auto &physicalDevice = carcass->physicalDevice;

    uint32_t deviceCount;
    std::vector<VkPhysicalDevice> devices{};

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    devices.resize(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    uint32_t bestRating = 0;
    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    for(const auto& device : devices)
    {
        const uint32_t newRating = RatePhysicalDevice(device);
        if(newRating > bestRating)
        {
            bestRating = newRating;
            bestDevice = device;
        }
    }

    physicalDevice = bestDevice != VK_NULL_HANDLE ? bestDevice : devices[0];
}

static void DestroyDeviceInternal(Carcass *carcass)
{
    auto &device = carcass->device;
    auto &graphicsQueue = carcass->graphicsQueue;

    vkDestroyDevice(device, nullptr);

    device = VK_NULL_HANDLE;
    graphicsQueue = VK_NULL_HANDLE;
}

void CreateDevice(Carcass *carcass)
{
    AquirePhysicalDevice(carcass);

    auto &physicalDevice = carcass->physicalDevice;
    auto &device = carcass->device;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.flags = 0;

    QueueFamilyIndices family = FindQueueFamily(carcass);

    std::vector<VkDeviceQueueCreateInfo> queueInfos{};
    std::vector<std::tuple<uint32_t, std::reference_wrapper<VkQueue>>> constructInfo =
    {
        {family.graphics.value(), std::ref(carcass->graphicsQueue)},
        {family.present.value(), std::ref(carcass->presentQueue)},
    };

    float queuePriority = 1.0f;
    for (const auto& info : constructInfo)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = std::get<0>(info);
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueInfos.push_back(queueCreateInfo);
    }

    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueInfos.data();

    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = nullptr;

    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);

    for (uint32_t i = 0; i < constructInfo.size(); i++)
        vkGetDeviceQueue(device, std::get<0>(constructInfo[i]), 0, &std::get<1>(constructInfo[i]).get());
}

void DestroyDevice(Carcass *carcass)
{
    DestroyDeviceInternal(carcass);
}
