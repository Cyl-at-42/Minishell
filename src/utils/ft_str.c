#include "ft_utils.h"

size_t	ft_strlen(const char *s)
{
	size_t	counter;

	counter = 0;
	while (*s++)
		counter++;
	return (counter);
}

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i--;
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	int		index;

	new_str = malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(char) + 1);
	if (!new_str)
		return (NULL);
	index = 0;
	while (*s1)
		new_str[index++] = *s1++;
	while (*s2)
		new_str[index++] = *s2++;
	new_str[index] = '\0';
	return (new_str);
}

char	*ft_strdup(const char *s)
{
	int		str_len;
	char	*src_copy;
	int		index;

	str_len = ft_strlen(s) + 1;
	src_copy = malloc(str_len);
	if (src_copy == NULL)
		return (NULL);
	index = -1;
	while (++index < str_len)
		src_copy[index] = s[index];
	return (src_copy);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_size;
	size_t	index;

	index = 0;
	src_size = ft_strlen(src);
	if (size != 0)
	{
		while (src[index] != '\0' && index < (size - 1))
		{
			dst[index] = src[index];
			index++;
		}
		dst[index] = '\0';
	}
	return (src_size);
}
