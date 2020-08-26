#define _GNU_SOURCE         /* Needed for getline */
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <model.h>
#include <geometry.h>

/*TODO - Polygons grater than 3 sides*/
struct face {
    int verts[3];   /* Vertices */
    int uv[3];      /* Texture coordinates */
    int vn[3];      /* Normals */
};

struct model {
    vec3* verts;
    int n_verts;
    struct face* faces;
    int n_faces;
    vec2* tc;
    int n_tc;
    vec3 *vn;
    int n_vn;
};

struct model*
rndr_model_create(const char *filename)
{
    /* Return if bad filename */
    if (!filename) return NULL;

    /* Allocate initial space for the model representation */
    struct model* model = malloc(sizeof(struct model));
    if (!model) return NULL;
    model->n_verts = 0;
    model->n_faces = 0;
    model->n_tc = 0;
    model->n_vn = 0;

    int res_verts, res_faces, res_vt, res_vn;   /* Initial reserved verts/faces */
    res_verts = res_faces = res_vt = res_vn = 16;
    model->verts = malloc(sizeof(vec3) * res_verts);
    if (!model->verts) { /* Could not allocate */
        rndr_model_delete(&model);
        return NULL;
    }  
    model->faces = malloc(sizeof(struct face) * res_faces);
    if (!model->faces) { /* Could not allocate */
        rndr_model_delete(&model);
        return NULL;
    }  
    model->tc = malloc(sizeof(vec2) * res_vt);
    if (!model->tc) { /* Could not allocate */
        rndr_model_delete(&model);
        return NULL;
    }
    model->vn = malloc(sizeof(vec3) * res_vn);
    if (!model->vn) { /* Could not allocate */
        rndr_model_delete(&model);
        return NULL;
    }

    FILE *f = fopen(filename, "r");
    char *line = NULL;
    ssize_t linelen = 0;
    size_t linecap = 0;
    while((linelen = getline(&line, &linecap, f)) != -1) {
        /* Substitute comments with end of string */
        int j;
        for (j = 0; j < linelen; ++j) {
            if (line[j] == '#') {
                line[j] = '\0';
                linelen = j;
                break;
            }
        }
        /* Empty lines */
        if (linelen < 2) continue;

        /* Read contents */
        if (strncmp(line, "v ", 2) == 0) {               /* Vertices */
            float x = 0.0f, y = 0.0f, z = 0.0f;
            if (sscanf(line, "%*s %f %f %f", &x, &y, &z) != 3)
                continue;
            
            /* Reserve memory if necessary */
            if (model->n_verts == res_verts - 1) {
                res_verts *= 2;
                vec3 *aux;
                aux = realloc(model->verts, sizeof(vec3) * res_verts);
                if (!aux) {
                    rndr_model_delete(&model);
                    return NULL;
                } else {
                    model->verts = aux;
                }
            }

            /* Insert new vert */
            vec3 *vec = &model->verts[model->n_verts];
            (*vec)[0] = x;
            (*vec)[1] = y;
            (*vec)[2] = z;
            model->n_verts++;
        } else if (strncmp(line, "vt ", 3) == 0) {       /* Tex coords */
            float u, v;
            if (sscanf(line, "vt %f %f %*f", &u, &v) != 2)
                continue;

            /* Reserve memory if necessary */
            if (model->n_tc == res_vt - 1) {
                res_vt *= 2;
                vec2 *aux;
                aux = realloc(model->tc, sizeof(vec2) * res_vt);
                if (!aux) {
                    rndr_model_delete(&model);
                    return NULL;
                } else {
                    model->tc = aux;
                }
            }

            /* Insert new tex coordinate */
            vec2 *texcoord = &model->tc[model->n_tc];
            (*texcoord)[0] = u; (*texcoord)[1] = v;
            model->n_tc++;
        } else if (strncmp(line, "vn ", 3) == 0) {       /* Normals */
            vec3 vn;
            if (sscanf(line, "vn %f %f %f", &vn[0], &vn[1], &vn[2]) != 3)
                continue;

            /* Reserve memory if necesary */
            if (model->n_vn == res_vn - 1) {
                res_vn *= 2;
                vec3 *aux;
                aux = realloc(model->vn, sizeof(vec3) * res_vn);
                if (!aux) {
                    rndr_model_delete(&model);
                    return NULL;
                } else {
                    model->vn = aux;
                }
            }

            /* Insert new normal */
            memcpy(&model->vn[model->n_vn], &vn, sizeof(vec3));
            model->n_vn++;
        } else if (strncmp(line, "f ", 2) == 0) {        /* Faces */
            /*TODO - Polygons greater than 3 sides*/
            struct vec3i v, uv, vn;
            if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &v.x, &uv.x, &vn.x, &v.y, &uv.y, &vn.y, &v.z, &uv.z, &vn.z) != 9) {
                continue;
            }

            /* Reserve memory if necessary */
            if (model->n_faces == res_faces - 1) {
                res_faces *= 2;
                struct face *aux;
                aux = realloc(model->faces, sizeof(struct face) * res_faces);
                if (!aux) {
                    rndr_model_delete(&model);
                    return NULL;
                } else {
                    model->faces = aux;
                }
            }

            /* Insert new face */
            struct face *face = &model->faces[model->n_faces];
            face->verts[0] = v.x - 1;       /* Wavefront OBJ indices start at 1 */
            face->verts[1] = v.y - 1;
            face->verts[2] = v.z - 1;
            face->uv[0] = uv.x - 1;
            face->uv[1] = uv.y - 1;
            face->uv[2] = uv.z - 1;
            face->vn[0] = vn.x - 1;
            face->vn[1] = vn.y - 1;
            face->vn[2] = vn.z - 1;
            model->n_faces++;
        }
    }

    /* Reallocate for using only the needed space */
    model->verts = realloc(model->verts, sizeof(vec3) * model->n_verts);
    model->faces = realloc(model->faces, sizeof(struct face) * model->n_faces);
    model->tc    = realloc(model->tc, sizeof(vec2) * model->n_tc);
    model->vn    = realloc(model->vn, sizeof(vec3) * model->n_vn);

    free(line);
    fclose(f);

    return model;
}

