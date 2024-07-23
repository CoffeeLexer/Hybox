#pragma once

struct Carcass;

static const char* requiredDeviceExtensions[] =
{
    "VK_KHR_swapchain",
};

void CreateDevice(Carcass *carcass);
void DestroyDevice(Carcass *carcass);
