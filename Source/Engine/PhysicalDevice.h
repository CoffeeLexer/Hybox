#pragma once

#include <vector>

struct Surface
{

};

struct PhysicalDevice
{
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    std::vector<VkExtensionProperties> extensions;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkQueueFamilyProperties> queueFamilies;
    VkPhysicalDeviceMemoryProperties memory;

    VkSurfaceCapabilitiesKHR capabilities;
    
    std::vector<VkPresentModeKHR> presentModes;

    PhysicalDevice(VkPhysicalDevice);
private:
    VkPhysicalDevice physicalDevice;

    void GetProperties();
    void GetFeatures();

    void GetSurfaceCapabilities();
    void GetSurfaceFormats();
    void GetPresentModes();
};
