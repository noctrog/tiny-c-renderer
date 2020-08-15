#ifndef GEOMETRY_H_HGQZ875L
#define GEOMETRY_H_HGQZ875L

struct vec2f {
    float x, y;
};

struct vec3f {
    float x, y, z;
};

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

/*Dot product*/
float
rndr_geometry_dot(const struct vec3f *v1, const struct vec3f *v2);

/*Cross product*/
struct vec3f
rndr_geometry_cross(const struct vec3f *v1, const struct vec3f *v2);

/*Normalize vector*/
void
rndr_geometry_normalize(struct vec3f *v);

/*Get bounding box of triangle*/
struct bbox
rndr_geometry_triangle_bounding_box(const gm_triangle *tr);

struct vec3f
rndr_geometry_barycentric_coords(const gm_triangle *tr, const struct vec2i *p);

/*Check if pixel is inside a triangle*/
int
rndr_geometry_pixel_in_triangle(const gm_triangle *tr, const struct vec2i *p);

// Calculate normal of a triangle
struct vec3f
rndr_geometry_triangle_normal(struct vec3f **u);

#endif /* end of include guard: GEOMETRY_H_HGQZ875L */
