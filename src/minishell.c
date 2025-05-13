#include "minishell.h"
#include <signal.h>
#include <sys/types.h>

extern bool	g_sig;

static void	_init_minishell_level(t_minishell *minishell)
{
	char	*env;

	env = getenv("SHLVL");
	if (!env)
		return ;
	minishell->shell_level = ft_atoi(env);
	if (minishell->shell_level < 0)
		minishell->shell_level = -1;
	minishell->shell_level++;
	if (minishell->shell_level > 999)
	{
		minishell->shell_level = 1;
		write_error_strs(2, APP_NAME,
			": warning: shell level (1000) too high, resetting to 1\n");
	}
}

bool	init_minishell(t_minishell *minishell, char **envp)
{
	char		pwd_buffer[1024];

	minishell->prev_retcode = 0;
	minishell->shell_level = 1;
	minishell->echo_fd = 0;
	minishell->pwd = NULL;
	if (!isatty(0))
		minishell->echo_fd = open("/dev/null", O_WRONLY);
	if (minishell->echo_fd == -1)
		return (false);
	if (!getcwd(pwd_buffer + 4, 1024 - 4))
		return (perror("getcwd"), false);
	minishell->pwd = ft_strdup(pwd_buffer + 4);
	if (!*minishell->pwd)
		return (false);
	ft_memcpy(pwd_buffer, "PWD=", 4);
	_init_minishell_level(minishell);
	if (!init_env_handler(&minishell->env_handler, ENVHASHTABLE_SIZE, envp))
		return (false);
	if (!env_handler_set(&minishell->env_handler, "SHLVL=999")
		|| !env_handler_set(&minishell->env_handler, pwd_buffer))
		return (destroy_env_handler(&minishell->env_handler), false);
	ft_m_putnbr(env_handler_get_env_value(&minishell->env_handler, "SHLVL"),
		minishell->shell_level);
	return (true);
}

int	destroy_minishell(t_minishell *minishell)
{
	if (minishell->pwd)
		free(minishell->pwd);
	if (minishell->echo_fd)
		close (minishell->echo_fd);
	destroy_env_handler(&minishell->env_handler);
	return ((unsigned char)minishell->prev_retcode);
}

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_minishell		minishell;
	bool			r;

	(void) argv;
	if (argc != 1)
		return (write_error_strs(2, APP_NAME, ": too many arguments\n"), 1);
	if (!init_minishell(&minishell, envp))
		return (write_error_strs(2, APP_NAME, ": main init failure !\n"), 1);
	while (1)
	{
		init_sig_handler(true);
		input = minishell_get_line(&minishell);
		if (g_sig == true)
			minishell.prev_retcode = 130;
		g_sig = false;
		if (!input)
			break ;
		r = minishell_exec_line(&minishell, input);
		free(input);
		if (r)
			break ;
	}
	return (clear_history(), destroy_minishell(&minishell));
}
