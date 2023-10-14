#include <cbased.h>

b32
test_get_extensions(void)
{
  const char *extension_names[VK_MAX_EXTENSION_NAME_SIZE] = { 0 };
  u32         extension_count                             = 0;

  if (!get_required_extensions(extension_names, &extension_count)) {
    LOG_ERR("[TEST] Failed get required extensions\n");
    return false;
  }

  return true;
}

int
main(void)
{
  os_init(false);

  if (!test_get_extensions()) {
    return 0;
  }
  LOG_INFO("[TEST] [OK] test_get_extensions\n");

  if (!check_validation_layer_support()) {
    return 0;
  }
  LOG_INFO("[TEST] [OK] check_validation_layer_support\n");

  vulkan_init();
  vulkan_instance_create();

  debug_messenger_init();

  vulkan_surface_create();
  vulkan_physical_device_pick();
  vulkan_physical_device_create();
  vulkan_pipeline_create(str_lit("build/assets/shaders/simple.vert.spv"),
                         str_lit("build/assets/shaders/simple.frag.spv"));

  os_destroy();

  return 0;
}
