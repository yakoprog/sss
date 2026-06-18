/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:33:36 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:33:36 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_n_flag(char *str)
{
	int	i;

	if (str[0] != '-' || str[1] != 'n')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	n_flag;

	n_flag = 0;
	i = 1;
	while (cmd->args[i] != NULL && is_n_flag(cmd->args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i] != NULL)
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (n_flag == 0)
		printf("\n");
	g_exit_status = 0;
}
