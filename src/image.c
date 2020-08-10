#include <stdlib.h>
#include <image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

struct image {
    int x, y;
    unsigned char* data;
};

struct image* 
renderer_image_create(int x, int y)
{
    struct image* img = malloc(sizeof(struct image));
    if (!img) return NULL;

    img->data = calloc(3 * x * y, 1);
    if (!img->data) {
        free(img);
        return NULL;
    }

    img->x = x;
    img->y = y;

    return img;
}

void
renderer_image_delete(struct image* img)
{
    free(img->data);
    free(img);
}

void
renderer_image_set_pixel(struct image* img, struct color* col, int x, int y)
{
    if (x >= img->x || y >= img->y) return;

    /*Origin is at the left bottom corner of the image*/
    size_t idx = 3 * (img->x * (img->y - 1 - y) + x);
    img->data[idx+0] = col->r;
    img->data[idx+1] = col->g;
    img->data[idx+2] = col->b;
}

int 
renderer_image_save(struct image* img, char* filename)
{
    if (!filename) return -1;
    if (stbi_write_png(filename, img->x, img->y, 3, img->data, 0))
        return -1;

    return 0;
}
