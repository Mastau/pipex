/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomarna <thomarna@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:27:53 by thomarna          #+#    #+#             */
/*   Updated: 2024/12/09 17:50:29 by thomarna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void	ft_close_std(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

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
	ft_close_std();
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

void	parent(int *fd, char **av, char **ep, pid_t pid)
{
	int	fdout;

	close(fd[1]);
	fdout = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fdout < 0)
	{
		close(fd[0]);
		err0r(av[4]);
	}
	dup2(fd[0], 0);
	dup2(fdout, 1);
	close(fd[0]);
	close(fdout);
	if (execmd(ep, av[3]))
		err0r(av[3]);
	waitpid(pid, NULL, 0);
}

void	child(int *fd, char **av, char **ep)
{
	int	fdin;

	close(fd[0]);
	fdin = open(av[1], O_RDONLY);
	if (fdin < 0)
	{
		close(fd[1]);
		err0r(av[1]);
	}
	dup2(fd[1], 1);
	dup2(fdin, 0);
	close(fd[1]);
	close(fdin);
	if (execmd(ep, av[2]))
		err0r(av[2]);
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
		err0r("");
	pid = fork();
	if (pid < 0)
		err0r("");
	if (pid == 0)
		child(fd, av, ep);
	else
		parent(fd, av, ep, pid);
}
