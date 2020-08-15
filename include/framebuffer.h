#ifndef IMAGE_H_P2AJDWBK
#define IMAGE_H_P2AJDWBK

#include <geometry.h>
#include <color.h>
#include <texture.h>

struct framebuffer;

struct framebuffer* 
rndr_framebuffer_create(int x, int y);

void
rndr_framebuffer_delete(struct framebuffer** fb);

void
rndr_framebuffer_set_pixel(struct framebuffer* fb, const struct color* col, const struct vec2i *p);

void
rndr_framebuffer_set_pixel_z(struct framebuffer *fb, int zval, const struct vec2i *p);

float
rndr_framebuffer_get_pixel_z(const struct framebuffer *fb, const struct vec2i *p);

void
rndr_framebuffer_draw_line(struct framebuffer *fb, struct color *col, 
                               struct vec2i *p0, struct vec2i *p1);

void
rndr_framebuffer_draw_triangle(struct framebuffer *fb, const gm_triangle *tr, 
                               struct vec3f **u, struct vec2f **uv, 
                               struct vec3f **vn, struct texture *tex);

void
rndr_framebuffer_clear_color(struct framebuffer *fb, const struct color *col);

void
rndr_framebuffer_clear_z(struct framebuffer *fb);

int 
rndr_framebuffer_save(struct framebuffer* fb, char* filename);

#endif /* end of include guard: IMAGE_H_P2AJDWBK */
