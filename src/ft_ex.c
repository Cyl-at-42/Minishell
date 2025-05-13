#include "ft_ex.h"

// = libftex ==================================================================

bool	ft_realloc(void **src, size_t src_size, size_t new_size)
{
	unsigned char	*new_memblock;

	new_memblock = (unsigned char *)malloc(new_size);
	if (!new_memblock)
		return (false);
	if (src_size > new_size)
		src_size = new_size;
	ft_memcpy((void *)new_memblock, *src, src_size);
	free(*src);
	*src = new_memblock;
	return (true);
}

char	*ft_copy_substr(char *dest, char *substr_start, char *substr_end)
{
	char	*dest2;

	dest2 = dest;
	while (substr_start < substr_end)
		*dest++ = *substr_start++;
	*dest++ = 0;
	return (dest2);
}

int	ft_strcmp_cu(const char *s1, const char *s2)
{
	size_t			index;
	unsigned char	a;
	unsigned char	b;

	index = 0;
	while ((s1[index] != '\0' || s2[index] != '\0'))
	{
		a = (unsigned char)s1[index];
		b = (unsigned char)s2[index];
		if (a >= 'a' && a <= 'z')
			a -= 32;
		if (b >= 'a' && b <= 'z')
			b -= 32;
		if (a != b)
			return (a - b);
		index++;
	}
	return (0);
}

// Convert an unsigned short int from 0 to 999 to ASCII into mem
void	ft_m_putnbr(char *dest, unsigned short nb)
{
	dest[0] = '0' + nb % 10;
	dest[1] = 0;
	nb /= 10;
	if (!nb)
		return ;
	dest[1] = dest[0];
	dest[2] = 0;
	dest[0] = '0' + nb % 10;
	nb /= 10;
	if (!nb)
		return ;
	dest[2] = dest[1];
	dest[1] = dest[0];
	dest[3] = 0;
	dest[0] = '0' + nb % 10;
}

void	ft_safe_close(int *fd_ptr)
{
	if (*fd_ptr <= 0)
		return ;
	close(*fd_ptr);
	*fd_ptr = 0;
}
