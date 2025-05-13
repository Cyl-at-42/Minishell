#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

# include "xarray.h"
# include "xbuffer.h"
# include "xstring.h"
# include "env_handler.h"
# include "ast.h"
# include "parser.h"
# include "estr.h"
# include "shell.h"
# include "builtin.h"
# include "wildcard.h"
# include "ft_utils.h"
# include "errors.h"

# define APP_NAME "minishell"

typedef struct s_minishell
{
	t_env_handler	env_handler;
	int				prev_retcode;
	int				shell_level;
	int				echo_fd;
	char			*pwd;
}	t_minishell;

int				shell_get_heredoc(t_shell *shell, char *dest, char *limiter,
					t_xstring *exp_string);
void			init_sig_handler(bool is_parent);
void			init_shell(t_shell *shell, t_env_handler *env_handler,
					char **pwd, int prev_retcode);
t_shell_heredoc	*heredoc_sig(t_shell_heredoc *heredoc);
bool			get_prompt(char **input, t_minishell *minishell);
bool			readline_ext(char **out_input, char *prompt, int echo_fd);
char			*minishell_get_line(t_minishell *minishell);
bool			minishell_exec_line(t_minishell *minishell, char *input);
void			minishell_write_parser_errors(t_minishell *minishell,
					t_parser *parser);
#endif
