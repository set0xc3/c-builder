#pragma once

#include "base/defines.h"
#include "base/string.h"
#include <vulkan/vulkan.h>

typedef struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;

  VkSurfaceFormatKHR *format_list;
  u32                 format_count;

  VkPresentModeKHR *present_mode_list;
  u32               present_mode_count;
} SwapChainSupportDetails;

typedef struct QueueFamilyIndices {
  b32 is_complete;

  u32 graphics_family_index;
  u32 present_family_index;
} QueueFamilyIndices;

typedef struct Vulkan_Context {
  VkDebugUtilsMessengerEXT debug_messenger;

  VkInstance       instance;
  VkDevice         device;
  VkSurfaceKHR     surface;
  VkQueue          graphics_queue;
  VkQueue          present_queue;
  VkPhysicalDevice gpu;
} Vulkan_Context;

QueueFamilyIndices find_queue_families(VkPhysicalDevice _device);

b32 get_required_extensions(const char **out_extension_names,
                            u32         *out_extension_count);
b32 check_validation_layer_support(void);
b32 check_device_extension_support(VkPhysicalDevice device);

b32 setup_debug_messenger(void);

void create_debug_messenger_information(
    VkDebugUtilsMessengerCreateInfoEXT *create_info);

VkResult create_debug_utils_messenger_ext(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT    *pDebugMessenger);

void
destroy_debug_utils_messenger_ext(VkInstance                   instance,
                                  VkDebugUtilsMessengerEXT     debugMessenger,
                                  const VkAllocationCallbacks *pAllocator);

b32 is_device_suitable(VkPhysicalDevice device);

SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice device);

API void vulkan_init(void);
API void vulkan_destroy(void);

API b32  vulkan_instance_create(void);
API b32  vulkan_surface_create(void);
API void vulkan_physical_device_pick(void);
API void vulkan_physical_device_create(void);
API void vulkan_pipeline_create(const String vert_filepath,
                                const String frag_filepath);
