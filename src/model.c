#define _GNU_SOURCE         /* Needed for getline */
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <model.h>
#include <geometry.h>

/*TODO - Polygons grater than 3 sides*/
struct face {
    /* int* verts; */
    /* int  n_verts; */
    int verts[3];
    int uv[3];      /* Texture coordinates */
};

struct model {
    struct vec3f* verts;
    int n_verts;
    struct face* faces;
    int n_faces;
    struct vec2f* tc;
    int n_tc;
};

struct model*
renderer_model_create(const char *filename)
{
    /* Return if bad filename */
    if (!filename) return NULL;

    /* Allocate initial space for the model representation */
    struct model* model = malloc(sizeof(struct model));
    if (!model) return NULL;
    model->n_verts = 0;
    model->n_faces = 0;
    model->n_tc = 0;

    int res_verts = 16, res_faces = 16, res_vt = 16;     /* Initial reserved verts/faces */
    model->verts = malloc(sizeof(struct vec3f) * res_verts);
    if (!model->verts) { /* Could not allocate */
        free(model); 
        return NULL;
    }  
    model->faces = malloc(sizeof(struct face) * res_faces);
    if (!model->faces) { /* Could not allocate */
        free(model->verts);
        free(model);
        return NULL;
    }  
    model->tc = malloc(sizeof(struct vec2f) * res_vt);
    if (!model->tc) { /* Could not allocate */
        free(model->faces);
        free(model->verts);
        free(model);
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
                struct vec3f *aux;
                aux = realloc(model->verts, sizeof(struct vec3f) * res_verts);
                if (!aux) {
                    free(model->verts);
                    free(model->faces);
                    free(model->tc);
                    free(model);
                    return NULL;
                } else {
                    model->verts = aux;
                }
            }

            /* Insert new vert */
            struct vec3f *vec = &model->verts[model->n_verts];
            vec->x = x;
            vec->y = y;
            vec->z = z;
            model->n_verts++;
        } else if (strncmp(line, "vt ", 3) == 0) {       /* Tex coords */
            float u, v;
            if (sscanf(line, "vt %f %f %*f", &u, &v) != 2)
                continue;

            /* Reserve memory if necessary */
            if (model->n_tc == res_vt - 1) {
                res_vt *= 2;
                struct vec2f *aux;
                aux = realloc(model->tc, sizeof(struct vec2f) * res_vt);
                if (!aux) {
                    free(model->tc);
                    free(model->faces);
                    free(model->verts);
                    free(model);
                    return NULL;
                } else {
                    model->tc = aux;
                }
            }

            /* Insert new tex coordinate */
            struct vec2f *texcoord = &model->tc[model->n_tc];
            texcoord->x = u; texcoord->y = v;
            model->n_tc++;
        } else if (strncmp(line, "vn ", 3) == 0) {       /* Normals */
            /* TODO */
            continue;
        } else if (strncmp(line, "f ", 2) == 0) {        /* Faces */
            /*TODO - Polygons greater than 3 sides*/
            int v1 = 0, v2 = 0, v3 = 0, uv1 = 0, uv2 = 0, uv3 = 0;
            if (sscanf(line, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d",
                        &v1, &uv1, &v2, &uv2, &v3, &uv3) != 6) {
                continue;
            }

            /* Reserve memory if necessary */
            if (model->n_faces == res_faces - 1) {
                res_faces *= 2;
                struct face *aux;
                aux = realloc(model->faces, sizeof(struct face) * res_faces);
                if (!aux) {
                    free(model->faces);
                    free(model->tc);
                    free(model->verts);
                    free(model);
                    return NULL;
                } else {
                    model->faces = aux;
                }
            }

            /* Insert new face */
            struct face *face = &model->faces[model->n_faces];
            face->verts[0] = v1 - 1;       /* Wavefront OBJ indices start at 1 */
            face->verts[1] = v2 - 1;
            face->verts[2] = v3 - 1;
            face->uv[0] = uv1 - 1;
            face->uv[1] = uv2 - 1;
            face->uv[2] = uv3 - 1;
            model->n_faces++;
        }
    }

    /* Reallocate for using only the needed space */
    model->verts = realloc(model->verts, sizeof(struct vec3f) * model->n_verts);
    model->faces = realloc(model->faces, sizeof(struct face) * model->n_faces);
    model->tc    = realloc(model->tc, sizeof(struct vec2f) * model->n_tc);

    free(line);
    fclose(f);

    return model;
}

void
renderer_model_delete(struct model **model)
{
    free((*model)->verts);
    free((*model)->faces);
    free((*model)->tc);
    free(*model);

    *model = NULL;
}

int
renderer_model_nvers(const struct model *model)
{
    return model->n_verts;
}

int
renderer_model_nfaces(const struct model *model)
{
    return model->n_faces;
}

struct vec3f *
renderer_model_vert(const struct model *model, int i)
{
    if (model && model->verts && i >= 0 && i < model->n_verts) {
        return &model->verts[i];
    } else {
        return NULL;
    }
}

struct vec2f *
renderer_model_texcoords(const struct model *model, int i)
{ 
    if (model && model->tc && i >= 0 && i < model->n_tc) {
        return &model->tc[i];
    } else {
        return NULL;
    }

}

struct face *
renderer_model_face(const struct model *model, int i)
{
    if (model && model->faces && i >= 0 && i < model->n_faces) {
        return &model->faces[i];
    } else {
        return NULL;
    }
}

/* Returns the jth vec of the ith face of the model */
struct vec3f *
renderer_model_face_vec(const struct model *m, int i, int j)
{
    if (j < 0 || j > 2) return NULL;

    struct face *f = renderer_model_face(m, i);
    if (!f) return NULL;
    
    struct vec3f *v = renderer_model_vert(m, f->verts[j]);

    return v;       /* If invalid it is already NULL */
}

/* Returns the jth uv coord of the ith face of the model */
struct vec2f *
renderer_model_face_uv(const struct model *m, int i, int j)
{
    if (j < 0 || j > 2) return NULL;

    struct face *f = renderer_model_face(m, i);
    if (!f) return NULL;

    struct vec2f *uv = renderer_model_texcoords(m, f->uv[j]);

    return uv;       /* If invalid it is already NULL */
}
