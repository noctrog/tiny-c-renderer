struct image;
struct color {
    unsigned char r, g, b;
};


struct image* 
renderer_image_create(int x, int y);

void
renderer_image_delete(struct image* img);

void
renderer_image_set_pixel(struct image* img, struct color* col, int x, int y);

int 
renderer_image_save(struct image* img, char* filename);
