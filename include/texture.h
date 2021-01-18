#ifndef TEXTURE_H_16JPB9UM
#define TEXTURE_H_16JPB9UM

#include <geometry.h>
#include <color.h>

struct texture;

/**
 *  \brief Create and allocate memory for a texture, loaded from an image file.
 *  \param filename: path to an image.
 *  \return Pointer to the newly created texture
 */
struct texture *
rndr_texture_create(const char *filename);

/**
 *  \brief Delete and free a texture's memory
 *  \param tex: pointer to a pointer to an existing texture
 *  \return void
 */
void
rndr_texture_delete(struct texture **tex);

/**
 *  \brief Get the texture color of a pixel
 *  \param tex: pointer to a texture
 *  \param p: pointer to a vec2i indicating pixel position
 *  \return pointer to the corresponding color
 */
struct color *
rndr_texture_get_color(struct texture *tex, struct vec2i *p);

/**
 *  \brief Returns image width
 *  \param tex: pointer to a texture
 *  \return Width in pixels
 */
int
rndr_texture_get_width(struct texture *tex);

/**
 *  \brief Returns image height
 *  \param tex: pointer to a texture
 *  \return Height in pixels
 */
int
rndr_texture_get_height(struct texture *tex);

#endif /* end of include guard: TEXTURE_H_16JPB9UM */
