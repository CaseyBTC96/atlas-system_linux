#include "multithreading.h"

#define NUM_THREADS 4

typedef struct thread_data {
    img_t *img_blur;
    img_t const *img;
    kernel_t const *kernel;
    size_t start_y;
    size_t end_y;
} thread_data_t;

static void *thread_func(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    blur_portion_t portion = {
        .img = data->img,
        .img_blur = data->img_blur,
        .kernel = data->kernel,
        .y = data->start_y,
        .h = data->end_y - data->start_y
    };
    blur_portion(&portion);
    return NULL;
}

void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    size_t portion_height = img->h / NUM_THREADS;
    size_t i;

    for (i = 0; i < NUM_THREADS; i++) {
        thread_data[i].img_blur = img_blur;
        thread_data[i].img = img;
        thread_data[i].kernel = kernel;
        thread_data[i].start_y = i * portion_height;
        thread_data[i].end_y = (i == NUM_THREADS - 1) ? 
                              img->h : (i + 1) * portion_height;

        pthread_create(&threads[i], NULL, thread_func, &thread_data[i]);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}
