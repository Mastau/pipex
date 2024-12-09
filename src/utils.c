/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomarna <thomarna@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 19:03:46 by thomarna          #+#    #+#             */
/*   Updated: 2024/12/09 19:05:43 by thomarna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	err0r(char *cmd)
{
	char	**str;

	str = ft_split(cmd, ' ');
	if (str[0])
	{
		if (errno == 14)
			ft_dprintf(2, "%s: command not found\n", str[0]);
		else
			ft_dprintf(2, "pipex: %s: %s\n", str[0], strerror(errno));
	}
	else
	{
		if (errno == 14)
			ft_dprintf(2, " : command not found\n");
		else
			ft_dprintf(2, "pipex: %s\n", strerror(errno));
	}
	free_split(str);
	exit(EXIT_FAILURE);
}

char	*get_path(char **ep, char *cmd)
{
	char	**paths;
	char	*tmp;
	char	*path;
	int		i;

	i = -1;
	while (*ep && ft_strnstr(*ep, "PATH", 4) == 0)
		ep++;
	if (!*ep)
		return (NULL);
	paths = ft_split(*ep + 5, ':');
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, F_OK | X_OK) == 0)
		{
			free_split(paths);
			return (path);
		}
		free(path);
	}
	free_split(paths);
	return (0);
}

int	execmd(char **ep, char *av)
{
	char	**cmds;
	char	*path;

	cmds = ft_split(av, ' ');
	if (cmds == NULL)
		err0r("");
	if (access(cmds[0], F_OK | X_OK) == 0)
		path = cmds[0];
	else
		path = get_path(ep, cmds[0]);
	if (path == NULL)
	{
		free_split(cmds);
		return (1);
	}
	execve(path, cmds, ep);
	free(path);
	free_split(cmds);
	return (1);
}
