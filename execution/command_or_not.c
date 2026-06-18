/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_or_not.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:23:18 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:23:22 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_free_split(char **arr, size_t k)
{
	while (arr[k] != NULL)
	{
		free(arr[k]);
		k++;
	}
	free(arr);
}

static char	*ft_find_command(char *input)
{
	char	*command;
	char	*command_parsed;
	int		i;

	i = 0;
	if (access(input, F_OK | X_OK) == 0)
		return (ft_strdup(input));
	while (input[i] != '\0' && input[i] != ' ')
		i++;
	command = ft_substr(input, 0, i);
	if (!command)
		return (NULL);
	command_parsed = ft_strjoin("/", command);
	if (!command_parsed)
		return (NULL);
	free(command);
	return (command_parsed);
}

static char	*ft_find_path_location(char **path, char *command_parsed)
{
	char	*path_location;
	int		i;
	size_t	k;

	i = 0;
	k = 0;
	while (path[k] != NULL)
	{
		path_location = ft_strjoin(path[k], command_parsed);
		i = access(path_location, F_OK);
		if (i == 0)
		{
			free(command_parsed);
			ft_free_split(path, 0);
			return (path_location);
		}
		else if (i < 0)
		{
			free(path[k]);
			free(path_location);
		}
		k++;
	}
	free(path);
	return (NULL);
}

int	check_command(char *input, char **envp)
{
	char	**path;
	char	*path_location;
	char	*command_parsed;
	int		i;

	i = 0;
	if (access(input, X_OK | F_OK) == 0)
		return (0);
	command_parsed = ft_find_command(input);
	if (!command_parsed)
		return (1);
	while (envp[i] != NULL && ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	path = ft_split(envp[i] + 5, ':');
	if (!path)
		return (1);
	path_location = ft_find_path_location(path, command_parsed);
	if (!path_location)
		return (1);
	free(path_location);
	return (0);
}
