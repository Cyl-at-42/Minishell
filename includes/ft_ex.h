#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "ft_utils.h"

bool	ft_realloc(void **src, size_t src_size, size_t new_size);
char	*ft_copy_substr(char *dest, char *substr_start, char *substr_end);
int		ft_strcmp_cu(const char *s1, const char *s2);
char	**ft_split_ex(char const *s, char c, size_t extrachars);
void	ft_m_putnbr(char *dest, unsigned short nb);
void	ft_safe_close(int	*fd_ptr);
