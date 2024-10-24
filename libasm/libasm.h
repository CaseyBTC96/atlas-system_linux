#ifndef LIBASM_H
#define LIBASM_H

size_t asm_strlen(const char *str);
int asm_strcmp(const char *s1, const char *s2);
char *asm_strchr(const char *s, int c);
void *asm_memcpy(void *dest, const void *src, size_t n);
size_t asm_putc(int c);

#endif