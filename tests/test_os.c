#include <cbased.h>

#include <unity.h>

void
setUp(void)
{
}

void
tearDown(void)
{
}

void
test_os(void)
{
  TEST_ASSERT(os_init(false));
  TEST_ASSERT(os_destroy());
}
