/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomarna <thomarna@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:27:53 by thomarna          #+#    #+#             */
/*   Updated: 2024/12/05 14:09:58 by thomarna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

void	err0r(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

char	*get_path(char **ep, char *cmd)
{
	char	**paths;
	char	*tmp;
	char	*path;
	int		i;

	i = 0;
	if (access(cmd, X_OK) == 0)
		return (cmd);
	while (*ep && ft_strnstr(*ep, "PATH", 4) == 0)
		ep++;
	if (!ep)
		return (NULL);
	paths = ft_split(*ep + 5, ':');
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
		{
			free_split(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_split(paths);
	return (0);
}

void	execmd(char **ep, char *av)
{
	char	**cmds;
	char	*path;

	cmds = ft_split(av, ' ');
	if (cmds == NULL)
		err0r();
	path = get_path(ep, cmds[0]);
	if (path == NULL)
	{
		free_split(cmds);
		err0r();
	}
	execve(path, cmds, ep);
	free(path);
	free_split(cmds);
}

void	parent(int *fd, char **av, char **ep)
{
	int	fdout;

	close(fd[1]);
	fdout = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fdout < 0)
		err0r();
	dup2(fd[0], 0);
	dup2(fdout, 1);
	close(fd[0]);
	close(fdout);
	execmd(ep, av[3]);
}

void	child(int *fd, char **av, char **ep)
{
	int	fdin;

	close(fd[0]);
	fdin = open(av[1], O_RDONLY);
	if (fdin < 0)
		err0r();
	dup2(fd[1], 1);
	dup2(fdin, 0);
	close(fd[1]);
	close(fdin);
	execmd(ep, av[2]);
}

int	main(int ac, char **av, char **ep)
{
	int		fd[2];
	pid_t	pid;

	if (ac != 5)
	{
		ft_dprintf(2, "Usage: %s infile cmd1 cmd2 outfile\n", av[0]);
		return (EXIT_FAILURE);
	}
	if (pipe(fd) < 0)
		err0r();
	pid = fork();
	if (pid < 0)
		err0r();
	if (pid)
		child(fd, av, ep);
	waitpid(pid, NULL, 0);
	parent(fd, av, ep);
	return (EXIT_SUCCESS);
}
