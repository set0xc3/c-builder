#pragma once

#include "core/defines.h"
#include "core/vector.h"

API void renderer_startup(void);
API void renderer_flush(void);
API void renderer_draw_rect(Vector4 rect, Vector4 color);
API void renderer_draw_text(const char *text, Vector2 pos, Vector4 color);
API void renderer_draw_icon(i32 id, Vector4 rect, Vector4 color);
API void renderer_set_clip_rect(Vector4 rect);
API void renderer_clear(Vector4 color);
API void renderer_present(void);
API void renderer_window_resize(Vector2 rect);

API void renderer_quad_push(Vector4 src, Vector4 dst, Vector4 color);
