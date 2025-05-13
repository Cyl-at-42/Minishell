#include "ft_utils.h"

bool	ft_isalpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return (true);
	}
	return (false);
}

bool	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
	{
		return (true);
	}
	return (false);
}
