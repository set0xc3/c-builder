#include "vulkan.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vulkan/vulkan.h>

static const char *validation_layers[] = {
  "VK_LAYER_KHRONOS_validation",
};

static const char *device_extension_names[] = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifndef NDEBUG
static b32 enable_validation_layers = true;
#else
static b32 enable_validation_layers = false;
#endif

static VulkanContext *ctx;

VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT             messageType,
               const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
               void                                       *pUserData)
{
  LOG_DEBUG("[VK] validation layer: %s\n", pCallbackData->pMessage);

  return VK_FALSE;
}

QueueFamilyIndices
find_queue_families(VkPhysicalDevice _device)
{
  QueueFamilyIndices indices = { 0 };
  VkPhysicalDevice   device  = _device;

  VkQueueFamilyProperties *queue_families     = NULL;
  u32                      queue_family_count = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);

  queue_families = calloc(queue_family_count, sizeof(VkQueueFamilyProperties));

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count,
                                           queue_families);

  b32 graphics_family_has_value = false;
  b32 present_family_has_value  = false;

  for (u32 i = 0; i < queue_family_count; i++) {
    if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphics_family_index = i;
      graphics_family_has_value     = true;
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, ctx->surface,
                                         &present_support);

    if (present_support) {
      indices.present_family_index = i;
      present_family_has_value     = true;
    }

    if (graphics_family_has_value && present_family_has_value) {
      indices.is_complete = true;
      break;
    }
  }

  free(queue_families);

  return indices;
}

b32
get_required_extensions(const char **out_extension_names,
                        u32         *out_extension_count)
{
  OS_Window *window = os_window_root_get();

  SDL_Vulkan_GetInstanceExtensions(window->sdl.window, out_extension_count,
                                   NULL);

  if (enable_validation_layers) {
    SDL_Vulkan_GetInstanceExtensions(window->sdl.window, out_extension_count,
                                     out_extension_names);

    *out_extension_count += 1;

    if (*out_extension_count > VK_MAX_EXTENSION_NAME_SIZE) {
      return false;
    }

    static char *str_ext = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    out_extension_names[*out_extension_count - 1] = str_ext;
  } else {
    SDL_Vulkan_GetInstanceExtensions(window->sdl.window, out_extension_count,
                                     out_extension_names);
  }

  return true;
}

b32
check_validation_layer_support(void)
{
  u32 available_layer_count = 0;
  vkEnumerateInstanceLayerProperties(&available_layer_count, NULL);

  VkLayerProperties *available_layers
      = calloc(available_layer_count, sizeof(VkLayerProperties));
  vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers);

  for (u32 i = 0; i < ArrayCount(validation_layers); i++) {
    b32         layer_found = false;
    const char *layer_name  = validation_layers[i];

    for (u32 j = 0; j < available_layer_count; j++) {
      if (strcmp(layer_name, available_layers[j].layerName) == 0) {
        layer_found = true;
        break;
      }
    }

    if (!layer_found) {
      return false;
    }
  }

  return true;
}

b32
check_device_extension_support(VkPhysicalDevice device)
{
  u32 extension_count = 0;
  vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);

  VkExtensionProperties *available_extensions
      = calloc(extension_count, sizeof(VkExtensionProperties));
  vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count,
                                       available_extensions);

  u32 found_count = 0;
  for (u32 i = 0; i < extension_count; i++) {
    const char *extension_name = available_extensions[i].extensionName;

    for (u32 j = 0; j < ArrayCount(device_extension_names); j++) {
      if (strcmp(extension_name, device_extension_names[j]) == 0) {
        found_count++;
      }
    }
  }

  if (found_count != ArrayCount(device_extension_names)) {
    return false;
  }

  return true;
}

b32
debug_messenger_init(void)
{
  if (!enable_validation_layers) {
    return false;
  }

  VkDebugUtilsMessengerCreateInfoEXT create_info = { 0 };
  debug_messenger_information_create(&create_info);

  if (debug_utils_messenger_ext_create(ctx->instance, &create_info, NULL,
                                       &ctx->debug_messenger)
      != VK_SUCCESS) {
    LOG_ERR("[VK] Failed to set up debug messenger\n");
    return false;
  }

  return true;
}

void
debug_messenger_information_create(
    VkDebugUtilsMessengerCreateInfoEXT *create_info)
{
  create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info->messageSeverity
      = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info->pfnUserCallback = debug_callback;
}

