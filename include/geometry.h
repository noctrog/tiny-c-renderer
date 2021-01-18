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

/**
 *  \brief Compute the bounding box of a triangle (AABB)
 *  \param tr: pointer to a triangle
 *  \return bounding box
 */
struct bbox 
rndr_geometry_triangle_bounding_box(const gm_triangle* tr);

/**
 *  \brief Compute barycentric coordinates
 *
 *  https://en.wikipedia.org/wiki/Barycentric_coordinate_system
 *
 *  \param tr: pointer to a triangle
 *  \param p: pointer to a point refering to a pixel (vec2i)
 *  \param dest: vector containing the resulting barycentri coords.
 *  \return void
 */
void
rndr_geometry_barycentric_coords(const gm_triangle *tr, const struct vec2i *p, vec3 dest);

/*Returns 1 if point is inside the triangle, 0 if not*/
/**
 *  \brief Returns 1 if p is inside the triangle, 0 if not
 *  \param tr: pointer to a triangle
 *  \param p: pointer to a point
 *  \return integer
 */
int
rndr_geometry_pixel_in_triangle(const gm_triangle *tr, const struct vec2i *p);

/**
 *  \brief Computes the normal vector of a triangle in 3D space
 *  \param u: array of size 3 of vec3 pointers (vertices of the triangle)
 *  \param dest: resulting normal vector
 *  \return void
 */
void
rndr_geometry_triangle_normal(vec3 *u[3], vec3 dest);

#endif /* end of include guard: GEOMETRY_H_HGQZ875L */
