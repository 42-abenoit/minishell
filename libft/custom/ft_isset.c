/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenoit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 13:40:47 by abenoit           #+#    #+#             */
/*   Updated: 2020/12/26 00:25:58 by mvidal-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_isset(char c, char *set)
{
	while (*set != '\0')
	{
		if (c == *set)
			return (TRUE);
		set++;
	}
	return (FALSE);
}
