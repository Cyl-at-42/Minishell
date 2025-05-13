#ifndef BUILTIN_H
# define BUILTIN_H

# define CD_CWD_BUFFERSIZE 1024

# include "minishell.h"

int		builtin_cd(t_shell *shell);
char	*cd_get_current_wd(void);

int		builtin_env(t_shell *shell);
int		builtin_pwd(t_shell *shell);
int		builtin_echo(t_shell *shell);
int		builtin_exit(t_shell *shell);
int		builtin_unset(t_shell *shell);
char	*cd_get_current_wd(void);
void	cd_update_wds(t_env_handler *env_handler, char *old_wd);
int		builtin_export(t_shell *shell);
bool	export_show_env(t_env_handler *env_handler);
bool	export_assign_variable(t_env_handler *env_handler, char *arg);
void	builtins_tinystr_copy(unsigned long *dest, char *source);
void	builtins_init_tinystrs(unsigned long *builtins_tinystrs);
void	*str_is_builtin(char *str);

#endif
