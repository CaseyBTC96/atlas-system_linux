#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

/* Global mutex for thread safety */
static pthread_mutex_t tprintf_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * tprintf_init - Initialize mutex for thread-safe printing
 */
__attribute__((constructor))
void tprintf_init(void)
{
	pthread_mutex_init(&tprintf_mutex, NULL);
}

/**
 * tprintf_cleanup - Cleanup mutex after program completion
 */
__attribute__((destructor))
void tprintf_cleanup(void)
{
	pthread_mutex_destroy(&tprintf_mutex);
}

/**
 * tprintf - Thread-safe printf with thread ID
 * @format: Format string
 * Return: Number of characters printed
 */
int tprintf(char const *format, ...)
{
	va_list args;
	int printed_chars;

	pthread_mutex_lock(&tprintf_mutex);

	printed_chars = printf("[%lu] ", pthread_self());

	va_start(args, format);
	printed_chars += vprintf(format, args);
	va_end(args);

	pthread_mutex_unlock(&tprintf_mutex);

	return (printed_chars);
}
