#include <stdlib.h>
#include <stdio.h>

#include <framebuffer.h>
#include <model.h>
#include <geometry.h>

int main(int argc, char *argv[])
{
    const int width = 1000;
    const int height = 1000;
    const struct color col_back = {.r = 0x28, .g = 0x28, .b = 0x28};

    struct framebuffer* fb = renderer_framebuffer_create(width, height);
    renderer_framebuffer_clear_color(fb, &col_back);
    renderer_framebuffer_clear_z(fb);

    struct model *m = renderer_model_create("media/models/african_head.obj");
    if (!m) exit(-1);
    struct texture *tex = renderer_texture_create("media/models/african_head_diffuse.tga");
    if (!tex) {
        printf("Problema al leer la imagen\n");
        exit(-1);
    }

    int i, nfaces;
    nfaces = renderer_model_nfaces(m);
    for (i = 0; i < nfaces; ++i) {
        gm_triangle tr;             /* Current triangle, screen space */
        struct vec3f *u[3];         /* Triangle 3D vertices */
        struct vec2f *uv[3];        /* Texture coordinates */

        /* Construct triangle */
        int j;
        for (j = 0; j < 3; ++j) {
            u[j] = renderer_model_face_vec(m, i, j);
            if (u[j] == NULL) continue;

            uv[j] = renderer_model_face_uv(m, i, j);

            /*Resize to fill framebuffer*/
            int x = (u[j]->x + 1.0f) * width / 2.0f;
            int y = (u[j]->y + 1.0f) * height / 2.0f;

            tr[j].x = x;
            tr[j].y = y;
        }

        renderer_framebuffer_draw_triangle(fb, &tr, u, uv, tex);
    }

    renderer_framebuffer_save(fb, "media/lesson-3.png");

    renderer_framebuffer_delete(&fb);
    renderer_texture_delete(&tex);
    renderer_model_delete(&m);
    return 0;
}
