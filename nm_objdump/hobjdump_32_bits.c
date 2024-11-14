#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "hobjdump.h"


void print_section_contents_32(Elf32_Shdr *shdr, char *map, int is_big_endian)
{
	size_t section_size;
	size_t i, j;
	unsigned char *section_data;

	section_data = (unsigned char *)(map + my_be32toh(shdr->sh_offset,
					is_big_endian));
	section_size = my_be32toh(shdr->sh_size, is_big_endian);

	for (i = 0; i < section_size; i += 16)
	{
		printf(" %04x", (int)(my_be32toh(shdr->sh_addr, is_big_endian) + i));
		for (j = 0; j < 16; j++)
		{
			if (i + j < section_size)
			{
				if (j % 4 == 0)
				{
					printf(" ");/* espacio entre bloques */
				}
				printf("%02x", section_data[i + j]);
			}
			else
			{
				if (j % 4 == 0)
				{
					printf(" ");/* espacio entre bloques */
				}
				printf("  ");
			}
		}
		printf("  ");
		for (j = 0; j < 16; j++)
		{
			if (i + j < section_size)
			{
				char c = section_data[i + j];

				if (c >= 32 && c <= 126)
				{
					printf("%c", c);
				}
				else
				{
					printf(".");
				}
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}


void print_sections_32(Elf32_Ehdr *ehdr, int is_big_endian, void *map)
{
	int i;
	Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)map + my_be32toh(ehdr->e_shoff,
	is_big_endian));
	char *string_table = (char *)map +
	my_be32toh(shdr[my_be16toh(ehdr->e_shstrndx, is_big_endian)].sh_offset,
	is_big_endian);

	for (i = 1; i < my_be16toh(ehdr->e_shnum, is_big_endian); i++)
	{

		char *section_name = string_table + my_be32toh(shdr[i].sh_name,
		is_big_endian);

		/* Evita estas secciones */
		if (strcmp(section_name, ".bss") == 0 ||
			strcmp(section_name, ".shstrtab") == 0 ||
			strcmp(section_name, ".symtab") == 0 ||
			strcmp(section_name, ".tm_clone_table") == 0 ||/* solaris */
			strcmp(section_name, ".rel.text") == 0 ||
			strcmp(section_name, ".rel.data") == 0 ||
			strcmp(section_name, ".strtab") == 0)
			{
			continue;
			}
		printf("Contents of section %s:\n", section_name);
		print_section_contents_32(&shdr[i], map, is_big_endian);
	}
}

void print_elf_header_32(Elf32_Ehdr *ehdr, const char *filename, void *map)
{
	const char *formatted_filename = filename;
	int flag_printed = 0;
	unsigned long flags_interp = 0;
	int is_big_endian = 0;

	if (formatted_filename[0] == '.' && formatted_filename[1] == '/')
		formatted_filename += 2;

	is_big_endian = (ehdr->e_ident[EI_DATA] == ELFDATA2MSB);
	printf("%s:     file format %s\n", formatted_filename,
			(is_big_endian) ? "elf32-big" : "elf32-i386");
	printf("architecture: %s,", (is_big_endian) ? "UNKNOWN!" : "i386");

	if (my_be16toh(ehdr->e_type, is_big_endian) == ET_EXEC)
		flags_interp |= EXEC_P;
	if (my_be16toh(ehdr->e_type, is_big_endian) == ET_REL)
		flags_interp |= HAS_RELOC;
	if (my_be16toh(ehdr->e_type, is_big_endian) == ET_DYN)
		flags_interp |= DYNAMIC;

	flags_interp |= (ehdr->e_shnum > 0) ? HAS_SYMS : 0;
	flags_interp |= (ehdr->e_phnum > 0) ? D_PAGED : 0;

	printf(" flags 0x%08lx:\n", flags_interp);
	print_flag(&flag_printed, flags_interp, EXEC_P, "EXEC_P");
	print_flag(&flag_printed, flags_interp, HAS_RELOC, "HAS_RELOC");
	print_flag(&flag_printed, flags_interp, HAS_SYMS, "HAS_SYMS");
	print_flag(&flag_printed, flags_interp, DYNAMIC, "DYNAMIC");
	print_flag(&flag_printed, flags_interp, D_PAGED, "D_PAGED");
	printf("\n");
	printf("start address 0x%08lx\n",
	(unsigned long)(my_be32toh(ehdr->e_entry, is_big_endian)));
	printf("\n");
	print_sections_32(ehdr, is_big_endian, map);
}

int analyze_32bit_elf(Elf32_Ehdr *ehdr, const char *filename, void *map)
{
	if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		printf("\n");
		print_elf_header_32(ehdr, filename, map);
	}
	else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		printf("\n");
		print_elf_header_32(ehdr, filename, map);
	}
	else
	{
		printf("endianness unknown.\n");
	}
	return (0);
}
