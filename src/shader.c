#include "pipeline.h"
#include <shader.h>

enum PipelineError
vertex_shader_default(struct pipeline_data *data)
{
    if (!data) return PIPELINE_INVALID_DATA;

    size_t i, n_faces;
    rndr_pipeline_data_get_n_faces(data, &n_faces);

    /* For each triangle in mesh */
    for (i = 0; i < n_faces; ++i) {
	
    }

    return PIPELINE_OK;
}

enum PipelineError
fragment_shader_default(struct pipeline_data *data)
{
  
    return PIPELINE_OK;
}
