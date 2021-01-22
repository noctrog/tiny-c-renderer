/**
 *   \file shader.h
 *   \brief Declaration of different types of shaders
 */

#ifndef SHADER_H
#define SHADER_H

#include <pipeline.h>

struct framebuffer;

enum PipelineError
vertex_shader_default(struct pipeline_data *data);

enum PipelineError
fragment_shader_default(struct pipeline_data *data);

#endif /* SHADER_H */
