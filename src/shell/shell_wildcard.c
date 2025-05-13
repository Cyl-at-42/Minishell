#include <dirent.h>
#include "shell.h"

static void	_sort_table(char **env_table, size_t env_size)
{
	size_t	i;
	char	*temp;

	if (env_size <= 1)
		return ;
	while (env_size-- != 0)
	{
		i = 0;
		while (i < env_size)
		{
			if (ft_strcmp_cu(env_table[i], env_table[i + 1]) > 0)
			{
				temp = env_table[i];
				env_table[i] = env_table[i + 1];
				env_table[i + 1] = temp;
			}
			i++;
		}
	}
}

bool	_scan_dirent(struct dirent *dirent, short *pattern_wstr,
	t_shell *shell, t_xarray *target_array)
{
	char			*filename_arg_ptr;
	unsigned int	new_element;
	size_t			len;
	char			result;

	if (*dirent->d_name != '.' || *pattern_wstr == '.')
	{
		if (!wpm_wildcard_pattern_match(&result, pattern_wstr, dirent->d_name))
			return (false);
		if (result)
		{
			len = ft_strlen(dirent->d_name);
			filename_arg_ptr = xbuffer_add_data(shell->generated_args_buffer,
					dirent->d_name, len + 1);
			if (!filename_arg_ptr)
				return (false);
			filename_arg_ptr[len] = 0;
			if (!xarray_new_element(target_array, &new_element))
				return (false);
			((char **)target_array->data)[new_element] = filename_arg_ptr;
		}
	}
	return (true);
}

bool	shell_wildcard_populate_args(t_shell *shell, t_xarray *target_array,
	char *pattern_escstr)
{
	DIR				*dir_stream;
	struct dirent	*dirent;
	short			*pattern_wstr;
	unsigned int	entries_start_index;
	char			**entries_start;

	pattern_wstr = wpm_create_pattern_wstr(pattern_escstr);
	if (!pattern_wstr)
		return (NULL);
	dir_stream = opendir(".");
	entries_start_index = target_array->used;
	dirent = readdir(dir_stream);
	while (dirent)
	{
		if (!_scan_dirent(dirent, pattern_wstr, shell, target_array))
			return (closedir(dir_stream), free(pattern_wstr), false);
		dirent = readdir(dir_stream);
	}
	entries_start = ((char **) target_array->data) + entries_start_index ;
	if (target_array->used - entries_start_index)
		_sort_table(entries_start, target_array->used - entries_start_index);
	closedir(dir_stream);
	free(pattern_wstr);
	return (true);
}
