#include <limits.h>
#include <math.h>
#include <geometry.h>

#include <cglm/vec3.h>

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

void
rndr_geometry_barycentric_coords(const gm_triangle *tr, const struct vec2i *p, vec3 dest)
{
    vec3 c = {-1.0f, -1.0f, -1.0f};
    if (!tr || !p) {
        glm_vec3_copy(c, dest);
    }

    vec3 v1 = {(*tr)[2].x - (*tr)[0].x, (*tr)[1].x - (*tr)[0].x, (*tr)[0].x - p->x};
    vec3 v2 = {(*tr)[2].y - (*tr)[0].y, (*tr)[1].y - (*tr)[0].y, (*tr)[0].y - p->y};

    glm_vec3_cross(v1, v2, c);

    /* If the triangle is degenerate, c.z will be 0 */
    if (fabs(c[2]) < 1.0f) {
        c[0] = -1.0f;
    } else {
        vec3 u;
        glm_vec3_copy(c, u);
        c[0] = 1.0f - (u[0] + u[1]) / u[2];
        c[1] = u[1] / u[2];
        c[2] = u[0] / u[2];
    }

    glm_vec3_copy(c, dest);
}

/*Returns 1 if point is inside the triangle, 0 if not*/
int
rndr_geometry_pixel_in_triangle(const gm_triangle *tr, const struct vec2i *p)
{
    if (!tr || !p) return 0;

    vec3 c;
    rndr_geometry_barycentric_coords(tr, p, c);

    if (c[0] < 0.0f || c[1] < 0.0f || c[2] < 0.0f)
        return 0;
    else
        return 1;
}

void
rndr_geometry_triangle_normal(vec3 *u[3], vec3 dest)
{
    vec3 n = {0.0f, 0.0f, 0.0f};
    if (!u || !u) {
        glm_vec3_copy(n, dest);
        return;
    }

    /* Compute normal and normalize */
    vec3 u1, u2;
    glm_vec3_sub(*u[1], *u[0], u1);
    glm_vec3_sub(*u[2], *u[0], u2);

    glm_vec3_crossn(u1, u2, dest);
}
