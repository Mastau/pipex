/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomarna <thomarna@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:27:53 by thomarna          #+#    #+#             */
/*   Updated: 2024/12/04 18:21:19 by thomarna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

char	*get_path(char **ep, char *cmd)
{
	char	**paths;
	char	*tmp;
	char	*path;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	while (ft_strnstr(*ep, "PATH", 4) == 0)
		ep++;
	if (!ep)
		return (NULL);
	*ep += 5;
	paths = ft_split(*ep, ':');
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
			break;
		paths++;
	}
	return (path);
}

void	execmd(char **ep, char *av)
{
	char	**cmds;
	char	*path;

	cmds = ft_split(av, ' ');
	path = get_path(ep, cmds[0]);
	ft_dprintf(2, "%s\n", path);
	execve(path, cmds, ep);
}

void	parent(int *fd, char **av, char **ep)
{
	int	fdout;

	fdout = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	dup2(fd[0], 0);
	dup2(fdout, 1);
	close(fd[1]);
	execmd(ep, av[3]);
}

void	child(int *fd, char **av, char **ep)
{
	int	fdin;

	fdin = open(av[1], O_RDONLY);
	dup2(fd[1], 1);
	dup2(fdin, 0);
	close(fd[0]);
	execmd(ep, av[2]);
}

int	main(int ac, char **av, char **ep)
{
	int		fd[2];
	pid_t	pid;

	if (ac != 5)
		return (0);
	pipe(fd);
	pid = fork();
	if (pid)
		child(fd, av, ep);
	waitpid(pid, NULL, 0);
	parent(fd, av, ep);
}
