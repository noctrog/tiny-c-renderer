#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#define STB_IMAGE_IMPLEMENTATION
#include <texture.h>
#include <stb_image.h>

/* TODO: support transparency */
struct texture {
    int x, y;               /* Texture size */
    int nChannels;          /* Original image channels, (textures has 3 channels always) */
    unsigned char *color;   /* Color data */
};

struct texture *
renderer_texture_create(const char *filename)
{
    if (!filename) return NULL;

    struct texture *tex = malloc(sizeof(struct texture));
    if (!tex) return NULL;

    tex->color = stbi_load(filename, &tex->x, &tex->y, &tex->nChannels, 3);
    if (!tex->color) {
        free(tex);
        return NULL;
    }
    return tex;
}

void
renderer_texture_delete(struct texture **tex)
{
    if (!tex) return;
    if ((*tex)->color) free((*tex)->color);
    free(*tex);
    *tex = NULL;
}


struct color *
renderer_texture_get_color(struct texture *tex, struct vec2i *p)
{
    if (!tex || !p) return NULL;
    if (p->x < 0 || p->y < 0 || p->x >= tex->x || p->y >= tex->y) return NULL;

    size_t idx = 3 * (tex->x * (tex->y - 1 - p->y) + p->x);
    struct color * col = (struct color *) &tex->color[idx];
    return col;
}

int
renderer_texture_get_width(struct texture *tex)
{
    if (!tex) return -1;
    return tex->x;
}

int
renderer_texture_get_height(struct texture *tex)
{
    if (!tex) return -1;
    return tex->y;
}
