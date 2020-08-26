/*******************************************************
*  Model header. Works with Wavefront OBJ 3D objetcs  *
*******************************************************/

#ifndef MODEL_H_RBWTLNK5
#define MODEL_H_RBWTLNK5

#include <cglm/vec2.h>
#include <cglm/vec3.h>

struct model;
struct face;

struct model*
rndr_model_create(const char *filename);

void
rndr_model_delete(struct model **model);

int
rndr_model_nvers(const struct model *model);

int
rndr_model_nfaces(const struct model *model);

vec3 *
rndr_model_vert(const struct model *model, int i);

vec2 *
rndr_model_texcoords(const struct model *model, int i);

vec3 *
rndr_model_normal(const struct model *m, int i);

struct face *
rndr_model_face(const struct model *model, int i);

vec3 *
rndr_model_face_vec(const struct model *m, int i, int j);

vec2 *
rndr_model_face_uv(const struct model *m, int i, int j);

vec3 *
rndr_model_face_vn(const struct model *m, int i, int j);

#endif /* end of include guard: MODEL_H_RBWTLNK5 */
