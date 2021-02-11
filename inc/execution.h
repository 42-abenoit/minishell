#ifndef EXECUTION_H
# define EXECUTION_H

#include <sys/types.h> // waitpid, opendir
#include <sys/wait.h> // waitpid
#include <fcntl.h> // open
#include <dirent.h> // readdir, opendir

# include "minishell.h"
# include "error.h"

# define NOT_FOUND	-1
# define KEEP_ON	303

enum		e_cmd_code
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	ELSE
};

typedef int			(*t_exec)(char **args, t_xe *xe);

/*
**	main function
*/

int			parent_pipe_end(t_command *cur_command, t_xe *xe,
								int fd_in, int proc);

int		execute_cmd(char **args, char **redir_paths, enum e_redir_op *redir_types, t_xe *xe);

int		apply_redirs(char **redir_paths, enum e_redir_op *redir_types, t_xe *xe);
int		find_ambig_redir(char **redir_paths, enum e_redir_op *redir_types);

/*
**	main function
*/

int		create_cmd(char **tmp, char **path, char **args);
char	**create_path_array(t_xe *xe);

#endif
