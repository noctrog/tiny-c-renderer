#include <stdlib.h>
#include <math.h>
#include <image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

struct image {
    int x, y;
    unsigned char* data;
};

static inline void int_swap(int *a, int *b)
{
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a = *a ^ *b;
}

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
renderer_image_delete(struct image **img)
{
    free((*img)->data);
    free(*img);
    *img = NULL;
}

void
renderer_image_set_pixel(struct image *img, struct color *col, int x, int y)
{
    if (x >= img->x || y >= img->y) return;

    /*Origin is at the left bottom corner of the image*/
    size_t idx = 3 * (img->x * (img->y - 1 - y) + x);
    img->data[idx+0] = col->r;
    img->data[idx+1] = col->g;
    img->data[idx+2] = col->b;
}

void
renderer_image_draw_line(struct image *img, struct color *col, int x0, int y0, int x1, int y1)
{
    /*Check pointers*/
    if (!img || !col || !img->data)
        return;
    /* Check boundaries */
    if (x0 < 0 || x0 >= img->x || x1 < 0 || x1 >= img->x || y0 < 0 || y0 >= img->y || y1 < 0 || y1 >= img->y)
        return;

    int bSteep = 0;
    /* If the line is steep, transpose it */
    if (abs(y1-y0) > abs(x1-x0)) {
        bSteep = 1;     /* Transpose the image */
        int_swap(&x0, &y0);
        int_swap(&x1, &y1);
    }

    /* Make the line always left to right */
    if (x0 > x1) {
        int_swap(&x1, &x0);
        int_swap(&y1, &y0);
    }

    for (int x = x0; x <= x1; ++x) {
        float t = (x - x0) / (float)(x1 - x0);
        int y = y0 + t * (y1 - y0);
        if (bSteep) {
            renderer_image_set_pixel(img, col, y, x);
        } else {
            renderer_image_set_pixel(img, col, x, y);
        }
    }
}

int 
renderer_image_save(struct image *img, char *filename)
{
    if (!filename) return -1;
    if (stbi_write_png(filename, img->x, img->y, 3, img->data, 0))
        return -1;

    return 0;
}
