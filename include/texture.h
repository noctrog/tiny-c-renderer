#ifndef TEXTURE_H_16JPB9UM
#define TEXTURE_H_16JPB9UM

#include <geometry.h>
#include <color.h>

struct texture;

struct texture *
rndr_texture_create(const char *filename);

void
rndr_texture_delete(struct texture **tex);

struct color *
rndr_texture_get_color(struct texture *tex, struct vec2i *p);

int
rndr_texture_get_width(struct texture *tex);

int
rndr_texture_get_height(struct texture *tex);

#endif /* end of include guard: TEXTURE_H_16JPB9UM */
