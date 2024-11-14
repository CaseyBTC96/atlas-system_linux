#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "hnm.h"

void process_symbols_32bit(Elf32_Ehdr *ehdr, void *map)
{
	int i;
	int j;
	int num_syms;

	Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)map + ehdr->e_shoff);
	char *strtab = (char *)((char *)map + shdr[ehdr->e_shstrndx].sh_offset);
	Elf32_Sym *symtab = NULL;
	char type;

	printf("Value    Type Name\n");

	for (i = 0; i < ehdr->e_shnum; i++)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
		{
			symtab = (Elf32_Sym *)((char *)map + shdr[i].sh_offset);
			num_syms = shdr[i].sh_size / sizeof(Elf32_Sym);

			for (j = 0; j < num_syms; j++)
			{
				Elf32_Sym *symbol = &symtab[j];
				char *symbol_name = strtab + symbol->st_name;
				char symbol_type = ELF32_ST_TYPE(symbol->st_info);
				char symbol_bind = ELF32_ST_BIND(symbol->st_info);

				if (symbol_bind == STB_GLOBAL)
				{
					type = (symbol_type == STT_OBJECT) ? 'B' : 'T';
				}
				else if (symbol_bind == STB_WEAK)
				{
					type = (symbol_type == STT_OBJECT) ? 'W' : 'V';
				}
				else
				{
					type = 's';
				}

				printf("%08x %c %s\n", (unsigned int)symbol->st_value, type, symbol_name);
			}
		}
	}
}


int analyze_32bit_elf(Elf32_Ehdr *ehdr, void *map)
{

	printf("32 bits, ");
	if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		printf("little-endian.\n");
		process_symbols_32bit(ehdr, map);
	}
	else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		printf("big-endian.\n");
	}
	else
	{
		printf("endianness unknown.\n");
	}

	return (0);
}

int analyze_64bit_elf(Elf64_Ehdr *ehdr)
{

	printf("64 bits, ");
	if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		printf("little-endian.\n");
	}
	else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		printf("big-endian.\n");
	}
	else
	{
		printf("endianness unknown.\n");
	}
	/* llamar funcion se ocupara procesar sinbolos y salida */

	return (0);
}

int analyze_file(const char *filename)
{
	int fd;
	void *map;
	size_t file_size;
	Elf32_Ehdr *ehdr32;
	Elf64_Ehdr *ehdr64;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}

	file_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	map = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED)
	{
		perror("mmap");
		return (1);
	}
	ehdr32 = (Elf32_Ehdr *)map;
	ehdr64 = (Elf64_Ehdr *)map;
	if (ehdr32->e_ident[EI_CLASS] == ELFCLASS32)
	{
		return (analyze_32bit_elf(ehdr32, map));

	}
	else if (ehdr64->e_ident[EI_CLASS] == ELFCLASS64)
	{
		return (analyze_64bit_elf(ehdr64));
	}
	else
	{
		fprintf(stderr, "No es un archivo ELF válido.\n");
		return (1);
	}
	close(fd);
	munmap(map, file_size);
	return (0);
}

int main(int argc, char *argv[])
{
	int i;

	if (argc < 2)
	{
		fprintf(stderr, "hnm: %s [objfile...]\n", argv[0]);
		return (1);
	}

	for (i = 1; i < argc; i++)
	{
		if (analyze_file(argv[i]) != 0)
		{
			return (1);
		}
	}

	return (0);
}
