#include "minishell.h"
#include "builtin.h"
#include "ft_utils.h"

static bool	final_prompt(char **input, char *start, char *end, size_t len)
{
	*input = ft_fuse(3, start, &end[len], "$ ");
	if (!*input)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

bool	get_prompt(char **input, t_minishell *minishell)
{
	char	*home;
	char	*wd_str;

	home = env_handler_get_env_value(&minishell->env_handler, "HOME");
	wd_str = minishell->pwd;
	if (home && *home && strncmp(wd_str, home, ft_strlen(home)) == 0
		&& ft_strlen(wd_str) != 1)
		return (
			final_prompt(input, "~", wd_str, ft_strlen(home)));
	else
		return (final_prompt(input, "", wd_str, 0));
}
