#ifndef STRING_H
#define STRING_H

#include "types.h"

void *memset(void *dst, char c, uint32 n);

void *memcpy(void *dst, const void *src, uint32 n);

int memcmp(uint8 *s1, uint8 *s2, uint32 n);

int strlen(const char *s);

int strcmp(const char *s1, char *s2);

int strcpy(char *dst, const char *src);

void strcat(char *dest, const char *src);

int isspace(char c);

int isalpha(char c);
char upper(char c);
char lower(char c);

void itoa(char *buf, int base, int d);

#endif