VkResult
debug_utils_messenger_ext_create(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT    *pDebugMessenger)
{
  PFN_vkCreateDebugUtilsMessengerEXT func
      = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkCreateDebugUtilsMessengerEXT");

  if (func != NULL) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void
debug_utils_messenger_ext_destroy(VkInstance                   instance,
                                  VkDebugUtilsMessengerEXT     debugMessenger,
                                  const VkAllocationCallbacks *pAllocator)
{
  PFN_vkDestroyDebugUtilsMessengerEXT func
      = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkDestroyDebugUtilsMessengerEXT");

  if (func != NULL) {
    func(instance, debugMessenger, pAllocator);
  }
}

b32
device_suitable_is_valid(VkPhysicalDevice device)
{
  QueueFamilyIndices indices = find_queue_families(device);
  b32 extensions_supported   = check_device_extension_support(device);
  b32 swap_chain_adequate    = false;

  if (extensions_supported) {
    SwapChainSupportDetails swap_chain_support
        = query_swapchain_support(device);
    swap_chain_adequate = !swap_chain_support.format_count
                          && !swap_chain_support.present_mode_count;
  }

  VkPhysicalDeviceFeatures supported_features = { 0 };
  vkGetPhysicalDeviceFeatures(device, &supported_features);

  return indices.is_complete && extensions_supported && swap_chain_adequate
         && supported_features.samplerAnisotropy;
}

void
vulkan_init(void)
{
  ctx = calloc(1, sizeof(VulkanContext));

  if (SDL_Vulkan_LoadLibrary(NULL) != 0) {
    LOG_ERR("[VK] Failed load library: %s\n", SDL_GetError());
  }
}

SwapChainSupportDetails
query_swapchain_support(VkPhysicalDevice device)
{
  SwapChainSupportDetails details = { 0 };
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, ctx->surface,
                                            &details.capabilities);

  u32 format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, ctx->surface, &format_count,
                                       NULL);
  if (format_count != 0) {
    details.format_list = calloc(format_count, sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device, ctx->surface, &details.format_count, details.format_list);
  }

  u32 present_mode_count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, ctx->surface,
                                            &present_mode_count, NULL);
  if (present_mode_count != 0) {
    details.present_mode_list
        = calloc(present_mode_count, sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, ctx->surface,
                                              &details.present_mode_count,
                                              details.present_mode_list);
  }

  return details;
}

void
vulkan_destroy(void)
{
  vkDestroyDevice(ctx->device, NULL);

  if (enable_validation_layers) {
    debug_utils_messenger_ext_destroy(ctx->instance, ctx->debug_messenger,
                                      NULL);
  }

  vkDestroyInstance(ctx->instance, NULL);
}

b32
vulkan_instance_create(void)
{
  OS_Window *window = os_window_root_get();

  if (enable_validation_layers && !check_validation_layer_support()) {
    LOG_WARN("Validation layers not supported\n");
    enable_validation_layers = false;
  }

  const VkApplicationInfo app_info = {
    .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext              = NULL,
    .pApplicationName   = "NoAppName",
    .applicationVersion = 0,
    .pEngineName        = "NoEngineName",
    .engineVersion      = 0,
    .apiVersion         = VK_API_VERSION_1_0,
  };

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { 0 };

  VkInstanceCreateInfo create_info = {
    .sType                 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext                 = NULL,
    .flags                 = 0,
    .pApplicationInfo      = &app_info,
    .enabledLayerCount     = 0,
    .ppEnabledLayerNames   = NULL,
    .enabledExtensionCount = 0,
  };

  const char *extension_names[VK_MAX_EXTENSION_NAME_SIZE] = { 0 };
  u32         extension_count                             = 0;

  if (get_required_extensions(extension_names, &extension_count)) {
    create_info.ppEnabledExtensionNames = extension_names;
    create_info.enabledExtensionCount   = extension_count;
  } else {
    LOG_ERR("[VK] Failed get_required_extensions\n");
    return false;
  }

  if (enable_validation_layers) {
    create_info.pNext
        = (VkDebugUtilsMessengerCreateInfoEXT *)&debug_create_info;
    create_info.ppEnabledLayerNames = validation_layers;
    create_info.enabledLayerCount   = ArrayCount(validation_layers);
    debug_messenger_information_create(&debug_create_info);
  }

  if (vkCreateInstance(&create_info, NULL, &ctx->instance) != VK_SUCCESS) {
    LOG_ERR("[VK] Failed to create instance\n");
    return false;
  }

  return true;
}

