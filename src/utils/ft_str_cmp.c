#include "ft_utils.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	index;

	index = 0;
	while ((s1[index] != '\0' || s2[index] != '\0') && index < n)
	{
		if (s1[index] != s2[index])
		{
			return (((unsigned char)s1[index] - (unsigned char)s2[index]));
		}
		index++;
	}
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	index;

	index = 0;
	while ((s1[index] != '\0' || s2[index] != '\0'))
	{
		if (s1[index] != s2[index])
		{
			return (((unsigned char)s1[index] - (unsigned char)s2[index]));
		}
		index++;
	}
	return (0);
}

char	*ft_find_last_of(char *str, char find)
{
	long	index;

	index = ft_strlen(str);
	if (index == 0)
		return (NULL);
	while (--index >= 0)
	{
		if (str[index] == find)
			return (&str[index]);
	}
	return (NULL);
}
