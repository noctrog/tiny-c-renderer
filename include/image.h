#ifndef IMAGE_H_P2AJDWBK
#define IMAGE_H_P2AJDWBK

struct image;
struct color {
    unsigned char r, g, b;
};


struct image* 
renderer_image_create(int x, int y);

void
renderer_image_delete(struct image** img);

void
renderer_image_set_pixel(struct image* img, struct color* col, int x, int y);

void
renderer_image_draw_line(struct image *img, struct color *col, int x0, int y0, int x1, int y1);

int 
renderer_image_save(struct image* img, char* filename);

#endif /* end of include guard: IMAGE_H_P2AJDWBK */
