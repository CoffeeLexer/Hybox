#include "Instance.h"

#include "Carcass.h"
#include "Whatchamacallit.h"
#include "Window/Window.h"

#include <cstdio>
#include <cstdlib>
#include <vulkan/vulkan.h>

#ifdef NDEBUG
const bool enableValidationLayer = false;
#else
const bool enableValidationLayer = true;
#endif

const std::vector<const char*> validationLayers
{
    "VK_LAYER_KHRONOS_validation",
};

std::vector<const char*> GetAccessibleLayers()
{
    uint32_t count;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    std::vector<VkLayerProperties> properties{};
    properties.resize(count);
    vkEnumerateInstanceLayerProperties(&count, properties.data());
    std::vector<const char*> names;
    for(const auto &property : properties)
        names.push_back(property.layerName);
    return names;
}

bool IsValidationLayerSupported()
{
    const auto accessible = GetAccessibleLayers();
    return ASubsetOfB(validationLayers, accessible);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    switch(severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        fprintf(stdout, "INFO: %s\n", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        fprintf(stdout, "WARN: %s\n", pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        fprintf(stderr, "ERRO: %s\n", pCallbackData->pMessage);
        break;
    default:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:\
        break;
    }
    return true;
}

VkDebugUtilsMessengerCreateInfoEXT GetDebugMessengerCreateInfo()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    createInfo.pfnUserCallback = debug_callback;
    createInfo.pUserData = nullptr;
    return createInfo;
}

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(func == nullptr)
        return VK_ERROR_EXTENSION_NOT_PRESENT;

    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pDebugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func == nullptr)
        fprintf(stderr, "ERROR: Debug messenger destroy function not loaded");
    else
        func(instance, pDebugMessenger, pAllocator);
}

void CreateDebugMessengerInternal(Carcass &carcass)
{
    auto &instance = carcass.instance;
    auto &debugMessenger = carcass.debugMessenger;
    auto createInfo = GetDebugMessengerCreateInfo();

    if(vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        fprintf(stderr, "CRITICAL: Failed to create debug messenger\n");
        exit(2);
    }
}

void DestroyDebugMessengerInternal(Carcass &carcass)
{
    auto &instance = carcass.instance;
    auto &debugMessenger = carcass.debugMessenger;

    vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

    debugMessenger = VK_NULL_HANDLE;
}


void CreateInstanceInternal(Carcass &carcass)
{
    auto &instance = carcass.instance;

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "Application Name";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "Engine Name";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;

    std::vector<const char*> extensions = Window::GetVulkanEssentialExtensions();
    auto debugMessengerCreateInfo = GetDebugMessengerCreateInfo();

    if(enableValidationLayer)
    {
        if(IsValidationLayerSupported())
        {
            printf("STATUS: Validation layers enabled\n");
            createInfo.enabledLayerCount = validationLayers.size();
            createInfo.ppEnabledLayerNames = validationLayers.data();
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerCreateInfo;

            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        else
        {
            fprintf(stderr, "ERROR: Validation Layer requirements not meet\n");
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
            createInfo.pNext = nullptr;
        }
    }
    else
    {
        printf("STATUS: Validation layers disabled\n");
    }

    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        instance = VK_NULL_HANDLE;
        fprintf(stderr, "CRITICAL: Failed creating instance");
        exit(1);
    }
}


void CreateInstance(Carcass &carcass)
{
    CreateInstanceInternal(carcass);
    if(enableValidationLayer && IsValidationLayerSupported())
        CreateDebugMessengerInternal(carcass);
}

void DestroyInstance(Carcass &carcass)
{
    // If not VK_NULL_HANDLE -> Destroy
    if(enableValidationLayer && IsValidationLayerSupported())
        DestroyDebugMessengerInternal(carcass);
    vkDestroyInstance(carcass.instance, nullptr);
}
