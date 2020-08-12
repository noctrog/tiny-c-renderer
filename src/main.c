#include <stdlib.h>
#include <image.h>
#include <model.h>
#include <geometry.h>

int main(int argc, char *argv[])
{
    const int width = 1000;
    const int height = 1000;

    struct image* img = renderer_image_create(width, height);
    struct color white = {0xff, 0xff, 0xff};

    struct model *m = renderer_model_create("media/models/african_head.obj");
    if (!m) exit(-1);

    int i, nfaces;
    nfaces = renderer_model_nfaces(m);
    for (i = 0; i < nfaces; ++i) {
        gm_triangle tr;
        struct vec3f *u[3];
        struct color col;

        /* Construct triangle*/
        int j;
        for (j = 0; j < 3; ++j) {
            u[j] = renderer_model_face_vec(m, i, j);

            if (u[j] == NULL) continue;

            /*Resize to fill image*/
            int x = (u[j]->x + 1.0f) * width / 2.0f;
            int y = (u[j]->y + 1.0f) * height / 2.0f;
            
            tr[j].x = x;
            tr[j].y = y;
        }

        /* Compute normal */
        struct vec3f u1 = {.x = u[1]->x - u[0]->x, .y = u[1]->y - u[0]->y, .z = u[1]->z - u[0]->z };
        struct vec3f u2 = {.x = u[2]->x - u[1]->x, .y = u[2]->y - u[1]->y, .z = u[2]->z - u[1]->z };
        struct vec3f n  = renderer_geometry_cross(&u1, &u2);
        renderer_geometry_normalize(&n);
        float intensity = (n.z) / 3.0f;
        if (intensity > 0.0f) {
            col.r = abs((int)(intensity * 255));
            col.g = abs((int)(intensity * 255));
            col.b = abs((int)(intensity * 255));

            renderer_image_draw_triangle(img, &col, &tr);
        }
    }

    renderer_image_save(img, "media/lesson-2.png");
    renderer_image_delete(&img);

    renderer_model_delete(&m);
    return 0;
}
