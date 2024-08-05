#pragma once

#include <vector>
#include <algorithm>
#include <vulkan/vulkan.h>

class Swapchain
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    VkPhysicalDevice device;
    VkSurfaceKHR surface;

    VkSurfaceCapabilitiesKHR GetCapabilities()
    {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);
        return capabilities;
    }

    std::vector<VkSurfaceFormatKHR> GetFormats()
    {
        uint32_t count;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);

        if (count == 0) return {};

        std::vector<VkSurfaceFormatKHR> formats(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data());

        return formats;
    }

    std::vector<VkPresentModeKHR> GetPresentModes()
    {
        uint32_t count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);

        if (count == 0) return {};
        
        std::vector<VkPresentModeKHR> modes(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, modes.data());
        
        return modes;
    }

    Swapchain(VkPhysicalDevice device, VkSurfaceKHR surface)
        : device(device)
        , surface(surface)
        , capabilities(GetCapabilities())
        , formats(GetFormats())
        , presentModes(GetPresentModes())
    {}

    // TODO: OSX P3 needs implementation
    VkSurfaceFormatKHR GetFormat()
    {
        for (const auto& format : formats)
        {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return format;
        }
        printf("WARN: Surface color shift\n");
        return formats[0];
    }

    VkPresentModeKHR GetPresentMode()
    {
        const VkPresentModeKHR ranks[] =
        {
            // Best
            VK_PRESENT_MODE_MAILBOX_KHR,
            VK_PRESENT_MODE_FIFO_KHR,
            VK_PRESENT_MODE_FIFO_RELAXED_KHR,
            VK_PRESENT_MODE_IMMEDIATE_KHR,
            // Worst
        };
        const uint32_t count = sizeof(ranks) / sizeof(ranks[0]);

        uint32_t best = count;
        for (const auto& mode : presentModes)
        {
            uint32_t distance = 0;
            for (; distance < count; distance++)
                if (ranks[distance] == mode) break;

            best = std::min(best, distance);
        }
        if (best == count) 
        {
            printf("WARN: Using unknown present mode\n");
            return presentModes[0];
        }
        return ranks[best];
    }

    VkExtent2D GetSwapExtent()
    {
        if (static_cast<uint32_t>(capabilities.currentExtent.width) == static_cast<uint32_t>(-1))
        {
            
        }
    }
};
