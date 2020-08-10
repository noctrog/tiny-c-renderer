#define _GNU_SOURCE         /* Needed for getline */
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <model.h>
#include <geometry.h>

/*TODO - Polygons grater than 3 sides*/
struct face {
    /*int* verts;*/
    /*int  n_verts;*/
    int verts[3];
};

struct model {
    struct vec3f* verts;
    int n_verts;
    struct face* faces;
    int n_faces;
};

struct model*
renderer_model_create(const char *filename)
{
    /*Return if bad filename*/
    if (!filename) return NULL;

    /*Allocate initial space for the model representation*/
    struct model* model = malloc(sizeof(struct model));
    if (!model) return NULL;
    int res_verts = 16, res_faces = 16;     /* Initial reserved verts/faces */
    model->verts = malloc(sizeof(struct vec3f) * res_verts);
    if (!model->verts) {free(model); return NULL;}  /* Could not allocate */
    model->faces = malloc(sizeof(struct face)  * res_faces);
    if (!model->faces) {free(model->verts); free(model); return NULL;}  /* Could not allocate */

    FILE *f = fopen(filename, "r");
    char *line;
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
                model->verts = realloc(model->verts, sizeof(struct vec3f) * res_verts);
            }

            /* Insert new vert */
            struct vec3f *vec = &model->verts[model->n_verts];
            vec->x = x;
            vec->y = y;
            vec->z = z;
            model->n_verts++;
        } else if (strncmp(line, "vt ", 3) == 0) {       /* Tex coords */
            /* TODO */
            continue;
        } else if (strncmp(line, "vn ", 3) == 0) {       /* Normals */
            /* TODO */
            continue;
        } else if (strncmp(line, "f ", 2) == 0) {        /* Faces */
            /*TODO - Polygons greater than 3 sides*/
            int id1 = 0, id2 = 0, id3 = 0;
            if (sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
                        &id1, &id2, &id3) != 3) {
                continue;
            }

            /*Reserve memory if necessary*/
            if (model->n_faces == res_faces - 1) {
                res_faces *= 2;
                model->faces = realloc(model->faces, sizeof(struct face) * res_faces);
            }

            /* Insert new face */
            struct face *face = &model->faces[model->n_faces];
            face->verts[0] = id1 - 1;       /* Wavefront OBJ indices start at 1 */
            face->verts[1] = id2 - 1;
            face->verts[2] = id3 - 1;
            model->n_faces++;
        }
    }

    /*Reallocate for using only the needed space*/
    model->verts = realloc(model->verts, sizeof(struct vec3f) * model->n_verts);
    model->faces = realloc(model->faces, sizeof(struct face) * model->n_faces);

    return model;
}

void
renderer_model_delete(struct model **model)
{
    free((*model)->verts);
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
