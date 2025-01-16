#include "multithreading.h"
#include <stdlib.h>

/**
 * apply_gaussian_blur - Applies Gaussian blur to a single pixel
 * @portion: Portion of the image to blur
 * @x: X coordinate of the pixel
 * @y: Y coordinate of the pixel
 * Return: Blurred pixel value
 */
static pixel_t apply_gaussian_blur(blur_portion_t const *portion, size_t x, size_t y)
{
    pixel_t blurred = {0, 0, 0};
    float r = 0, g = 0, b = 0;
    size_t i, j;
    int kx, ky, px;  /* Removed unused 'py' variable */
    size_t k_size = portion->kernel->size;
    size_t half_k = k_size / 2;

    for (i = 0; i < k_size; i++)
    {
        for (j = 0; j < k_size; j++)
        {
            kx = x - half_k + j;
            ky = y - half_k + i;

            if (kx >= 0 && (size_t)kx < portion->img->w &&
                ky >= 0 && (size_t)ky < portion->img->h)
            {
                px = ky * portion->img->w + kx;
                r += portion->img->pixels[px].r * portion->kernel->matrix[i][j];
                g += portion->img->pixels[px].g * portion->kernel->matrix[i][j];
                b += portion->img->pixels[px].b * portion->kernel->matrix[i][j];
            }
        }
    }

    blurred.r = (unsigned char)(r > 255 ? 255 : r);
    blurred.g = (unsigned char)(g > 255 ? 255 : g);
    blurred.b = (unsigned char)(b > 255 ? 255 : b);

    return blurred;
}

/**
 * blur_portion - Blurs a portion of an image using Gaussian blur
 * @portion: Pointer to blur_portion_t structure
 */
void blur_portion(blur_portion_t const *portion)
{
    size_t x, y;
    size_t end_x = portion->x + portion->w;
    size_t end_y = portion->y + portion->h;

    for (y = portion->y; y < end_y; y++)
    {
        for (x = portion->x; x < end_x; x++)
        {
            portion->img_blur->pixels[y * portion->img->w + x] = 
                apply_gaussian_blur(portion, x, y);
        }
    }
}
