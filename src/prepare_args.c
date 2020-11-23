#include "execution.h"
#include <unistd.h>
#include <fcntl.h>

char	**extract_vars(char *str, t_list *var_positions, char **env)
{
	t_var_pos	*cur_var_pos;
	char		**ret;
	char		*var_name;
	size_t		i;

	ret = malloc(sizeof(char *) * (ft_lstsize(var_positions) + 1));
	if (ret == NULL)
		return (NULL);
	ft_bzero(ret, sizeof(char *) * (ft_lstsize(var_positions) + 1));
	i = 0;
	while (var_positions != NULL)
	{
		cur_var_pos = var_positions->content;
		var_name = ft_substr(str, cur_var_pos->start, cur_var_pos->len);
		if (var_name == NULL)
		{
			free_str_array(ret);
			return (NULL);
		}
		if (*var_name == '~')
			ret[i] = get_var_value(env, "HOME");
		if (*var_name == '?')
			ret[i] = ft_itoa(stat_loc);
		else
			ret[i] = get_var_value(env, var_name);
		free(var_name);
		if (ret[i] == NULL)
		{
			free_str_array(ret);
			return (NULL);
		}
		i++;
		var_positions = var_positions->next;
	}
	return (ret);
}

size_t	resize_token(char *str, t_list *var_positions, char **var_values)
{
	size_t		ret;
	t_var_pos	*cur_var_pos;

	ret = ft_strlen(str);
	while (var_positions != NULL)
	{
		cur_var_pos = var_positions->content;
		ret -= cur_var_pos->len;
		var_positions = var_positions->next;
	}
	while (*var_values != NULL)
	{
		ret += ft_strlen(*var_values);
		var_values++;
	}
	return (ret);
}

char	*expand_token_vars(t_token *token, char **env)
{
	char		***ptr;
	char		**var_values;
	char		*ret;
	size_t		token_len;
	t_list		*var_positions;
	t_var_pos	*var;
	size_t		i_ret;
	size_t		j_input;
	size_t		k_value;
	size_t		l_values;

	// if (token->var_positions == NULL)
	// 	return (ft_strdup(token->str));
	var_values = extract_vars(token->str, token->var_positions, env);
	if (var_values == NULL)
		return (NULL);
	token_len = resize_token(token->str, token->var_positions, var_values);
	ret = (char *)malloc(sizeof(char) * (token_len + 2)); // + 2?
	if (ret == NULL)
		return (NULL);
	i_ret = 0;
	j_input = 0;
	l_values = 0;
	var_positions = token->var_positions;
	while (i_ret < token_len)
	{
		if (var_positions != NULL) 
			var = var_positions->content;
		if (var != NULL && j_input == var->start)
		{
			if (var_values[l_values] != NULL)
			{
				j_input = var->start + var->len;
				if (ft_strlen(var_values[l_values]) != 0)
				{
					k_value = 0;
					while (var_values[l_values][k_value])
					{
						ret[i_ret] = var_values[l_values][k_value];
						k_value++;
						i_ret++;
					}
					j_input = var->start + var->len;
				}
			}
			var_positions = var_positions->next;
			l_values++;
		}
		else
		{
			ret[i_ret] = token->str[j_input];
			i_ret++;
			j_input++;
		}
	}
	ret[i_ret] = '\0';
	ptr = &var_values; // ??
	free_str_array(var_values);
//	i = 0;
//	while (var_values[i] != NULL)
//		free(var_values[i++]);
//	free(var_values);
	return (ret);
}

void	apply_redir(char *cur_arg, enum e_redir_op redir)
{
	int			redir_fd;
	mode_t		mode;
	int			flags;

	mode = 0;
	fd_backup = dup(0); // eh bien il sort d'ou ce fd_backup ; err a gerer
	if (redir == FILEIN)
	{
		fd_old = STDIN_FILENO; // et fd_old
		flags = O_RDONLY;
	}
	else
	{
		fd_old = STDOUT_FILENO;
		flags = O_WRONLY | O_CREAT;
		mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
		if (redir == APPEND)
			flags |= O_APPEND;
	}
	redir_fd = open(cur_arg, flags, mode);
	if (redir_fd >= 0) // else error?
	{
		dup2(fd_old, fd_backup); // error?
		dup2(redir_fd, fd_old); // error
		close(redir_fd); // error
	}
}

char	**prepare_args(t_command *command, char **env)
{
	t_list		*tokens;
	t_token		*cur_token;
	char		**args;
	char		*cur_arg;
	size_t		i;

	tokens = command->tokens;
	args = (char **)malloc(sizeof(char *) * (ft_lstsize(tokens) + 1));
	if (args == NULL)
		return (NULL); // MALLOC_ERR
	ft_bzero(args, sizeof(char *) * (ft_lstsize(tokens) + 1));
	i = 0;
	while (tokens != NULL)
	{
		cur_token = ft_lstpop(&tokens);
  		cur_arg = expand_token_vars(cur_token, env);
		if (cur_arg == NULL)
		{
			free_str_array(args);
			return (NULL); // MALLOC_ERR
		}
		if (cur_token->redir != NO_REDIR)
			apply_redir(cur_arg, cur_token->redir); // error?
		else
		{
			args[i] = cur_arg;
			i++;
		}
		free_token(cur_token);
	}
	free(command);
//	ft_printarray_fd(args, 1);
	return (args);
}
