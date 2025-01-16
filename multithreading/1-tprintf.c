#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

/**
 * tprintf - Prints a formatted string with thread ID
 * @format: Format string to print
 * Return: Number of characters printed
 */
int tprintf(char const *format, ...)
{
	va_list args;
	int printed_chars;

	printed_chars = printf("[%lu] ", pthread_self());

	va_start(args, format);
	printed_chars += vprintf(format, args);
	va_end(args);

	return (printed_chars);
}
