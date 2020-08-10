#include <image.h>

int main(int argc, char *argv[])
{
    struct image* img = renderer_image_create(100, 100);
    struct color white = {0xff, 0xff, 0xff};
    renderer_image_set_pixel(img, &white, 49, 49);
    renderer_image_save(img, "prueba.png");
    renderer_image_delete(img);
    return 0;
}
