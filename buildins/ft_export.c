/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:32:36 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:32:36 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_export(t_cmd *cmd, char ***env)
{
	int	i;

	g_exit_status = 0;
	if (cmd->args[1] == NULL)
	{
		return ;
	}
	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (is_valid_env_name(cmd->args[i]) == 0)
		{
			export_error("export", cmd->args[i]);
		}
		else
		{
			*env = export_add(cmd->args[i], *env);
		}
		i++;
	}
}
