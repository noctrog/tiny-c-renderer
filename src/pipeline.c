#include <string.h>
#include <pipeline.h>
#include <framebuffer.h>

enum PipelineError
rndr_pipeline_draw_triangles(struct framebuffer *fb,
			     const struct pipeline_data *data,
			     vertex_shader vs,
			     fragment_shader fs)
{
    /* Create a copy of all triangle vertices, since their positions
     * will be modified by the vertex shader */
    triangle_vertices new_u;
    *new_u = calloc(data->n_triangles, sizeof(vec3));
    memcpy(*new_u, data->u, sizeof(vec3) * data->n_triangles);

    enum PipelineError err = PIPELINE_OK;
    /* Run vertex shader */
    if ((err = vs(new_u, data->mvp)) != PIPELINE_OK) {
	/* TODO error handling */
	return err;
    }

    /* Run fragment shader */
    if ((err = fs(fb, new_u, data->uv, data->vn, data->textures)) != PIPELINE_OK) {
	/* TODO error handling */
	return err;
    }

    /* Everything executed correctly */
    return err;
}
