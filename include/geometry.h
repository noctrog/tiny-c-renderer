#ifndef GEOMETRY_H_HGQZ875L
#define GEOMETRY_H_HGQZ875L

#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>

/* TODO: use _Generic */

struct vec2i {
    int x, y;
};

struct vec3i {
    int x, y, z;
};

struct vec4i {
    int x, y, z, w;
};

typedef struct vec2i gm_triangle[3];

/*Bounding box*/
struct bbox {
    int x, y, w, h;
};

/*Get bounding box of triangle*/
struct bbox
rndr_geometry_triangle_bounding_box(const gm_triangle *tr);

/* Calculate barycentric coordinates */
void
rndr_geometry_barycentric_coords(const gm_triangle *tr, const struct vec2i *p, vec3 dest);

/*Check if pixel is inside a triangle*/
int
rndr_geometry_pixel_in_triangle(const gm_triangle *tr, const struct vec2i *p);

/* Calculate normal of a triangle */
void
rndr_geometry_triangle_normal(vec3 *u[3], vec3 dest);

#endif /* end of include guard: GEOMETRY_H_HGQZ875L */
