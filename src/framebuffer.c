#include <stdlib.h>
#include <math.h>
#include <framebuffer.h>
#include <geometry.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

struct framebuffer {
    int x, y;
    unsigned char* color;
    float *z_buffer;
};

static inline void 
int_swap(int *a, int *b)
{
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a = *a ^ *b;
}

struct framebuffer* 
renderer_framebuffer_create(int x, int y)
{
    if (x <= 0 || y <= 0) return NULL;

    struct framebuffer* fb = malloc(sizeof(struct framebuffer));
    if (!fb) return NULL;

    /* Color data */
    fb->color = calloc(3 * x * y, sizeof(unsigned char));
    if (!fb->color) {
        free(fb);
        return NULL;
    }

    /*Z buffer */
    fb->z_buffer = malloc(x * y * sizeof(float));
    if (!fb->z_buffer) {
        free(fb->color);
        free(fb);
        return NULL;
    }

    fb->x = x;
    fb->y = y;

    return fb;
}

void
renderer_framebuffer_delete(struct framebuffer **fb)
{
    if ((*fb)->color)       free((*fb)->color);
    if ((*fb)->z_buffer)    free((*fb)->z_buffer);
    free(*fb);
    *fb = NULL;
}

void
renderer_framebuffer_set_pixel(struct framebuffer *fb, const struct color *col,
                               const struct vec2i *p)
{
    if (!fb || !col || !p) return;
    if (p->x < 0 || p->y < 0 || p->x >= fb->x || p->y >= fb->y) return;

    /* Origin is at the left bottom corner of the framebuffer */
    size_t idx = 3 * (fb->x * (fb->y - 1 - p->y) + p->x);

    fb->color[idx+0] = col->r;
    fb->color[idx+1] = col->g;
    fb->color[idx+2] = col->b;
}

void
renderer_framebuffer_set_pixel_z(struct framebuffer *fb, int zval, const struct vec2i *p)
{
    if (!fb || !p || p->x < 0 || p->y < 0 || p->x >= fb->x || p->y >= fb->y) return;

    /* Origin is at the left bottom corner of the framebuffer */
    size_t idx = (fb->x * (fb->y - 1 - p->y) + p->x);
    fb->z_buffer[idx] = zval;
}

float
renderer_framebuffer_get_pixel_z(const struct framebuffer *fb, const struct vec2i *p)
{
    if (!fb || !p || p->x < 0 || p->y < 0 || p->x >= fb->x || p->y >= fb->y) return -10.0f;

    size_t idx = (fb->x * (fb->y - 1 - p->y) + p->x);
    return fb->z_buffer[idx];
}

void
renderer_framebuffer_draw_line(struct framebuffer *fb, struct color *col, 
                               struct vec2i *p0, struct vec2i *p1)
{
    /*Check pointers*/
    if (!fb || !col || !fb->color)
        return;
    /* Check boundaries */
    if (p0->x < 0 || p0->x >= fb->x || p1->x < 0 || p1->x >= fb->x || p0->y < 0 || p0->y >= fb->y || p1->y < 0 || p1->y >= fb->y)
        return;

    int bSteep = 0;
    /* If the line is steep, transpose it */
    if (abs(p1->y-p0->y) > abs(p1->x-p0->x)) {
        bSteep = 1;     /* Transpose the framebuffer */
        int_swap(&p0->x, &p0->y);
        int_swap(&p1->x, &p1->y);
    }

    /* Make the line always left to right */
    if (p0->x > p1->x) {
        int_swap(&p1->x, &p0->x);
        int_swap(&p1->y, &p0->y);
    }

    struct vec2i p;
    for (p.x = p0->x; p.x <= p1->x; ++p.x) {
        float t = (p.x - p0->x) / (float)(p1->x - p0->x);
        p.y = p0->y + t * (p1->y - p0->y);
        if (bSteep) {
            renderer_framebuffer_set_pixel(fb, col, &p);
        } else {
            renderer_framebuffer_set_pixel(fb, col, &p);
        }
    }
}

void
renderer_framebuffer_draw_triangle(struct framebuffer *fb, 
                                   const gm_triangle *tr, 
                                   struct vec3f **u,
                                   struct vec2f **uv,
                                   struct texture *tex)
{
    if (!fb || !tr || !u || !uv || !tex) return;

    struct bbox bb = renderer_geometry_triangle_bounding_box(tr);
    if (bb.x == -1 || bb.y == -1 || bb.w == -1 || bb.h == -1) return;

    int num_pixels = bb.h * bb.w, j;
    for (j = 0; j < num_pixels; ++j) {
        struct vec2i p = {.x = bb.x + j % bb.w, .y = bb.y + j / bb.w};

        /* Compute barycentric coordinates */
        struct vec3f bc = renderer_geometry_barycentric_coords(tr, &p);

        if (renderer_geometry_pixel_in_triangle(tr, &p)) {
            /* Compute z value */
            float z_val = 0.0f;
            z_val += (u[0])->z * bc.x;
            z_val += (u[1])->z * bc.y;
            z_val += (u[2])->z * bc.z;

            if (renderer_framebuffer_get_pixel_z(fb, &p) > z_val) {
                struct vec3f n = renderer_geometry_triangle_normal(u);
                float intensity = (n.z);
                if (intensity > 0.0f) {
                    /* Compute texture coordinates */
                    struct vec2f tc = {
                        .x = (uv[0])->x * bc.x + (uv[1])->x * bc.y + (uv[2])->x * bc.z,
                        .y = (uv[0])->y * bc.x + (uv[1])->y * bc.y + (uv[2])->y * bc.z
                    };
                    /* Calculate texture pixel */
                    struct vec2i tci = {
                        .x = tc.x * renderer_texture_get_width(tex),
                        .y = tc.y * renderer_texture_get_height(tex)
                    };
                    /* Retrieve color */
                    struct color *c = renderer_texture_get_color(tex, &tci);
                    if (!c) continue;

                    c->r *= intensity;
                    c->g *= intensity;
                    c->b *= intensity;
                    /* c->r = fabs(intensity) * 255; */
                    /* c->g = fabs(intensity) * 255; */
                    /* c->b = fabs(intensity) * 255; */

                    renderer_framebuffer_set_pixel_z(fb, z_val, &p);
                    renderer_framebuffer_set_pixel(fb, c, &p);
                }
            }
        }
    }
}

void
renderer_framebuffer_clear_color(struct framebuffer *fb, const struct color *col)
{
    if (!fb || !col) return;

    int num_pixels = fb->x * fb->y, j;
    for (j = 0; j < num_pixels; ++j) {
        fb->color[3 * j + 0] = col->r;
        fb->color[3 * j + 1] = col->g;
        fb->color[3 * j + 2] = col->b;
    }
}

void
renderer_framebuffer_clear_z(struct framebuffer *fb)
{
    if (!fb || !fb->z_buffer) return;

    int num_pixels = fb->x * fb->y, j;
    for (j = 0; j < num_pixels; ++j) {
        fb->z_buffer[j] = 10.0f;
    }
}

int 
renderer_framebuffer_save(struct framebuffer *fb, char *filename)
{
    if (!filename || !fb) return -1;
    if (stbi_write_png(filename, fb->x, fb->y, 3, fb->color, 0))
        return -1;

    return 0;
}
