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
        int j;
        for (j = 0; j < 3; ++j) {
            struct vec3f *v0 = renderer_model_face_vec(m, i, j);
            struct vec3f *v1 = renderer_model_face_vec(m, i, (j+1)%3);

            if (v0 == NULL || v1 == NULL) continue;

            /*Resize to fill image*/
            int x0 = (v0->x + 1.0f) * width / 2.0f;
            int x1 = (v1->x + 1.0f) * width / 2.0f;
            int y0 = (v0->y + 1.0f) * height / 2.0f;
            int y1 = (v1->y + 1.0f) * height / 2.0f;

            renderer_image_draw_line(img, &white, x0, y0, x1, y1);
        }
    }

    renderer_image_save(img, "media/lesson-1.png");
    renderer_image_delete(&img);

    renderer_model_delete(&m);
    return 0;
}
