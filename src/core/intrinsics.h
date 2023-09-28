#pragma once

#include "core/base.h"

#define type_has_field(type, field)      offsetof(type, field)
#define type_field_type(type, field)     typeof(((type *)0)->field)
#define offset_of_by_string(type, field) ((size_t)(&((type *)0)->field))
