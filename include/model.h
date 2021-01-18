/**
 *   \file model.h
 *   \brief Simple library to load Wavefront OBJ 3D objects
 */

#ifndef MODEL_H_RBWTLNK5
#define MODEL_H_RBWTLNK5

#include <cglm/vec2.h>
#include <cglm/vec3.h>

struct model;
struct face;

/**
 * @brief      Create a 3D model 
 *
 * @details    Creates a 3D model in the heap that has to be freed with rndr_model_delete
 *
 * @param      filename indicates the file path of the obj model
 *
 * @return     Data structure containing the 3d model.
 */
struct model*
rndr_model_create(const char *filename);

/**
 *  \brief Frees a model's memeory
 *  \param model: pointer to a pointer of a model
 *  \return void
 */
void
rndr_model_delete(struct model **model);

/**
 *  \brief Returns the number of vertices in a model
 *
 *  If no model is given, 0 is returned
 *
 *  \param model: a pointer to the models
 *  \return Number of vertices
 */
int
rndr_model_nvers(const struct model *model);

/**
 *  \brief Returns the number of faces of a given model 
 *
 *  If no model is given, 0 is returned
 *
 *  \param model: a pointer to the model
 *  \return Number of faces
 */
int
rndr_model_nfaces(const struct model *model);

/**
 *  \brief Returns the ith vertex of a given model.
 *  \param model: pointer to a 3D model.
 *  \param i: number of the vertex.
 *  \return Pointer to the vec3 of the vertex.
 */
vec3 *
rndr_model_vert(const struct model *model, int i);

/**
 *  \brief Retrieve the texture coordinates of vertex i.
 *  \param model: pointer to the 3D model.
 *  \param i: id of the vertex.
 *  \return pointer containing vec2 of the texture.
 */
vec2 *
rndr_model_texcoords(const struct model *model, int i);

/**
 *  \brief Retrieve the normal of a vertex 
 *  \param model: pointer of the 3D model
 *  \param i: id of the vertex
 *  \return Pointer to the corresponding normal coordinates
 */
vec3 *
rndr_model_normal(const struct model *m, int i);

/**
 *  \brief Retrieves the ith face of a model
 *  \param model: pointer to the 3D model.
 *  \param i: id of the face.
 *  \return Pointer to the face
 */
struct face *
rndr_model_face(const struct model *model, int i);

/**
 *  \brief Returns the jth vec of the ith face of the model
 *  \param m: Pointer of the 3D model
 *  \param i: id of the face
 *  \param j: id of the vertex (0 to 2 for triangle face)
 *  \return Pointer to the vertex
 */
vec3 *
rndr_model_face_vec(const struct model *m, int i, int j);

/**
 *  \brief Returns the jth uv coord of the ith face of the model
 *  \param m: Pointer of the 3D model
 *  \param i: id of the face
 *  \param j: id of the uv coord (0 to 2 for triangle face)
 *  \return Pointer to the uv coords
 */
vec2 *
rndr_model_face_uv(const struct model *m, int i, int j);

/**
 *  \brief Returns the jth normal of the ith face of the model
 *  \param m: Pointer of the 3D model
 *  \param i: id of the face
 *  \param j: id of the normal (0 to 2 for triangle face)
 *  \return Pointer to the normal
 */
vec3 *
rndr_model_face_vn(const struct model *m, int i, int j);

#endif /* end of include guard: MODEL_H_RBWTLNK5 */
