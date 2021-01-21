#include "pipeline.h"
#include <shader.h>
#include <framebuffer.h>

enum PipelineError
vertex_shader_default(triangle_vertices u,
		      const mat4 mvp)
{

  return 0;
}

enum PipelineError
fragment_shader_default(struct framebuffer *fb,
			triangle_vertices u,
			const triangle_tex_coords uv,
			const triangle_normals vn,
			const tex_array textures);
