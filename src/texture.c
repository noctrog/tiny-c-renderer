#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#define STB_IMAGE_IMPLEMENTATION
#include <texture.h>
#include <stb_image.h>

/* TODO: support transparency */
struct texture {
    int x, y;               /* Texture size */
    int nChannels;          /* Original image channels, (textures has 3 channels always) */
    float *color;           /* Color data */
};

/**
 *  \brief Creates texture located in the heap from an image file. 
 *
 *  Needs to be freed afterwards.
 *
 *  \param filename: path to image file.
 *  \return Pointer to the new texture.
 */
struct texture *
rndr_texture_create(const char *filename)
{
    if (!filename) return NULL;

    struct texture *tex = malloc(sizeof(struct texture));
    if (!tex) return NULL;

    /* Read texture from file */
    unsigned char* color = stbi_load(filename, &tex->x, &tex->y, &tex->nChannels, 3);
    if (!color) {
        free(tex);
        return NULL;
    }

    /* Convert to floating point texture */
    tex->color = calloc(tex->x * tex->y * tex->nChannels, sizeof(float));
    int i;
    for (i = 0; i < tex->x * tex->y * tex->nChannels; ++i) {
	tex->color[i] = (float)color[i] / 255.0f;
    }

    free(color);
    return tex;
}

/**
 *  \brief Deletes and frees a texture's memory
 *  \param tex: pointer to a pointer to a texture.
 *  \return void
 */
void
rndr_texture_delete(struct texture **tex)
{
    if (!tex) return;
    if ((*tex)->color) free((*tex)->color);
    free(*tex);
    *tex = NULL;
}

/**
 *  \brief Returns the color of a given pixel
 *
 *  Returns NULL if pixel is invalid.
 *
 *  \param tex: pointer to a texture.
 *  \param p: pointer to a vec2i indicating the pixel.
 *  \return Pointer to a float color.
 */
struct colorf *
rndr_texture_get_color(struct texture *tex, struct vec2i *p)
{
    if (!tex || !p) return NULL;
    if (p->x < 0 || p->y < 0 || p->x >= tex->x || p->y >= tex->y) return NULL;

    size_t idx = 3 * (tex->x * (tex->y - 1 - p->y) + p->x);
    struct colorf * col = (struct colorf *) &tex->color[idx];
    return col;
}

/**
 *  \brief Returns texture's width.
 *  \param tex: pointer to a texture.
 *  \return Texture width.
 */
int
rndr_texture_get_width(struct texture *tex)
{
    if (!tex) return -1;
    return tex->x;
}

/**
 *  \brief Returns texture's height.
 *  \param tex: pointer to a texture.
 *  \return Texture height.
 */
int
rndr_texture_get_height(struct texture *tex)
{
    if (!tex) return -1;
    return tex->y;
}
