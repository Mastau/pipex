/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomarna <thomarna@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:27:53 by thomarna          #+#    #+#             */
/*   Updated: 2024/12/10 11:24:02 by thomarna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	parent(int *fd, char **av, char **ep, pid_t pid)
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
		return (err0r(av[3]));
	waitpid(pid, NULL, 0);
	return (1);
}

int	child(int *fd, char **av, char **ep)
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
		return (err0r(av[2]));
	return (0);
}

int	main(int ac, char **av, char **ep)
{
	int		fd[2];
	pid_t	pid;
	int		ret;

	ret = 0;
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
		ret = child(fd, av, ep);
	else
		ret = parent(fd, av, ep, pid);
	waitpid(pid, NULL, 0);
	return (ret);
}
