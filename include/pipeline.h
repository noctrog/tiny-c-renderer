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

typedef vec3 *triangle_vertices;
typedef vec2 *triangle_tex_coords;
typedef vec3 *triangle_normals;

typedef struct texture *tex_array[N_MAX_TEXTURES];

struct pipeline_data;

/* --- Error handling --- */
enum PipelineError {
    PIPELINE_OK = 0,
    PIPELINE_ERROR,
    PIPELINE_INVALID_DATA,
    PIPELINE_INVALID_FB,
    PIPELINE_INVALID_RANGE
};

/* --- Shaders --- */
typedef enum PipelineError (*vertex_shader)(struct pipeline_data *data);
typedef enum PipelineError (*fragment_shader)(struct pipeline_data *data);

/* --- Pipeline functions --- */

/**
 *  \brief Sets the number of faces of the Pipeline Data
 *  \param data: Pipeline Data structure
 *  \param n_faces: Number of faces to draw
 *  \return void
 */
enum PipelineError
rndr_pipeline_data_set_n_faces(struct pipeline_data *data, size_t n_faces);

/**
 *  \brief Sets internal pipeline_data vertices pointer to verts.
 *
 *  This does not copy the contents of verts, only assigns the pointer.
 *
 *  \param data: Pipeline data structure.
 *  \param verts: Pointer to vector of verts.
 *  \return void
 */
enum PipelineError
rndr_pipeline_data_set_vertices(struct pipeline_data *data, triangle_vertices verts,
				size_t n_vers);

/**
 *  \brief Sets internal pipeline_data tex coordinates pointer to tc.
 *
 *  This does not copy the contents of tc, only assigns the pointer.
 *
 *  \param data: Pipeline data structure.
 *  \param verts: Pointer to vector of tex coordinates.
 *  \return void
 */
enum PipelineError
rndr_pipeline_data_set_tex_coords(struct pipeline_data *data, triangle_tex_coords tc);

/**
 *  \brief Sets internal pipeline_data normal pointer to n.
 *
 *  This does not copy the contents of n, only assigns the pointer.
 *
 *  \param data: Pipeline data structure.
 *  \param verts: Pointer to vector of normals.
 *  \return void
 */
enum PipelineError
rndr_pipeline_data_set_normals(struct pipeline_data *data, triangle_normals n);

/**
 *  \brief Sets internal texture at nth slot.
 *
 *  This does not copy the texture, only assigns a pointer.
 *
 *  \param data: Pipeline data structure.
 *  \param t: texture to add to the pipeline data.
 *  \param n: texture slot.
 *  \return void
 */
enum PipelineError
rndr_pipeline_data_set_texture(struct pipeline_data *data, struct texture *t, size_t n);

/**
 *  \brief Sets the Model-View-Projection matrix.
 *  \param data: Pipeline data structure.
 *  \param mvp: MVP matrix.
 *  \return void
 */
enum PipelineError
rndr_pipeline_data_set_mvp(struct pipeline_data *data, mat4 mvp);

/**
 *  \brief Sets the current framebuffer on which to draw on.
 *  \param data: Pipeline data structure.
 *  \param fb: Pointer to framebuffer.
 *  \return PIPELINE_INVALID_FB if framebuffer is invalid, PIPELINE_OK otherwise.
 */
enum PipelineError
rndr_pipeline_data_set_fb(struct pipeline_data *data, struct framebuffer *fb);

/**
 *  \brief Returns the number of faces saved in Pipeline data.
 *  \param data: Pipeline data structure.
 *  \param n_faces: Stores the result.
 *  \return PIPELINE_INVALID_DATA if data pointer or n_faces pointer is invalid. PIPELINE_OK otherwise.
 */
enum PipelineError
rndr_pipeline_data_get_n_faces(struct pipeline_data *data, size_t *n_faces);

/**
 *  \brief Returns the Model-View-Projection Matrix
 *
 *  This does not perform a copy.
 *
 *  \param data: Pipeline data structure.
 *  \param mvp: Stores the result.
 *  \return PIPELINE_INVALID_DATA if data is invalid, PIPELINE_OK if everything went well.
 */
enum PipelineError
rndr_pipeline_data_get_mvp(struct pipeline_data *data, mat4 mvp);

/**
 *  \brief Returns a pointer to the texture in the specified slot
 *  \param data: Pipeline data structure
 *  \param slot: number to the wanted slot.
 *  \param tex: pointer to a pointer to a texture. Stores the result.
 *  \return PIPELINE_INVALID_DATA if data pointer is invalid, PIPELINE_INVALID_RANGE if the slot specified is invalid or PIPELINE_OK if successful
 */
enum PipelineError
rndr_pipeline_data_get_texture(struct pipeline_data *data, size_t slot,
			       struct texture **tex);

/**
 *  \brief Returns the number of vertices of the vertices arrays
 *  \param data: pointer to Pipeline data structure.
 *  \param n_verts: pointer to the size_t to contain the number of vertices.
 *  \return PIPELINE_INVALID_DATA if data is not properly initialized. PIPELINE_OK if execution completed succesfully.
 */
enum PipelineError
rndr_pipeline_data_get_n_verts(struct pipeline_data *data, size_t *n_verts);

enum PipelineError
rndr_pipeline_draw_triangles(struct pipeline_data *data,
			     vertex_shader vs,
			     fragment_shader fs);

#endif /* PIPELINE_H */
