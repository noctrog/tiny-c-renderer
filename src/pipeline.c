#include <string.h>
#include <pipeline.h>
#include <framebuffer.h>
#include <cglm/cglm.h>

struct pipeline_data {
    /* Input data */
    size_t n_triangles;         /* Stores the total number of triangles to draw */

    triangle_vertices u;        /* 3D vertices of triangles */
    triangle_tex_coords uv;     /* Texture coordinates of triangles */
    triangle_normals vn;        /* Normals of triangles */
    size_t n_verts;             /* Number of vertices */

    tex_array textures;         /* 2D Textures */

    mat4 mvp;     /* Model-View-Projection Matrix */

    struct framebuffer *fb;     /* Current framebuffer to draw on */

    /* Intermediate auxiliar data data */
    triangle_vertices u_aux;
    triangle_normals vn_aux;
};

/* --- Private functions declaration ---  */
/**
 *  \brief Allocate memory for auxiliary memory for the pipeline's execution
 *
 *  This function should only be used by the library itself.
 *
 *  \param data: Pipeline data structure 
 *  \return void
 */
static void
rndr_pipeline_data_create_temp(struct pipeline_data *data);
/**
 *  \brief Free temporary memory used for the pipeline's execution
 *
 *  This functions should only be used by the library itself.
 *
 *  \param data: Pipeline data structure
 *  \return void
 */
static void
rndr_pipeline_data_free_temp(struct pipeline_data *data);    

/* --- Public functions --- */
enum PipelineError
rndr_pipeline_data_set_n_faces(struct pipeline_data *data, size_t n_faces)
{
    if (!data) return PIPELINE_INVALID_DATA;

    data->n_triangles = n_faces;
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_set_vertices(struct pipeline_data *data, triangle_vertices verts,
				size_t n_verts)
{
    if (!data || !verts) return PIPELINE_INVALID_DATA;

    data->u = verts;
    data->n_verts = n_verts;

    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_set_tex_coords(struct pipeline_data *data, triangle_tex_coords tc)
{
    if (!data || !tc) return PIPELINE_INVALID_DATA;

    data->uv = tc;
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_set_normals(struct pipeline_data *data, triangle_normals n)
{
    if (!data || !n) return PIPELINE_INVALID_DATA;

    data->vn = n;
    return PIPELINE_OK;
}


enum PipelineError
rndr_pipeline_data_set_texture(struct pipeline_data *data, struct texture *t, size_t n)
{
    if (!data || !t) return PIPELINE_INVALID_DATA;
    if (n >= N_MAX_TEXTURES) return PIPELINE_INVALID_RANGE;

    data->textures[n] = t;
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_set_mvp(struct pipeline_data *data, mat4 mvp)
{
    if (!data || !mvp) return PIPELINE_INVALID_DATA;

    glm_mat4_copy(mvp, data->mvp);
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_set_fb(struct pipeline_data *data, struct framebuffer *fb)
{
    if (!data || !fb) return PIPELINE_INVALID_FB;

    data->fb = fb;
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_get_n_faces(struct pipeline_data *data, size_t *n_faces)
{
    if (!data || !n_faces) return PIPELINE_INVALID_DATA;

    *n_faces = data->n_triangles;
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_get_mvp(struct pipeline_data *data, mat4 mvp)
{
    if (!data) return PIPELINE_INVALID_DATA;

    mvp = data->mvp;
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_get_texture(struct pipeline_data *data, size_t slot,
			       struct texture **tex)
{
    if (!data) return PIPELINE_INVALID_DATA;
    if (slot >= N_MAX_TEXTURES) return PIPELINE_INVALID_RANGE;

    *tex = data->textures[slot];
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_data_get_n_verts(struct pipeline_data *data, size_t *n_verts)
{
    if (!data || !n_verts) return PIPELINE_INVALID_DATA;

    *n_verts = data->n_verts;
    return PIPELINE_OK;
}

enum PipelineError
rndr_pipeline_draw_triangles(struct pipeline_data *data,
			     vertex_shader vs,
			     fragment_shader fs)
{
    /* Prepare auxiliar memory */
    rndr_pipeline_data_create_temp(data);

    enum PipelineError err = PIPELINE_OK;
    /* Run vertex shader */
    if ((err = vs(data)) != PIPELINE_OK) {
	/* TODO error handling */
	return err;
    }

    /* Run fragment shader */
    if ((err = fs(data)) != PIPELINE_OK) {
	/* TODO error handling */
	return err;
    }

    /* Clear temporary memory */
    rndr_pipeline_data_free_temp(data);

    /* Everything executed correctly */
    return err;
}

/* --- Private functions --- */
static void
rndr_pipeline_data_create_temp(struct pipeline_data *data)
{
    /* Check if pointers are valid */
    if (!data || !data->u || !data->vn)
	return;

    /* Free memory if not freed previously */
    rndr_pipeline_data_free_temp(data);

    /* Allocate memory for vertices */
    data->u_aux = calloc(data->n_verts, sizeof(vec3));
    /* Allocate memory for normals */
    data->vn_aux = calloc(data->n_verts, sizeof(vec3));
}

static void
rndr_pipeline_data_free_temp(struct pipeline_data *data)
{
    /* Check if pointers are valid */
    if (!data || !data->u || !data->vn)
	return;

    /* Free memory if not freed previously */
    if (data->u_aux)  free(data->u_aux);
    if (data->vn_aux) free(data->vn_aux);
}
