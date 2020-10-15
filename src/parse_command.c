#include "minishell.h"

#include <stdlib.h>
#include <unistd.h>

ssize_t		error(char *line, t_list **tokens, t_state_machine *machine)
{
	(void)line;
	(void)tokens;
	(void)machine;
	return (0);
}

ssize_t		end(char *line, t_list **tokens, t_state_machine *machine)
{
	(void)line;
	(void)tokens;
	machine->state = END;
	return (0);
}

ssize_t		space(char *line, t_list **tokens, t_state_machine *machine)
{
	ssize_t		elem_len;

	(void)tokens;
	elem_len = 0;
	if (*line == '"' || *line == '\'') // SQ
		machine->state = QUOTE;
	else if (*line == '\0') // SE
		machine->state = END;
	else if (ft_isset(*line, ISSPACE_3) == -1) // SL
		machine->state = LETTER;
	else // SS
		elem_len = 1;
	return (elem_len);
}

ssize_t		quote(char *line, t_list **tokens, t_state_machine *machine)
{
	char	quote_style;
	size_t	inside_quotes_len;

	quote_style = *line;
	line++;
	inside_quotes_len = 0;
	while (*line != quote_style && *line != '\0') // on va jusqu'au ending quote
	{
		add_to_buf(*line, machine);
		line++;
		inside_quotes_len++;
	}
	if (*line == '\0')
		return (ERROR); // pas d'ending quote
	else
		line++;
	if (*line == '\0' || ft_isset(*line, ISSPACE_3) != -1) // QE QS
	{
		if (link_token(tokens, machine) == ERROR)
			return (ERROR); // malloc error
		if (*line == '\0')
			machine->state = END;
		else
			machine->state = SPACE;
	}
	else if (*line != '"' && *line != '\'') // QL
		machine->state = LETTER;
	return (inside_quotes_len + 2);
}

ssize_t		letter(char *line, t_list **tokens, t_state_machine *machine)
{
	ssize_t		elem_len;

	elem_len = 0;
	if (*line == '"' || *line == '\'') // LQ
		machine->state = QUOTE;
	else if (*line == '\0' || ft_isset(*line, ISSPACE_3) != -1) // LE LS
	{
		if (link_token(tokens, machine) == ERROR) //si la ligne est vide ou commence par un space on devra retirer le token
			return (ERROR); // malloc error
		if (*line == '\0')
			machine->state = END;
		else
		{
			machine->state = SPACE;
			elem_len = 1;
		}
	}
	else // LL
	{
		add_to_buf(*line, machine);
		elem_len = 1;
	}
	return (elem_len);
}

char			**split_command(char *line)
{
	static t_function	process[NB_STATES] = {letter, quote, space, error, end};
	t_state_machine		machine;
	ssize_t				elem_len;
	t_list				*tokens;

	machine.state = LETTER;
	machine.len = 0;
	machine.cur_token = NULL;
	ft_bzero(&machine.buf, BUF_SIZE);
	tokens = NULL;
	while (machine.state != END)
	{
		elem_len = process[machine.state](line, &tokens, &machine);
		if (elem_len == ERROR)
			return (NULL); // err
		line += elem_len;
		//printf("current = %c ; state = %d\n", *line, machine.state);
	}
	size_t i = 0;
	while (tokens != NULL)
	{
		printf("%zu = %s\n", i, (char *)(tokens->content));
		tokens = tokens->next;
		i++;
	}
	return (NULL);
}
