/*******************************************************
*  Model header. Works with Wavefront OBJ 3D objetcs  *
*******************************************************/

#ifndef MODEL_H_RBWTLNK5
#define MODEL_H_RBWTLNK5

struct model;
struct vec3f;
struct face;

struct model*
renderer_model_create(const char *filename);

void
renderer_model_delete(struct model **model);

int
renderer_model_nvers(const struct model *model);

int
renderer_model_nfaces(const struct model *model);

struct vec3f *
renderer_model_vert(const struct model *model, int i);

struct face *
renderer_model_face(const struct model *model, int i);

struct vec3f *
renderer_model_face_vec(const struct model *m, int i, int j);

#endif /* end of include guard: MODEL_H_RBWTLNK5 */
