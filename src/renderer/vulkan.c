#include "vulkan.h"

#include "base/defines.h"
#include "base/log.h"
#include "base/string.h"
#include "os/os.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vulkan/vulkan.h>

static Vulkan_Context *ctx;

QueueFamilyIndices
vulkan_queue_families_find(VkPhysicalDevice *_device)
{
  QueueFamilyIndices result = { 0 };
  VkPhysicalDevice   device = *_device;

  VkQueueFamilyProperties *queue_families     = NULL;
  u32                      queue_family_count = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
  queue_families
      = malloc(sizeof(VkQueueFamilyProperties) * queue_family_count);

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count,
                                           queue_families);

  for (u32 i = 0; i < queue_family_count; i++) {
    if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      result.graphics_family = i;
    }

    VkBool32 support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, ctx->surface, &support);

    if (support) {
      result.present_family = i;
    }

    if (result.graphics_family >= 0 && result.present_family >= 0) {
      break;
    }
  }

  free(queue_families);

  LOG_INFO("[VK] queue_props: %u\n", queue_families);
  LOG_INFO("[VK] queue_count: %u\n", queue_family_count);

  return result;
}

void
vulkan_init(void)
{
  ctx = malloc(sizeof(Vulkan_Context));
  memset(ctx, 0, sizeof(Vulkan_Context));

#ifndef NDEBUG
  ctx->enable_validation_layers = true;
#endif

  SDL_Vulkan_LoadLibrary(NULL);

  vulkan_instance_create();
  vulkan_surface_create();
  vulkan_physical_device_pick();
  vulkan_physical_device_create();
  // vulkan_pipeline_create(str_lit("assets/shaders/simple.vert"),
  //                        str_lit("assets/shaders/simple.frag"));
}

void
vulkan_instance_create(void)
{
  OS_Window *window = os_window_root_get();

  if (ctx->enable_validation_layers) {
    // LOG_WARN("Validation layers not supported\n");
  }

  u32          extension_count = 0;
  const char **extension_names = { 0 };

  SDL_Vulkan_GetInstanceExtensions(window->sdl.handle, &extension_count, NULL);

  extension_names = malloc(sizeof(char) * extension_count);

  SDL_Vulkan_GetInstanceExtensions(window->sdl.handle, &extension_count,
                                   extension_names);

  const VkApplicationInfo app_info = {
    .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext              = NULL,
    .pApplicationName   = "NoAppName",
    .applicationVersion = 0,
    .pEngineName        = "NoEngineName",
    .engineVersion      = 0,
    .apiVersion         = VK_API_VERSION_1_0,
  };

  VkInstanceCreateInfo create_info = {
    .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = 0,
    .pApplicationInfo        = &app_info,
    .enabledLayerCount       = 0,
    .ppEnabledLayerNames     = NULL,
    .enabledExtensionCount   = extension_count,
    .ppEnabledExtensionNames = extension_names,
  };

  vkCreateInstance(&create_info, NULL, &ctx->instance);

  LOG_INFO("[VK] ctx: %u\n", ctx);
  LOG_INFO("[VK] ctx->instance: %u\n", ctx->instance);
  LOG_INFO("[VK] enabledExtensionCount: %u\n", extension_count);
  LOG_INFO("[VK] ppEnabledExtensionNames:\n");

  for (u32 i = 0; i < extension_count; i++) {
    LOG_INFO("[VK]    %s\n", extension_names[i]);
  }
}

void
vulkan_surface_create(void)
{
  OS_Window *window = os_window_root_get();

  SDL_Vulkan_CreateSurface(window->sdl.handle, ctx->instance, &ctx->surface);
  LOG_INFO("[VK] surface: %u\n", ctx->surface);
}

void
vulkan_physical_device_pick(void)
{
  VkPhysicalDevice *physical_devices = NULL;
  u32               device_count     = 0;

  vkEnumeratePhysicalDevices(ctx->instance, &device_count, NULL);
  physical_devices = malloc(sizeof(VkPhysicalDevice) * device_count);

  if (!device_count) {
    LOG_WARN("[VK] Failed to find GPUs\n");
    return;
  }

  vkEnumeratePhysicalDevices(ctx->instance, &device_count, physical_devices);
  ctx->gpu = malloc(sizeof(VkPhysicalDevice) * device_count);

  for (u32 i = 0; i < device_count; i++) {
    ctx->gpu[i] = physical_devices[i];
  }

  LOG_INFO("[VK] ctx->device_count: %u\n", device_count);
}

void
vulkan_physical_device_create(void)
{
  VkPhysicalDevice   gpu     = ctx->gpu[0];
  QueueFamilyIndices indices = vulkan_queue_families_find(&gpu);

  f32                           queue_priority    = 1.0f;
  const VkDeviceQueueCreateInfo queue_create_info = {
    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext            = NULL,
    .queueFamilyIndex = indices.graphics_family,
    .queueCount       = 1,
    .pQueuePriorities = &queue_priority,
  };

  VkPhysicalDeviceFeatures device_features = { 0 };
  vkGetPhysicalDeviceFeatures(gpu, &device_features);
  LOG_INFO("[VK] features: %u\n", device_features);

  const char *device_extension_names[]  = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  VkDeviceCreateInfo device_create_info = {
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = 0,
    .queueCreateInfoCount    = 1,
    .pQueueCreateInfos       = &queue_create_info,
    .enabledLayerCount       = 0,
    .ppEnabledLayerNames     = NULL,
    .enabledExtensionCount   = ArrayCount(device_extension_names),
    .ppEnabledExtensionNames = device_extension_names,
    .pEnabledFeatures        = &device_features,
  };

  if (ctx->enable_validation_layers) {
    // TODO!
  } else {
    device_create_info.enabledLayerCount = 0;
  }

  vkCreateDevice(gpu, &device_create_info, NULL, &ctx->device);
  LOG_INFO("[VK] ctx->device: %u\n", ctx->device);

  vkGetDeviceQueue(ctx->device, indices.graphics_family, 0,
                   &ctx->graphics_queue);
  LOG_INFO("[VK] ctx->graphics_queue: %u\n", ctx->graphics_queue);
}

void
vulkan_pipeline_create(const String vert_filepath, const String frag_filepath)
{
  String vert_source = os_file_read(vert_filepath);
  String frag_source = os_file_read(frag_filepath);

  LOG_INFO("vert_source_size: %u\n", vert_source.size);
  LOG_INFO("frag_source_size: %u\n", frag_source.size);
}
