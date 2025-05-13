#ifndef SHELL_H
# define SHELL_H

# include "ast.h"
# include "exec.h"
# include "xarray.h"
# include "xbuffer.h"
# include "xstring.h"
# include "env_handler.h"
# include "wildcard.h"
# include "parser.h"

# define TFNCS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

typedef struct s_parser	t_parser;

typedef struct s_shell
{
	t_ast			*ast;
	t_parser		*parser;
	t_xbuffer		*word_buffer;
	t_xbuffer		*generated_args_buffer;
	t_env_handler	*env_handler;
	t_xarray		argv_array;
	t_xarray		redir_args_array;
	t_exec			exec;
	t_exec			global_exec;
	t_exec			*current_exec;
	t_exec			*pipeline_execs;
	unsigned int	pipeline_size;
	unsigned int	pipeline_exec_index;
	unsigned int	exit;
	int				echo_fd;
	int				save_stdin_fd;
	int				save_stdout_fd;
	bool			word_hasquote;
	char			**pwd;
}	t_shell;

typedef struct s_shell_heredoc
{
	t_shell		*shell;
	t_xstring	*exp_string;
	int			pipe_fds[2];
	int			error;
	int			status;
	pid_t		new_pid;
	int			waitpidret;
}	t_shell_heredoc;

int				destroy_shell(t_shell *shell);
bool			shell_save_stdio(t_shell *shell);
bool			shell_restore_stdio(t_shell *shell);
void			shell_close_saved_stdio(t_shell *shell);

// Groups
bool			shell_exec_conditionnal_or(t_shell *shell, t_ast_node_i node);
bool			shell_exec_conditionnal_and(t_shell *shell, t_ast_node_i node);
bool			shell_exec_subshell(t_shell *shell, t_ast_node_i node);

// Pipeline
void			shell_close_pipes_and_files(t_shell *shell);
void			shell_wait_pipeline(t_shell *shell);
bool			shell_exec_pipeline(t_shell *shell, t_ast_node_i node);
bool			shell_exec_pipe(t_shell *shell, t_ast_node_i node);

// Tree
bool			shell_exec_subtree(t_shell *shell, t_ast_node_i node);
bool			shell_exec_children(t_shell *shell, t_ast_node_i node);
void			shell_clean_tree_files(t_shell *shell, t_ast_node_i node);

// Word
bool			shell_exec_word(t_shell *shell, t_ast_node_i node);
bool			shell_exec_dquoted(t_shell *shell, t_ast_node_i node);
bool			shell_exec_squoted(t_shell *shell, t_ast_node_i node);
bool			shell_process_word_buffer(t_shell *shell);
bool			shell_exec_word_elements(t_shell *shell, t_ast_node_i node);

// Args array
bool			shell_word_array_to_args(t_shell *shell, char **word_array_ptr,
					unsigned int *args_added);
bool			shell_string_to_args(t_shell *shell, char *str);
unsigned int	shell_get_args_count(t_shell *shell);
void			shell_arg_cancel_empty_string(t_shell *shell);
bool			shell_wildcard_populate_args(t_shell *shell,
					t_xarray *target_array, char *pattern_escstr);

// Command
bool			shell_exec_command(t_shell *shell, t_ast_node_i node);
bool			shell_extend_string(t_shell *shell, t_xstring *dest_string,
					char *in_str);

bool			shell_get_hd_process(t_shell_heredoc *hd, char *limiter,
					int echo_fd);

char			*find_path_env(char **env_table);
char			*edit_paths_str(char *paths_str);
char			*find_cmd_in_path_env(char *cmd, char *paths_str);
bool			shell_exec_redirs(t_shell *shell, t_ast_node_i node);
bool			shell_redirect_pipes(t_shell *shell, t_exec *exec);
char			*shell_find_ext_command(t_shell *shell, char *cmd_name);
bool			shell_execute_ext_command(t_shell *shell, char **argv);
bool			shell_execute_builtin_command(t_shell *shell,
					int (*builtin_ftn)(t_shell *));

#endif
