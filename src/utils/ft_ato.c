#include "ft_utils.h"

int	ft_atoi(const char *nptr)
{
	int	signe;
	int	resulte;

	signe = 1;
	resulte = 0;
	while (*nptr == ' ' || *nptr == '\n' || *nptr == '\t'
		|| *nptr == '\v' || *nptr == '\f' || *nptr == '\r')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			signe = -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		resulte = resulte * 10 + (*nptr - '0');
		nptr++;
	}
	return (resulte * signe);
}

long	ft_atol(const char *nptr)
{
	int		signe;
	long	resulte;

	signe = 1;
	resulte = 0;
	while (*nptr == ' ' || *nptr == '\n' || *nptr == '\t'
		|| *nptr == '\v' || *nptr == '\f' || *nptr == '\r')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			signe = -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		resulte = resulte * 10 + (*nptr - '0');
		nptr++;
	}
	return (resulte * signe);
}

unsigned long	ft_atoul(const char *nptr)
{
	unsigned long	resulte;

	resulte = 0;
	while (*nptr == ' ' || *nptr == '\n' || *nptr == '\t'
		|| *nptr == '\v' || *nptr == '\f' || *nptr == '\r')
		nptr++;
	if (*nptr == '+' || *nptr == '-')
		nptr++;
	while (ft_isdigit(*nptr))
	{
		resulte = resulte * 10 + (*nptr - '0');
		nptr++;
	}
	return (resulte);
}
