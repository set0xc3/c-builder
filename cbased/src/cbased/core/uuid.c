#include "uuid.h"

api uuid
uuid_gen(void)
{
  uuid result = { 0 };
  char v[]    = { '0', '1', '2', '3', '4', '5', '6', '7',
                  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
  for (u32 i = 0; i < 36; ++i) {
    if (i == 8 || i == 13 || i == 18 || i == 23) {
      // Put a dash
      result.value[i] = '-';
    } else {
      i32 offset      = rand() % 16;
      result.value[i] = v[offset];
    }
  }
  result.value[35] = '\0';
  return result;
}
