#pragma once

#include "core/base.h"
#include "core/os.h"

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

typedef struct PipelineContext {
  VkPipeline     graphics_pipeline;
  VkShaderModule vert_shader_module;
  VkShaderModule frag_shader_module;
} PipelineContext;

typedef struct PipelineBuilder {
  VkPipelineShaderStageCreateInfo *shader_stages;
  u64                              shader_stage_count;

  VkPipelineVertexInputStateCreateInfo   vertex_input_info;
  VkPipelineInputAssemblyStateCreateInfo input_assembly;
  VkViewport                             viewport;
  VkRect2D                               scissor;
  VkPipelineRasterizationStateCreateInfo rasterizer;
  VkPipelineColorBlendAttachmentState    color_blend_attachment;
  VkPipelineMultisampleStateCreateInfo   multisampling;
  VkPipelineLayout                       pipeline_layout;

} PipelineBuilder;

typedef struct VulkanContext {
  VkDebugUtilsMessengerEXT debug_messenger;

  VkInstance                 instance;
  VkDevice                   device;
  VkSurfaceKHR               surface;
  VkQueue                    graphics_queue;
  VkQueue                    present_queue;
  VkPhysicalDevice           gpu;
  VkPhysicalDeviceProperties properties;

  VkPipeline       triangle_pipeline;
  VkPipelineLayout triangle_pipeline_layout;
} VulkanContext;

QueueFamilyIndices find_queue_families(VkPhysicalDevice _device);

b32 get_required_extensions(const char **out_extension_names,
                            u32         *out_extension_count);
b32 check_validation_layer_support(void);
b32 check_device_extension_support(VkPhysicalDevice device);

b32 debug_messenger_init(void);

void debug_messenger_information_create(
    VkDebugUtilsMessengerCreateInfoEXT *create_info);

VkResult debug_utils_messenger_ext_create(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT    *pDebugMessenger);

void
debug_utils_messenger_ext_destroy(VkInstance                   instance,
                                  VkDebugUtilsMessengerEXT     debugMessenger,
                                  const VkAllocationCallbacks *pAllocator);

b32 device_suitable_is_valid(VkPhysicalDevice device);

SwapChainSupportDetails query_swapchain_support(VkPhysicalDevice device);

API void vulkan_init(void);
API void vulkan_destroy(void);

API b32 vulkan_instance_create(void);

API b32 vulkan_surface_create(void);

API void vulkan_physical_device_pick(void);
API void vulkan_physical_device_create(void);

API void vulkan_pipeline_create(const String vert_filepath,
                                const String frag_filepath);
API b32  vulkan_pipeline_shader_create(const String    file_source,
                                       VkShaderModule *shader_module);

API VkPipeline vulkan_pipeline_builder(PipelineBuilder *self, VkDevice device,
                                       VkRenderPass pass);

API VkPipelineShaderStageCreateInfo vulkan_pipeline_shader_stage_create_info(
    VkShaderStageFlagBits stage, VkShaderModule shader_module);

API VkPipelineVertexInputStateCreateInfo
vulkan_vertex_input_state_create_info(void);

API VkPipelineInputAssemblyStateCreateInfo
vulkan_input_assembly_create_info(VkPrimitiveTopology topology);

API VkPipelineRasterizationStateCreateInfo
vulkan_rasterization_state_create_info(VkPolygonMode polygon_mode);

API VkPipelineMultisampleStateCreateInfo
vulkan_multisampling_state_create_info(void);

API VkPipelineColorBlendAttachmentState
vulkan_color_blend_attachment_state(void);

API VkPipelineLayoutCreateInfo vulkan_pipeline_layout_create_info(void);
