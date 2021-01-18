/**
 *   \file framebuffer.h
 *   \brief Renderer's framebuffer functions.
 */


#ifndef IMAGE_H_P2AJDWBK
#define IMAGE_H_P2AJDWBK

#include <geometry.h>
#include <color.h>
#include <texture.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>

struct framebuffer;

/**
 *  \brief Creates and allocates memory for a framebuffer
 *  \param x: framebuffer's width
 *  \param y: framebuffer's height
 *  \return Pointer to the new framebuffer (has to be freed)
 */
struct framebuffer* 
rndr_framebuffer_create(int x, int y);

/**
 *  \brief Destroys and frees a framebuffer
 *  \param fb: pointer to a pointer to a framebuffer
 *  \return void
 */
void
rndr_framebuffer_delete(struct framebuffer **fb);

/**
 *  \brief Sets a pixel of the framebuffer to a color.
 *  \param fb: pointer to a framebuffer.
 *  \param col: pointer to the color to apply.
 *  \param p: pointer to the position of the pixel to change.
 *  \return void
 */
void
rndr_framebuffer_set_pixel(struct framebuffer *fb, const struct color *col,
			   const struct vec2i *p);

/**
 *  \brief Set value of the depth buffer
 *  \param fb: pointer to the framebuffer
 *  \param zval: zvalue (int)
 *  \param p: pointer to the position of the vertex
 *  \return void
 */
// TODO zval con float
void
rndr_framebuffer_set_pixel_z(struct framebuffer *fb, int zval, const struct vec2i *p);

/**
 *  \brief Retrieve value from zbuffer
 *  \param fb: pointer to the framebuffer
 *  \return value of the zbuffer at p
 */
float
rndr_framebuffer_get_pixel_z(const struct framebuffer *fb, const struct vec2i *p);

/**
 *  \brief Draw a colored line between two points
 *  \param fb: pointer to the framebuffer.
 *  \param col: pointer to the color.
 *  \param p0: one point of the segment.
 *  \param p1: the other point of the segment.
 *  \return void
 */
void
rndr_framebuffer_draw_line(struct framebuffer *fb, struct color *col, 
                           struct vec2i *p0, struct vec2i *p1);

/**
 *  \brief Draws a triangle with texture on the framebuffer
 *  \param TODO
 *  \return void
 */
void
rndr_framebuffer_draw_triangle(struct framebuffer *fb, 
                               const gm_triangle *tr, 
                               vec3 *u[3],
                               vec2 *uv[3],
                               vec3 *vn[3],
                               struct texture *tex);

/**
 *  \brief Clear the color buffer of the framebuffer
 *  \param fb: pointer to a framebuffer
 *  \param col: pointer to the color used to clean the framebuffer.
 *  \return void
 */
void
rndr_framebuffer_clear_color(struct framebuffer *fb, const struct color *col);

/**
 *  \brief Clear the z buffer
 *  \param fb: pointer to the framebuffer
 *  \return void
 */
void
rndr_framebuffer_clear_z(struct framebuffer *fb);

/**
 *  \brief Save a framebuffer as an image
 *  \param fb: pointer to a framebuffer
 *  \param filename: path to the image
 *  \return -1 on failure, 0 on success
 */
int 
rndr_framebuffer_save(struct framebuffer *fb, char *filename);

#endif /* end of include guard: IMAGE_H_P2AJDWBK */
