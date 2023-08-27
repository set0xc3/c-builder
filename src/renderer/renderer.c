#include "renderer.h"
#include "platform/platform.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define BUFFER_SIZE 16384

static GLfloat tex_buf[BUFFER_SIZE * 8];
static GLfloat vert_buf[BUFFER_SIZE * 8];
static GLubyte color_buf[BUFFER_SIZE * 16];
static GLuint  index_buf[BUFFER_SIZE * 6];

static int width   = 1280;
static int height  = 720;
static int buf_idx = 0;

void
renderer_init(void)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

void
renderer_flush(void)
{
    if (buf_idx == 0)
    {
        return;
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTexCoordPointer(2, GL_FLOAT, 0, tex_buf);
    glVertexPointer(2, GL_FLOAT, 0, vert_buf);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, color_buf);
    glDrawElements(GL_TRIANGLES, buf_idx * 6, GL_UNSIGNED_INT, index_buf);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    buf_idx = 0;
}

void
renderer_draw_rect(Vector4 rect, Vector4 color)
{
    renderer_quad_push(rect, vec4_init(125.0f, 68.0f, 3.0f, 3.0f), color);
}

void
renderer_set_clip_rect(Vector4 rect)
{
    renderer_flush();
    glScissor(rect.x, height - (rect.y + rect.height), rect.width,
              rect.height);
}

void
renderer_clear(Vector4 clr)
{
    renderer_flush();
    glClearColor(clr.r / 255., clr.g / 255., clr.b / 255., clr.a / 255.);
    glClear(GL_COLOR_BUFFER_BIT);
}

void
renderer_present(void)
{
    Window *window = platform_window_root_get();

    renderer_flush();
    SDL_GL_SwapWindow(window->sdl.handle);
}

void
renderer_window_resize(Vector2 rect)
{
    glViewport(0, 0, rect.width, rect.height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, rect.width, rect.height, 0.0f, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void
renderer_quad_push(Vector4 src, Vector4 dst, Vector4 color)
{
    if (buf_idx == BUFFER_SIZE)
    {
        renderer_flush();
    }

    int texvert_idx = buf_idx * 8;
    int color_idx   = buf_idx * 16;
    int element_idx = buf_idx * 4;
    int index_idx   = buf_idx * 6;
    buf_idx++;

    /* update vertex buffer */
    vert_buf[texvert_idx + 0] = src.x;
    vert_buf[texvert_idx + 1] = src.y;
    vert_buf[texvert_idx + 2] = src.x + src.width;
    vert_buf[texvert_idx + 3] = src.y;
    vert_buf[texvert_idx + 4] = src.x;
    vert_buf[texvert_idx + 5] = src.y + src.height;
    vert_buf[texvert_idx + 6] = src.x + src.width;
    vert_buf[texvert_idx + 7] = src.y + src.height;

    /* update color buffer */
    memcpy(color_buf + color_idx + 0, &color, 4);
    memcpy(color_buf + color_idx + 4, &color, 4);
    memcpy(color_buf + color_idx + 8, &color, 4);
    memcpy(color_buf + color_idx + 12, &color, 4);

    /* update index buffer */
    index_buf[index_idx + 0] = element_idx + 0;
    index_buf[index_idx + 1] = element_idx + 1;
    index_buf[index_idx + 2] = element_idx + 2;
    index_buf[index_idx + 3] = element_idx + 2;
    index_buf[index_idx + 4] = element_idx + 3;
    index_buf[index_idx + 5] = element_idx + 1;
}
