#pragma once

struct Carcass;

static const char*[] requiredDeviceExtensions =
{
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

void CreateDevice(Carcass *carcass);
void DestroyDevice(Carcass *carcass);
