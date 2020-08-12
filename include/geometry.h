#ifndef GEOMETRY_H_HGQZ875L
#define GEOMETRY_H_HGQZ875L

struct vec3f {
    float x, y, z;
};

struct vec2i {
    int x, y;
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
renderer_geometry_dot(const struct vec3f *v1, const struct vec3f *v2);

/*Cross product*/
struct vec3f
renderer_geometry_cross(const struct vec3f *v1, const struct vec3f *v2);

/*Normalize vector*/
void
renderer_geometry_normalize(struct vec3f *v);

/*Get bounding box of triangle*/
struct bbox
renderer_geometry_triangle_bounding_box(const gm_triangle *tr);

/*Check if pixel is inside a triangle*/
int
renderer_geometry_pixel_in_triangle(const gm_triangle *tr, const struct vec2i *p);

#endif /* end of include guard: GEOMETRY_H_HGQZ875L */
