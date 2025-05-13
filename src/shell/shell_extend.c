#include "shell.h"

static void	_micro_putnbr(char *dest, unsigned char nb)
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

// ! 1st chr of the identifier is not 0-9
static bool	_is_envname_chr(char c)
{
	return ((c == '_')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'));
}

static char	*_extend_non_id(t_xstring *dest_string, t_shell *shell,
	char *in_str)
{
	char	*str;
	char	rc_buffer[4];

	if (*in_str == '?')
	{
		_micro_putnbr(rc_buffer, (unsigned char)shell->global_exec.return_code);
		if (!xstring_add_string(dest_string, rc_buffer))
			return (NULL);
		return (++in_str);
	}
	if (*in_str)
		str = xstring_add_string(dest_string, "@@");
	else
		str = xstring_add_string(dest_string, "@");
	if (!str)
		return (NULL);
	str[0] = '$';
	str[1] = *in_str;
	if (*in_str)
		in_str++;
	return (in_str);
}

static char	*_extend_id(t_xstring *dest_string, t_shell *shell, char *in_str)
{
	char	*name_ptr;
	char	*env_value;
	char	save_chr;

	name_ptr = in_str;
	while (_is_envname_chr(*in_str))
		in_str++;
	save_chr = *in_str;
	*in_str = 0;
	env_value = env_handler_get_env_value(shell->env_handler, name_ptr);
	*in_str = save_chr;
	if (env_value)
		if (!xstring_add_string(dest_string, env_value))
			return (NULL);
	return (in_str);
}

bool	shell_extend_string(t_shell *shell, t_xstring *dest_string,
	char *in_str)
{
	char	*unextended_str;

	unextended_str = in_str;
	while (*in_str)
	{
		while (*in_str && *in_str != '$')
			in_str++;
		if (!xstring_add_substr(dest_string, unextended_str, in_str))
			return (false);
		if (*in_str == '$')
		{
			in_str++;
			if ((*in_str >= '0' && *in_str <= '9'))
				in_str = _extend_non_id(dest_string, shell, in_str);
			else if (_is_envname_chr(*in_str))
				in_str = _extend_id(dest_string, shell, in_str);
			else
				in_str = _extend_non_id(dest_string, shell, in_str);
			if (!in_str)
				return (false);
		}
		unextended_str = in_str;
	}
	return (xstring_add_string(dest_string, ""));
}
