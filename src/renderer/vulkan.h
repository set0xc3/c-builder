#pragma once

#include "base/defines.h"
#include "base/string.h"
#include <vulkan/vulkan.h>

typedef struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;

  VkSurfaceFormatKHR *format_list;
  u64                 format_count;

  VkPresentModeKHR *present_mode_list;
  u64               present_mode;
} SwapChainSupportDetails;

typedef struct QueueFamilyIndices {
  u32 graphics_family;
  u32 present_family;

  b8 graphics_family_has_value;
  b8 present_family_has_value;
} QueueFamilyIndices;

typedef struct Vulkan_Context {
  b8 enable_validation_layers;

  VkInstance        instance;
  VkDevice          device;
  VkSurfaceKHR      surface;
  VkQueue           graphics_queue;
  VkQueue           present_queue;
  VkPhysicalDevice *gpu;
} Vulkan_Context;

API QueueFamilyIndices vulkan_queue_families_find(VkPhysicalDevice *_device);

API void vulkan_init(void);
API void vulkan_instance_create(void);
API void vulkan_surface_create(void);
API void vulkan_physical_device_pick(void);
API void vulkan_physical_device_create(void);
API void vulkan_pipeline_create(const String vert_filepath,
                                const String frag_filepath);
