#include "minishell.h"

char	*find_path_env(char **env_table)
{
	while (*env_table)
	{
		if (!ft_strncmp("PATH=", *env_table, 5))
			return (*env_table + 5);
		env_table++;
	}
	return (NULL);
}

static void	path_add(char *dest, char *src)
{
	while (*dest++)
		;
	*(dest - 1) = '/';
	while (*src)
		*dest++ = *src++;
	*dest = 0;
}

char	*edit_paths_str(char *paths_str)
{
	t_xstring	edited_paths_string;
	char		*substr_start;

	substr_start = paths_str;
	if (!init_xstring(&edited_paths_string, ft_strlen(paths_str) + 8))
		return (NULL);
	if (paths_str[0] == ':')
		xstring_add_string(&edited_paths_string, ".");
	while (*paths_str)
	{
		if (paths_str[0] == ':' && (paths_str[1] == ':' || !paths_str[1]))
		{
			if (!xstring_add_substr(&edited_paths_string, substr_start,
					++paths_str))
				return (destroy_xstring(&edited_paths_string), NULL);
			if (!xstring_add_string(&edited_paths_string, "."))
				return (destroy_xstring(&edited_paths_string), NULL);
			substr_start = paths_str;
		}
		paths_str++;
	}
	if (!xstring_add_substr(&edited_paths_string, substr_start, paths_str))
		return (destroy_xstring(&edited_paths_string), NULL);
	return (edited_paths_string.data);
}

char	*find_cmd_in_path_env(char *cmd, char *paths_str)
{
	char	*res;
	char	**paths_table;
	char	*edited_paths_str;
	int		i;

	edited_paths_str = edit_paths_str(paths_str);
	if (!edited_paths_str)
		return (NULL);
	paths_table = ft_split_ex(edited_paths_str, ':', ft_strlen(cmd) + 1);
	if (!paths_table)
		return (free(edited_paths_str), NULL);
	res = NULL;
	i = 0;
	while (paths_table[i])
	{
		path_add(paths_table[i], cmd);
		if (!res && (access(paths_table[i], F_OK) == 0))
			res = paths_table[i];
		if (res != paths_table[i])
			free(paths_table[i]);
		i++;
	}
	free(edited_paths_str);
	free(paths_table);
	return (res);
}
