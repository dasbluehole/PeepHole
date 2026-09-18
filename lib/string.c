/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  string implementation                                  |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
// common.c -- Defines some global functions.
//             From JamesM's kernel development tutorials.

#include "string.h"


// Copy len bytes from src to dest.
void *memcpy(void *dest, const void *src, size_t len)
{
    // TODO: implement this yourself!
	unsigned char *d=(unsigned char *)dest;
	unsigned char *s=(unsigned char *)src;
	for(;len>0;len--)
	{
		*d++= *s++;
	}
	return d;
}

// Write len copies of val into dest.
void *memset(void *dest, int val, size_t len)
{
    // TODO: implement this yourself!
	unsigned char *d=(unsigned char *)dest;
	for(;len>0;len--)
	{
		*d++=val;
	}
	return d;
}
//Returns a pointer to first occourance of 'c' in 's'
void *memchr (const void *s, int c, size_t n)
{
  if (n)
  {
    char *p = (char *)s;
    char cc = c;
    do {
      if (*p == cc)
	return p;
      p++;
    } while (--n != 0);
  }
  return 0;
}
//compares 2 memory blocks; returns non-zero if un-equal else zero
int memcmp(const void *s1, const void *s2, size_t n)
{
  if (n != 0)
  {
    const unsigned char *p1 = s1, *p2 = s2;

    do {
      if (*p1++ != *p2++)
	return (*--p1 - *--p2);
    } while (--n != 0);
  }
  return 0;
}
//concatenate strings
char *strcat(char *s, const char *append)
{
  char *save = s;

  for (; *s; ++s);
  while ((*s++ = *append++));
  return save;
}
//search for a char 'c' in 's'; returns pointer of first occourance; if not found then zero
char *strchr(const char *s, int c)
{
  char cc = c;
  while (*s)
  {
    if (*s == cc)
      return (char *)s;
    s++;
  }
  if (cc == 0)
    return (char *)s;
  return 0;
}
//compares 2 strings
int strcmp(const char *s1, const char *s2)
{
  while (*s1 == *s2)
  {
    if (*s1 == 0)
      return 0;
    s1++;
    s2++;
  }
  return *(unsigned const char *)s1 - *(unsigned const char *)(s2);
}
//string copy
char * strcpy(char *to, const char *from)
{
  char *save = to;

  for (; (*to = *from); ++from, ++to);
  return save;
}

size_t strcspn(const char *s1, const char *s2)
{
  const char *p, *spanp;
  char c, sc;

  for (p = s1;;)
  {
    c = *p++;
    spanp = s2;
    do {
      if ((sc = *spanp++) == c)
	return p - 1 - s1;
    } while (sc != 0);
  }
  /* NOTREACHED */
}
//string length
size_t strlen(const char *str)
{
  const char *s;

  if (str == 0)
    return 0;
  for (s = str; *s; ++s);
  return s-str;
}
//strnlen checks UPTO MAX_LEN for '\0' if length is less then return length else MAX_LEN
size_t strnlen(const char *str, size_t MAX_LEN)
{
	size_t len;
	len=strlen(str);
	if (len+1< MAX_LEN)
	return len;
	return MAX_LEN;
}

//string concat
char *strncat(char *dst, const char *src, size_t n)
{
  if (n != 0)
  {
    char *d = dst;
    const char *s = src;

    while (*d != 0)
      d++;
    do {
      if ((*d = *s++) == 0)
	break;
      d++;
    } while (--n != 0);
    *d = 0;
  }
  return dst;
}
//string comp upto n chars
int strncmp(const char *s1, const char *s2, size_t n)
{

  if (n == 0)
    return 0;
  do {
    if (*s1 != *s2++)
      return *(unsigned const char *)s1 - *(unsigned const char *)--s2;
    if (*s1++ == 0)
      break;
  } while (--n != 0);
  return 0;
}
//string copy upto n chars
char *strncpy(char *dst, const char *src, size_t n)
{
  if (n != 0) {
    char *d = dst;
    const char *s = src;

    do {
      if ((*d++ = *s++) == 0)
      {
	while (--n != 0)
	  *d++ = 0;
	break;
      }
    } while (--n != 0);
  }
  return dst;
}

char *strpbrk(const char *s1, const char *s2)
{
  const char *scanp;
  int c, sc;

  while ((c = *s1++) != 0)
  {
    for (scanp = s2; (sc = *scanp++) != 0;)
      if (sc == c)
	return (char *)(s1 - 1);
  }
  return 0;
}

char *strrchr(const char *s, int c)
{
  char cc = c;
  const char *sp=(char *)0;
  while (*s)
  {
    if (*s == cc)
      sp = s;
    s++;
  }
  if (cc == 0)
    sp = s;
  return (char *)sp;
}

size_t strspn(const char *s1, const char *s2)
{
  const char *p = s1, *spanp;
  char c, sc;

 cont:
  c = *p++;
  for (spanp = s2; (sc = *spanp++) != 0;)
    if (sc == c)
      goto cont;
  return (p - 1 - s1);
}

char *strstr(const char *s, const char *find)
{
  char c, sc;
  size_t len;

  if ((c = *find++) != 0)
  {
    len = strlen(find);
    do {
      do {
	if ((sc = *s++) == 0)
	  return 0;
      } while (sc != c);
    } while (strncmp(s, find, len) != 0);
    s--;
  }
  return (char *)s;
}

char *strtok(char *s, const char *delim)
{
  const char *spanp;
  int c, sc;
  char *tok;
  static char *last;


  if (s == NULL && (s = last) == NULL)
    return (NULL);

  /*
   * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
   */
 cont:
  c = *s++;
  for (spanp = delim; (sc = *spanp++) != 0;) {
    if (c == sc)
      goto cont;
  }

  if (c == 0) {			/* no non-delimiter characters */
    last = NULL;
    return (NULL);
  }
  tok = s - 1;

  /*
   * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
   * Note that delim must have one NUL; we stop if we see that, too.
   */
  for (;;) {
    c = *s++;
    spanp = delim;
    do {
      if ((sc = *spanp++) == c) {
	if (c == 0)
	  s = NULL;
	else
	  s[-1] = 0;
	last = s;
	return (tok);
      }
    } while (sc != 0);
  }
  /* NOTREACHED */
}

size_t strxfrm (char *dst, char *src, size_t n)
{
  size_t r = 0;
  int c;

  if (n != 0) {
    while ((c = *src++) != 0)
    {
      r++;
      if (--n == 0)
      {
	while (*src++ != 0)
	  r++;
	break;
      }
      *dst++ = c;
    }
    *dst = 0;
  }
  return r;
}

char *strlwr (char *s) {
  char *p;

  p=s;
  while (*p) {
    if ((*p>='A') && (*p<='Z'))
      *p += 'a'-'A';
    p++;
  };

  return s;
}

char *strupr (char *s) {
  char *p;

  p=s;
  while (*p) {
    if ((*p>='a') && (*p<='z'))
      *p -= 'a'-'A';
    p++;
  };

  return s;
}
