/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomarna <thomarna@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:27:53 by thomarna          #+#    #+#             */
/*   Updated: 2024/12/13 15:18:35 by thomarna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	child1(int *fd, char **av, char **ep)
{
	int	fdin;

	close(fd[0]);
	fdin = open(av[1], O_RDONLY);
	if (fdin < 0)
	{
		close(fd[1]);
		return (err0r(av[1]));
	}
	dup2(fdin, 0);
	close(fdin);
	dup2(fd[1], 1);
	close(fd[1]);
	if (execmd(ep, av[2]))
		return (err0r(av[2]));
	return (1);
}

int	child2(int *fd, char **av, char **ep)
{
	int	fdout;

	close(fd[1]);
	fdout = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fdout < 0)
	{
		close(fd[0]);
		return (err0r(av[4]));
	}
	dup2(fd[0], 0);
	close(fd[0]);
	dup2(fdout, 1);
	close(fdout);
	execmd(ep, av[3]);
	err0r(av[3]);
	return (1);
}

int	main(int ac, char **av, char **ep)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (ac != 5)
	{
		ft_dprintf(2, "Usage: %s infile cmd1 cmd2 outfile\n", av[0]);
		return (1);
	}
	if (pipe(fd) < 0)
		err0r("");
	pid1 = fork();
	if (pid1 == 0)
		return (child1(fd, av, ep));
	pid2 = fork();
	if (pid2 == 0)
		return (child2(fd, av, ep));
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}

/* int pipex(char **cmd, char **env) { */
/* 	int pid1; */
/* 	int pid2; */
/*  */
/* 	pid1 = fork(); */
/* 	if (!pid1) { */
/*  */
/* 	} */
/* 	else { */
/* 		pid2 = fork(); */
/* 		if (!pid2) { */
/*  */
/* 		} */
/* 		else { */
/*  */
/* 		} */
/* 		waitpid(-1, NULL, WUNTRACED); */
/* 				*/
/*  */
/*  */
/* 	} */
/*  */
/* } */
