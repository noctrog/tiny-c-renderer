#include <limits.h>
#include <math.h>
#include <geometry.h>

float
rndr_geometry_dot(const struct vec3f *v1, const struct vec3f *v2)
{
    if (!v1 || !v2) return 0.0f;

    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

struct vec3f
rndr_geometry_cross(const struct vec3f *v1, const struct vec3f *v2)
{
    struct vec3f v = {.x = 0.0f, .y = 0.0f, .z = 0.0f};

    if (v1 && v2) {
        v.x = v1->y * v2->z - v1->z * v2->y;
        v.y = v1->z * v2->x - v1->x * v2->z;
        v.z = v1->x * v2->y - v1->y * v2->x;
    }

    return v;
}

void
rndr_geometry_normalize(struct vec3f *v)
{
    if (!v) return;

    float norm = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= norm;
    v->y /= norm;
    v->z /= norm;
}

struct bbox 
rndr_geometry_triangle_bounding_box(const gm_triangle* tr)
{
    struct bbox bb = {.x = -1, .y = -1, .w = -1, .h = -1};

    if (tr) {
        int j;
        int min_x = INT_MAX, max_x = -1, min_y = INT_MAX, max_y = -1;
        for (j = 0; j < 3; ++j) {
            int x = (*tr)[j].x, y = (*tr)[j].y;
            if (x < min_x) min_x = x;
            if (y < min_y) min_y = y;
            if (x > max_x) max_x = x;
            if (y > max_y) max_y = y;
        }

        bb.x = min_x;
        bb.y = min_y;
        bb.w = max_x - min_x;
        bb.h = max_y - min_y;
    }

    return bb;
}

struct vec3f
rndr_geometry_barycentric_coords(const gm_triangle *tr, const struct vec2i *p)
{
    struct vec3f c = {.x = -1.0f, .y = -1.0f, .z = -1.0f};
    if (!tr || !p) return c;

    struct vec3f v1 = {.x = (*tr)[2].x - (*tr)[0].x, .y = (*tr)[1].x - (*tr)[0].x, .z = (*tr)[0].x - p->x};
    struct vec3f v2 = {.x = (*tr)[2].y - (*tr)[0].y, .y = (*tr)[1].y - (*tr)[0].y, .z = (*tr)[0].y - p->y};
    c = rndr_geometry_cross(&v1, &v2);

    /* If the triangle is degenerate, c.z will be 0 */
    if (fabs(c.z) < 1.0f) {
        c.x = -1.0f;
    } else {
        struct vec3f u = c;
        c.x = 1.0f - (u.x + u.y) / u.z;
        c.y = u.y / u.z;
        c.z = u.x / u.z;
    }

    return c;
}

/*Returns 1 if point is inside the triangle, 0 if not*/
int
rndr_geometry_pixel_in_triangle(const gm_triangle *tr, const struct vec2i *p)
{
    if (!tr || !p) return 0;

    struct vec3f c = rndr_geometry_barycentric_coords(tr, p);

    if (c.x < 0.0f || c.y < 0.0f || c.z < 0.0f)
        return 0;
    else
        return 1;
}

struct vec3f
rndr_geometry_triangle_normal(struct vec3f **u)
{
    struct vec3f n = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    if (!u || !*u) return n;

    /* Compute normal */
    struct vec3f u1 = {.x = u[1]->x - u[0]->x, 
        .y = u[1]->y - u[0]->y,
        .z = u[1]->z - u[0]->z };
    struct vec3f u2 = {.x = u[2]->x - u[0]->x,
        .y = u[2]->y - u[0]->y,
        .z = u[2]->z - u[0]->z };
    n = rndr_geometry_cross(&u1, &u2);
    rndr_geometry_normalize(&n);

    return n;
}
