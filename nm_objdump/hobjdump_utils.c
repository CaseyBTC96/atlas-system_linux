#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "hobjdump.h"



void print_flag(int *flag_printed, unsigned long flags, unsigned long flag,
				const char *flag_name)
{
	if (flags & flag)
	{
		if (*flag_printed)
			printf(", ");
		printf("%s", flag_name);
		*flag_printed = 1;
	}
}

uint32_t my_be32toh(uint32_t value, int is_big_endian)
{
	if (is_big_endian)
		return (((value >> 24) & 0xFF) |
				((value >> 8) & 0xFF00) |
				((value & 0xFF00) << 8) |
				((value & 0xFF) << 24));
	else
		return (value);

}

uint16_t my_be16toh(uint16_t value, int is_big_endian)
{
	if (is_big_endian)
		return (((value >> 8) & 0xFF) | ((value & 0xFF) << 8));
	else
		return (value);
}
