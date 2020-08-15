#ifndef TEXTURE_H_16JPB9UM
#define TEXTURE_H_16JPB9UM

#include <geometry.h>
#include <color.h>

struct texture;

struct texture *
renderer_texture_create(const char *filename);

void
renderer_texture_delete(struct texture **tex);

struct color *
renderer_texture_get_color(struct texture *tex, struct vec2i *p);

int
renderer_texture_get_width(struct texture *tex);

int
renderer_texture_get_height(struct texture *tex);

#endif /* end of include guard: TEXTURE_H_16JPB9UM */
