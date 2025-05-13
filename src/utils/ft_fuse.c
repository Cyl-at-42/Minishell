#include "ft_utils.h"

static size_t	_get_fuse_size(size_t nb_of_element, va_list arg_ptr)
{
	size_t	fuse_size;
	size_t	index;

	fuse_size = 0;
	index = 0;
	while (index < nb_of_element)
	{
		fuse_size += ft_strlen(va_arg(arg_ptr, char *));
		index++;
	}
	return (fuse_size);
}

size_t	_fuse_add_back(char *fuse_str, char *str, size_t	len)
{
	return (ft_strlcpy(&fuse_str[len], str, ft_strlen(str) + 1));
}

char	*ft_fuse(size_t nb_of_element, ...)
{
	va_list	arg_ptr;
	char	*fuse_str;
	size_t	index;

	va_start(arg_ptr, nb_of_element);
	fuse_str = malloc(_get_fuse_size(nb_of_element, arg_ptr) + 1);
	if (!fuse_str)
		return (NULL);
	fuse_str[0] = '\0';
	index = 0;
	va_end(arg_ptr);
	va_start(arg_ptr, nb_of_element);
	while (index < nb_of_element)
	{
		_fuse_add_back(fuse_str, va_arg(arg_ptr, char *), ft_strlen(fuse_str));
		index++;
	}
	return (fuse_str);
}
