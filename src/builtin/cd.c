#include "builtin.h"

char	*cd_get_current_wd(void)
{
	char	*buffer;

	buffer = malloc(CD_CWD_BUFFERSIZE);
	if (!buffer)
		return (NULL);
	if (getcwd(buffer, CD_CWD_BUFFERSIZE) == NULL)
		return (perror("getcwd"), NULL);
	return (buffer);
}

static bool	_cd_go_back(char *pwd)
{
	char	*target;

	target = ft_strdup(pwd);
	if (!target)
		return (EXIT_FAILURE);
	if (ft_strcmp("/", target) == 0)
	{
		if (chdir("/") == -1)
			return (free(target),
				write_error_strs(3, APP_NAME, ": cd: /:", ""), EXIT_FAILURE);
	}
	*ft_find_last_of(target, '/') = '\0';
	if (target[0] == '\0')
	{
		target[0] = '/';
		target[1] = '\0';
	}
	if (chdir(target) == -1)
		return (write_error_strs(5, APP_NAME, ": cd: ", target, ":", ""),
			free(target), EXIT_FAILURE);
	return (free(target), EXIT_SUCCESS);
}

static bool	_cd_go_home(t_env_handler *env_handler)
{
	char	*var_home;

	var_home = env_handler_get_env_value(env_handler, "HOME");
	if (var_home && !(*var_home)
		&& !env_handler_get_env_is_null(env_handler, "HOME"))
		return (EXIT_SUCCESS);
	else if (var_home && !env_handler_get_env_is_null(env_handler, "HOME"))
	{
		if (chdir(var_home) == -1)
			return (write_error_strs(5, APP_NAME, ": cd: ", var_home, ":", ""),
				EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	else
		return (write_error_strs(2, APP_NAME, ": cd: HOME not set\n"),
			EXIT_FAILURE);
}

static int	_builtin_cd(t_shell *shell)
{
	int		argc;

	argc = shell->argv_array.used - 1;
	if (argc > 2)
		return (write_error_strs(2, APP_NAME,
				": cd: too many arguments\n"), EXIT_FAILURE);
	if (argc == 2)
	{
		if (ft_strcmp("..", ((char **)shell->argv_array.data)[1]) == 0)
			return (_cd_go_back(*shell->pwd));
		if (chdir(((char **)shell->argv_array.data)[1]) == -1)
		{
			write_error_strs(5, APP_NAME, ": cd: ",
				((char **)shell->argv_array.data)[1], ":", "");
			return (EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
	}
	else
		return (_cd_go_home(shell->env_handler));
}

int	builtin_cd(t_shell *shell)
{
	char	*wd_str;
	int		rc;

	rc = _builtin_cd(shell);
	if (rc)
		return (rc);
	if (env_handler_get_env_value(shell->env_handler, "OLDPWD"))
		env_handler_setval(shell->env_handler, "OLDPWD", *shell->pwd);
	wd_str = cd_get_current_wd();
	if (!wd_str)
		return (EXIT_FAILURE);
	free(*shell->pwd);
	*shell->pwd = wd_str;
	if (env_handler_get_env_value(shell->env_handler, "PWD"))
		env_handler_setval(shell->env_handler, "PWD", wd_str);
	return (rc);
}
