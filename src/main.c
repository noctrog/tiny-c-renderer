#include <stdlib.h>
#include <stdio.h>

#include <framebuffer.h>
#include <model.h>
#include <geometry.h>

int main(int argc, char *argv[])
{
    const int WIDTH = 1000;
    const int HEIGHT = 1000;
    const struct color col_back = {.r = 0x28, .g = 0x28, .b = 0x28};

    struct framebuffer* fb = rndr_framebuffer_create(WIDTH, HEIGHT);
    rndr_framebuffer_clear_color(fb, &col_back);
    rndr_framebuffer_clear_z(fb);
    
    struct model *m = rndr_model_create("media/models/african_head/african_head.obj");
    if (!m) {
      printf("ERROR: Couldn't read the model file\n");
      exit(-1);
    }
    struct texture *tex = rndr_texture_create("media/models/african_head/african_head_diffuse.tga");
    if (!tex) {
        printf("Problema al leer la imagen\n");
        exit(-1);
    }

    int i, nfaces;
    nfaces = rndr_model_nfaces(m);
    for (i = 0; i < nfaces; ++i) {
        gm_triangle tr;             /* Current triangle, screen space */
        vec3 *u[3];                  /* Triangle 3D vertices */
        vec2 *uv[3];                 /* Texture coordinates */
        vec3 *vn[3];                 /* Normals */

        /* Construct triangle */
        int j;
        for (j = 0; j < 3; ++j) {
            u[j] = rndr_model_face_vec(m, i, j);
            if (u[j] == NULL) continue;

            uv[j] = rndr_model_face_uv(m, i, j);
            vn[j] = rndr_model_face_vn(m, i, j);

            /* Resize to fill framebuffer */
            int x = ((*u[j])[0] + 1.0f) * WIDTH / 2.0f;
            int y = ((*u[j])[1] + 1.0f) * HEIGHT / 2.0f;

            tr[j].x = x;
            tr[j].y = y;
        }

        rndr_framebuffer_draw_triangle(fb, &tr, u, uv, vn, tex);
    }

    rndr_framebuffer_save(fb, "media/lesson-3.png");

    rndr_framebuffer_delete(&fb);
    rndr_texture_delete(&tex);
    rndr_model_delete(&m);
    return 0;
}
