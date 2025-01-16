#include <stdio.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * thread_entry - Entry point for a new thread
 * @arg: Pointer to a string to be printed
 *
 * Return: NULL
 */
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(NULL);
}
