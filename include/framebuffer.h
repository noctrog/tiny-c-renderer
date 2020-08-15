#ifndef IMAGE_H_P2AJDWBK
#define IMAGE_H_P2AJDWBK

#include <geometry.h>
#include <color.h>
#include <texture.h>

struct framebuffer;

struct framebuffer* 
renderer_framebuffer_create(int x, int y);

void
renderer_framebuffer_delete(struct framebuffer** fb);

void
renderer_framebuffer_set_pixel(struct framebuffer* fb, const struct color* col, const struct vec2i *p);

void
renderer_framebuffer_set_pixel_z(struct framebuffer *fb, int zval, const struct vec2i *p);

float
renderer_framebuffer_get_pixel_z(const struct framebuffer *fb, const struct vec2i *p);

void
renderer_framebuffer_draw_line(struct framebuffer *fb, struct color *col, 
                               struct vec2i *p0, struct vec2i *p1);

void
renderer_framebuffer_draw_triangle(struct framebuffer *fb, const gm_triangle *tr, 
                                   struct vec3f **u, struct vec2f **uv, 
                                   struct texture *tex);

void
renderer_framebuffer_clear_color(struct framebuffer *fb, const struct color *col);

void
renderer_framebuffer_clear_z(struct framebuffer *fb);

int 
renderer_framebuffer_save(struct framebuffer* fb, char* filename);

#endif /* end of include guard: IMAGE_H_P2AJDWBK */