b32
vulkan_surface_create(void)
{
  OS_Window *window = os_window_root_get();
  if (SDL_Vulkan_CreateSurface(window->sdl.window, ctx->instance,
                               &ctx->surface)
      == SDL_FALSE) {
    return false;
  }

  return true;
}

void
vulkan_physical_device_pick(void)
{
  VkPhysicalDevice *physical_device_list = NULL;
  u32               device_count         = 0;

  vkEnumeratePhysicalDevices(ctx->instance, &device_count, NULL);
  if (device_count == 0) {
    LOG_WARN("[VK] Failed to find GPUs\n");
    return;
  }
  physical_device_list = calloc(device_count, sizeof(VkPhysicalDevice));

  vkEnumeratePhysicalDevices(ctx->instance, &device_count,
                             physical_device_list);

  for (u32 i = 0; i < device_count; i++) {
    if (device_suitable_is_valid(physical_device_list[i])) {
      ctx->gpu = physical_device_list[0];
      vkGetPhysicalDeviceProperties(ctx->gpu, &ctx->properties);
      break;
    }
  }
}

void
vulkan_physical_device_create(void)
{
  QueueFamilyIndices indices = find_queue_families(ctx->gpu);

  uint32_t unique_queue_families[] = {
    indices.graphics_family_index,
    // indices.present_family_index, // TODO: is not unique
  };
  u32 unique_queue_familie_count = ArrayCount(unique_queue_families);

  VkDeviceQueueCreateInfo *queue_create_infos
      = calloc(unique_queue_familie_count, sizeof(VkDeviceQueueCreateInfo));

  f32 queue_priority = 1.0f;
  for (u32 i = 0; i < unique_queue_familie_count; i++) {
    uint32_t                queueFamily       = unique_queue_families[i];
    VkDeviceQueueCreateInfo queue_create_info = { 0 };
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queueFamily;
    queue_create_info.queueCount       = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos[i]              = queue_create_info;
  }

  VkPhysicalDeviceFeatures device_features = { 0 };
  vkGetPhysicalDeviceFeatures(ctx->gpu, &device_features);

  VkDeviceCreateInfo device_create_info = {
    .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext                   = NULL,
    .flags                   = 0,
    .queueCreateInfoCount    = unique_queue_familie_count,
    .pQueueCreateInfos       = queue_create_infos,
    .enabledLayerCount       = 0,
    .ppEnabledLayerNames     = NULL,
    .enabledExtensionCount   = ArrayCount(device_extension_names),
    .ppEnabledExtensionNames = device_extension_names,
    .pEnabledFeatures        = &device_features,
  };

  if (enable_validation_layers) {
    device_create_info.ppEnabledLayerNames = validation_layers;
    device_create_info.enabledLayerCount   = ArrayCount(validation_layers);
  } else {
    device_create_info.enabledLayerCount = 0;
  }

  vkCreateDevice(ctx->gpu, &device_create_info, NULL, &ctx->device);

  vkGetDeviceQueue(ctx->device, indices.graphics_family_index, 0,
                   &ctx->graphics_queue);
}

void
vulkan_pipeline_create(const String vert_filepath, const String frag_filepath)
{
  String vert_source = os_file_read(vert_filepath);
  String frag_source = os_file_read(frag_filepath);

  VkShaderModule vert_shader_module = NULL;
  if (!vulkan_pipeline_shader_create(vert_source, &vert_shader_module)) {
    LOG_ERR("[VK] Failed to create vert_shader_module\n");
  }

  VkShaderModule frag_shader_module = NULL;
  if (!vulkan_pipeline_shader_create(frag_source, &frag_shader_module)) {
    LOG_ERR("[VK] Failed to create frag_shader_module\n");
  }

  VkPipelineLayoutCreateInfo pipeline_layout_info
      = vulkan_pipeline_layout_create_info();
}

b32
vulkan_pipeline_shader_create(const String    file_source,
                              VkShaderModule *shader_module)
{
  VkShaderModuleCreateInfo create_info = {
    .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext    = NULL,
    .flags    = 0,
    .codeSize = file_source.size,
    .pCode    = (const u32 *)file_source.data,
  };

  if (vkCreateShaderModule(ctx->device, &create_info, NULL, shader_module)
      != VK_SUCCESS) {
    return false;
  }

  return true;
}

