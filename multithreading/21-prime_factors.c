#include "multithreading.h"
#include <stdlib.h>

/**
 * add_factor - Adds a factor to the list
 * @factors: Pointer to the list of factors
 * @value: The factor value to add
 * Return: 0 on success, 1 on failure
 */
int add_factor(list_t *factors, unsigned long value)
{
	unsigned long *factor = malloc(sizeof(unsigned long));

	if (!factor)
		return (1);

	*factor = value;
	list_add(factors, factor);
	return (0);
}

/**
 * handle_even_factors - Handles factorization of even numbers
 * @factors: Pointer to the list of factors
 * @num: Pointer to the number being factorized
 * Return: 0 on success, 1 on failure
 */
int handle_even_factors(list_t *factors, unsigned long *num)
{
	while (*num % 2 == 0)
	{
		if (add_factor(factors, 2))
			return (1);

		*num /= 2;
	}
	return (0);
}

/**
 * handle_odd_factors - Handles factorization of odd numbers
 * @factors: Pointer to the list of factors
 * @num: Pointer to the number being factorized
 * Return: 0 on success, 1 on failure
 */
int handle_odd_factors(list_t *factors, unsigned long *num)
{
	for (unsigned long i = 3; i * i <= *num; i += 2)
	{
		while (*num % i == 0)
		{
			if (add_factor(factors, i))

				return (1);

			*num /= i;
		}
	}
	return (0);
}

/**
 * prime_factors - Factorizes a number into a list of prime factors
 * @s: String representation of the number to factorize
 * Return: List of prime factors
 */
list_t *prime_factors(char const *s)
{
	list_t *factors = malloc(sizeof(list_t));

	if (!factors)
		return (NULL);

	list_init(factors);
	unsigned long num = strtoul(s, NULL, 10);

	if (handle_even_factors(factors, &num)
			|| handle_odd_factors(factors, &num))
	{
		list_destroy(factors, free);
		free(factors);

		return (NULL);
	}

	if (num > 2)
	{
		if (add_factor(factors, num))
		{
			list_destroy(factors, free);
			free(factors);

			return (NULL);
		}
	}

	return (factors);
}
