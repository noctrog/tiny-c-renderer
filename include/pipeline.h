/**
 *   \file pipeline.h
 *   \brief Contains declarations of functions related to the rendering pipeline.
 */

#ifndef PIPELINE_H
#define PIPELINE_H

#include <geometry.h>
#include <texture.h>

#define N_MAX_TEXTURES 16

struct framebuffer;

/* --- Pipeline data --- */

typedef vec3 *triangle_vertices[3];
typedef vec2 *triangle_tex_coords[3];
typedef vec3 *triangle_normals[3];

typedef struct texture *tex_array[N_MAX_TEXTURES];

struct pipeline_data {
    size_t n_triangles;         /* Stores the total number of triangles to draw */
    triangle_vertices u;        /* 3D vertices of triangles */
    triangle_tex_coords uv;     /* Texture coordinates of triangles */
    triangle_normals vn;        /* Normals of triangles */
    tex_array textures;         /* 2D Textures */

    mat4 mvp;     /* Model-View-Projection Matrix */
};

/* --- Error handling --- */
enum PipelineError {
    PIPELINE_OK = 0,
    ERROR
};

/* --- Shaders --- */
typedef enum PipelineError (*vertex_shader)(triangle_vertices u,
					    const mat4 mvp);
typedef enum PipelineError (*fragment_shader)(struct framebuffer *fb,
                                              triangle_vertices u,
                                              const triangle_tex_coords uv,
                                              const triangle_normals vn,
                                              const tex_array textures);

/* --- Pipeline functions --- */
enum PipelineError
rndr_pipeline_draw_triangles(struct framebuffer *fb,
			     const struct pipeline_data *data,
			     vertex_shader vs,
			     fragment_shader fs);

#endif /* PIPELINE_H */
