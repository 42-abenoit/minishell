#include "parsing.h"

char	*error(char *line, t_list **commands, t_state_machine *machine)
{
	static char		*err_msg[NB_PARSING_ERRORS] = {
		"No matching single quote",
		"No matching double quote",
		"Multiline inputs are currently not supported",
		"Redirection path invalid",
		"No redirection path specified"};

	ft_putstr_fd("minishell: syntax error: ", STDERR_FILENO);
	ft_putstr_fd(err_msg[machine->error], STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	if (machine->cur_token != NULL)
	{
		free_token(machine->cur_token);
		free_commands(commands);
	}
	machine->state = END;
	return (line);
}

char	*space(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	if (*line == '\0')
		machine->state = END;
	else if (ft_isspace(*line))
		line++;
	else if (*line == '>' || *line == '<')
		machine->state = ANGLE_BRACKET;
	else if (*line == ';')
		machine->state = SEMICOLON;
	else if (*line == '|')
		machine->state = PIPE;
	else
		machine->state = LETTER;
	return (line);
}

char	*backslash(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	if (*line != '\0')
	{
		add_to_buf(*line, machine);
		line++;
		machine->state = LETTER;
	}
	else
	{
		machine->state = ERR;
		machine->error = ESCAPE_NL;
	}
	return (line);
}

char	*dollar(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '?' || ft_isalnum(*line) || *line == '_')
	{
		line = parse_variable(line, machine);
		if (line == NULL)
			return (NULL);
		machine->state = LETTER;
	}
	else
	{
		add_to_buf('$', machine);
		machine->state = LETTER;
	}
	return (line);
}

char	*quote(char *line, t_list **commands, t_state_machine *machine)
{
	char	quote_style;

	(void)commands;
	quote_style = *line;
	line++;
	while (*line != quote_style && *line != '\0')
	{
		if (quote_style == '"' && *line == '$')
		{
			line++;
			if (*line == '?' || ft_isalnum(*line) || *line == '_')
			{
				line = parse_variable(line, machine);
				if (line == NULL)
					return (NULL);
			}
			else
				add_to_buf('$', machine);
		}
		else
		{
			if (quote_style == '"' && *line == '\\'
					&& ft_isset(line[1], "\\\"$"))
				line++;
			add_to_buf(*line, machine);
			line++;
		}
	}
	if (*line == '\0')
	{
		machine->state = ERR;
		machine->error = ft_index("'\"", quote_style);
	}
	else
	{
		line++;
		machine->state = LETTER;
	}
	return (line);
}

char	*angle_bracket(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	if (reset_buf(machine) == FAILURE)
		return(NULL);
	if (machine->cur_token->redir != NO_REDIR)
	{
		machine->state = ERR;
		machine->error = REDIR_PATH_INVALID;
	}
	else
	{
		if (*line == '<')
			machine->cur_token->redir = FILEIN;
		else if (line[1] == '>')
		{
			machine->cur_token->redir = APPEND;
			line++;
		}
		else
			machine->cur_token->redir = FILEOUT;
		line++;
		machine->state = SPACE;
	}
	return (line);
}

char	*semicolon(char *line, t_list **commands, t_state_machine *machine)
{
	if (new_command(commands) == FAILURE)
		return(NULL);
	line++;
	machine->state = SPACE;
	return (line);
}

char	*pipe_(char *line, t_list **commands, t_state_machine *machine)
{
	((t_command *)ft_lstlast(*commands)->content)->pipe_flag = TRUE;
	if (new_command(commands) == FAILURE)
		return (NULL);
	line++;
	machine->state = SPACE;
	return (line);
}

char	*tilde(char *line, t_list **commands, t_state_machine *machine)
{
	(void)commands;
	add_to_buf('~', machine);
	if (reset_buf(machine) == FAILURE)
		return (NULL);
	if (machine->cur_token->str[1] == '\0' && (ft_isset(*line, "/><;|")
				|| ft_isspace(*line) || *line == '\0'))
	{
		if (add_variable(&machine->cur_token->var_positions, 0, 1) == FAILURE)
		{
			free_token(machine->cur_token);
			return (NULL);
		}
	}
	machine->state = LETTER;
	return (line);
}

char	*letter(char *line, t_list **commands, t_state_machine *machine)
{
	t_list		**tokens;

	tokens = (t_list **)&((t_command *)(ft_lstlast(*commands))->content)->tokens;
	if (*line == '"' || *line == '\'')
		machine->state = QUOTE;
	else if (*line == '>' || *line == '<')
	{
		if (link_token(tokens, machine) == FAILURE)
			return (NULL);
		machine->state = ANGLE_BRACKET;
	}
	else if (*line == '$')
	{
		machine->state = DOLLAR;
		line++;
	}
	else if (*line == '~')
	{
		machine->state = TILDE;
		line++;
	}
	else if (*line == '\\')
	{
		machine->state = BACKSLASH;
		line++;
	}
	else if (*line == ';')
	{
		if (link_token(tokens, machine) == FAILURE)
			return (NULL);
		machine->state = SEMICOLON;
	}
	else if (*line == '|')
	{
		if (link_token(tokens, machine) == FAILURE)
			return (NULL);
		machine->state = PIPE;
	}
	else if (*line == '\0' || ft_isspace(*line))
	{
		if (link_token(tokens, machine) == FAILURE)
			return (NULL);
		if (*line == '\0')
			machine->state = END;
		else
		{
			machine->state = SPACE;
			line++;
		}
	}
	else
	{
		add_to_buf(*line, machine);
		line++;
	}
	return (line);
}

t_list	*parse_input(char *line)
{
	static t_parse	process[NB_STATES - 1] = {letter, quote, backslash,
		dollar, tilde, space, angle_bracket, semicolon, pipe_, error};
	t_state_machine		machine;
	t_list				*commands; // a mettre dans la machine pour retirer les (void)commands; de toutes les fonctions de process? et retirer cur_token?

	errno = 0;
	machine.state = SPACE;
	machine.len = 0;
	machine.cur_token = NULL;
	ft_bzero(&machine.buf, BUF_SIZE);
	commands = NULL;
	if (new_command(&commands) == FAILURE)
		return (NULL);
	while (machine.state != END)
	{
		line = process[machine.state](line, &commands, &machine);
		if (line == NULL)
		{
			free_commands(&commands);
			return (NULL);
		}
	}
	if (machine.cur_token != NULL && machine.cur_token->redir != NO_REDIR)
	{
		machine.error = REDIR_PATH_MISSING;
		error(line, &commands, &machine);
	}
//	print_tokens(commands);
	return (commands);
}