VkPipeline
vulkan_pipeline_builder(PipelineBuilder *self, VkDevice device,
                        VkRenderPass pass)
{
  VkPipelineViewportStateCreateInfo viewportState = { 0 };
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.pNext = NULL;

  viewportState.viewportCount = 1;
  viewportState.pViewports    = &self->viewport;
  viewportState.scissorCount  = 1;
  viewportState.pScissors     = &self->scissor;

  VkPipelineColorBlendStateCreateInfo colorBlending = { 0 };
  colorBlending.sType
      = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.pNext = NULL;

  colorBlending.logicOpEnable   = VK_FALSE;
  colorBlending.logicOp         = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments    = &self->color_blend_attachment;

  VkGraphicsPipelineCreateInfo pipelineInfo = { 0 };
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.pNext = NULL;

  pipelineInfo.stageCount          = self->shader_stage_count;
  pipelineInfo.pStages             = self->shader_stages;
  pipelineInfo.pVertexInputState   = &self->vertex_input_info;
  pipelineInfo.pInputAssemblyState = &self->input_assembly;
  pipelineInfo.pViewportState      = &viewportState;
  pipelineInfo.pRasterizationState = &self->rasterizer;
  pipelineInfo.pMultisampleState   = &self->multisampling;
  pipelineInfo.pColorBlendState    = &colorBlending;
  pipelineInfo.layout              = self->pipeline_layout;
  pipelineInfo.renderPass          = pass;
  pipelineInfo.subpass             = 0;
  pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;

  VkPipeline newPipeline;
  if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL,
                                &newPipeline)
      != VK_SUCCESS) {
    LOG_ERR("failed to create pipeline\n");
    return VK_NULL_HANDLE;
  } else {
    return newPipeline;
  }
}

VkPipelineShaderStageCreateInfo
vulkan_pipeline_shader_stage_create_info(VkShaderStageFlagBits stage,
                                         VkShaderModule        shader_module)
{
  VkPipelineShaderStageCreateInfo info = { 0 };
  info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  info.pNext  = NULL;
  info.stage  = stage;
  info.module = shader_module;
  info.pName  = "main";
  return info;
}

VkPipelineVertexInputStateCreateInfo
vulkan_vertex_input_state_create_info(void)
{
  VkPipelineVertexInputStateCreateInfo info = { 0 };
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  info.pNext = NULL;
  info.vertexBindingDescriptionCount   = 0;
  info.vertexAttributeDescriptionCount = 0;
  return info;
}

VkPipelineInputAssemblyStateCreateInfo
vulkan_input_assembly_create_info(VkPrimitiveTopology topology)
{
  VkPipelineInputAssemblyStateCreateInfo info = { 0 };
  info.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  info.pNext    = NULL;
  info.topology = topology;
  info.primitiveRestartEnable = VK_FALSE;
  return info;
}

VkPipelineRasterizationStateCreateInfo
vulkan_rasterization_state_create_info(VkPolygonMode polygon_mode)
{
  VkPipelineRasterizationStateCreateInfo info = { 0 };
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  info.pNext = NULL;
  info.depthClampEnable        = VK_FALSE;
  info.rasterizerDiscardEnable = VK_FALSE;
  info.polygonMode             = polygon_mode;
  info.lineWidth               = 1.0f;
  info.cullMode                = VK_CULL_MODE_NONE;
  info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
  info.depthBiasEnable         = VK_FALSE;
  info.depthBiasConstantFactor = 0.0f;
  info.depthBiasClamp          = 0.0f;
  info.depthBiasSlopeFactor    = 0.0f;
  return info;
}

VkPipelineMultisampleStateCreateInfo
vulkan_multisampling_state_create_info(void)
{
  VkPipelineMultisampleStateCreateInfo info = { 0 };
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  info.pNext = NULL;
  info.sampleShadingEnable   = VK_FALSE;
  info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
  info.minSampleShading      = 1.0f;
  info.pSampleMask           = NULL;
  info.alphaToCoverageEnable = VK_FALSE;
  info.alphaToOneEnable      = VK_FALSE;
  return info;
}

VkPipelineLayoutCreateInfo
vulkan_pipeline_layout_create_info(void)
{
  VkPipelineLayoutCreateInfo info = { 0 };
  info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  info.pNext                  = NULL;
  info.flags                  = 0;
  info.setLayoutCount         = 0;
  info.pSetLayouts            = NULL;
  info.pushConstantRangeCount = 0;
  info.pPushConstantRanges    = NULL;
  return info;
}
