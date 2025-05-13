#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <sys/types.h>

// ft_str
size_t			ft_strlen(const char *s);
char			*ft_strdup(const char *s);
char			*ft_strrchr(const char *s, int c);
char			*ft_strjoin(char const *s1, char const *s2);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);

// ft_str_cmp
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_find_last_of(char *str, char find);

// ft_is_str
bool			ft_isalpha(char c);
bool			ft_isdigit(char c);

// ft_ato
int				ft_atoi(const char *nptr);
long			ft_atol(const char *nptr);
unsigned long	ft_atoul(const char *nptr);

// ft_mem
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			*ft_memset(void *s, int c, size_t n);

char			*ft_fuse(size_t nb_of_element, ...);
