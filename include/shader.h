/**
 *   \file shader.h
 *   \brief Declaration of different types of shaders
 */

#ifndef SHADER_H
#define SHADER_H

#include <pipeline.h>

struct framebuffer;

enum PipelineError
vertex_shader_default(triangle_vertices u,
		      const mat4 mvp);

enum PipelineError
fragment_shader_default(struct framebuffer *fb,
			triangle_vertices u,
			const triangle_tex_coords uv,
			const triangle_normals vn,
			const tex_array textures);

#endif /* SHADER_H */
