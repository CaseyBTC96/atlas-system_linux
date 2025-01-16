#ifndef BLUR_IMAGE_H
#define BLUR_IMAGE_H

#include "multithreading.h"

void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel);

#endif