void
rndr_model_delete(struct model **model)
{
    if (!model || !*model) return;

    if ((*model)->verts)
        free((*model)->verts);
    if ((*model)->faces)
        free((*model)->faces);
    if ((*model)->tc)
        free((*model)->tc);
    if ((*model)->vn)
        free((*model)->vn);

    free(*model);

    *model = NULL;
}

int
rndr_model_nvers(const struct model *model)
{
    return model->n_verts;
}

int
rndr_model_nfaces(const struct model *model)
{
    return model->n_faces;
}

vec3 *
rndr_model_vert(const struct model *model, int i)
{
    if (model && model->verts && i >= 0 && i < model->n_verts) {
        return &model->verts[i];
    } else {
        return NULL;
    }
}

vec2 *
rndr_model_texcoords(const struct model *model, int i)
{ 
    if (model && model->tc && i >= 0 && i < model->n_tc) {
        return &model->tc[i];
    } else {
        return NULL;
    }

}

vec3 *
rndr_model_normal(const struct model *m, int i)
{
    if (m && m->tc && i >= 0 && i < m->n_vn) {
        return &m->vn[i];
    } else {
        return NULL;
    }

}

struct face *
rndr_model_face(const struct model *model, int i)
{
    if (model && model->faces && i >= 0 && i < model->n_faces) {
        return &model->faces[i];
    } else {
        return NULL;
    }
}

/* Returns the jth vec of the ith face of the model */
vec3 *
rndr_model_face_vec(const struct model *m, int i, int j)
{
    if (j < 0 || j > 2) return NULL;

    struct face *f = rndr_model_face(m, i);
    if (!f) return NULL;
    
    vec3 *v = rndr_model_vert(m, f->verts[j]);

    return v;       /* If invalid it is already NULL */
}

/* Returns the jth uv coord of the ith face of the model */
vec2 *
rndr_model_face_uv(const struct model *m, int i, int j)
{
    if (j < 0 || j > 2 || i < 0 || i >= m->n_faces) return NULL;

    struct face *f = rndr_model_face(m, i);
    if (!f) return NULL;

    vec2 *uv = rndr_model_texcoords(m, f->uv[j]);

    return uv;       /* If invalid it is already NULL */
}

/* Returns the jth normal of the ith face of the model */
vec3 *
rndr_model_face_vn(const struct model *m, int i, int j)
{
    if (j < 0 || j > 2 || i < 0 || i >= m->n_faces) return NULL;

    struct face *f = rndr_model_face(m, i);
    if (!f) return NULL;

    vec3 *vn = rndr_model_normal(m, f->vn[j]);

    return vn;      /* If invalid it is already NULL */
}
