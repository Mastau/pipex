/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thomarna <thomarna@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:27:11 by thomarna          #+#    #+#             */
/*   Updated: 2024/12/14 17:28:02 by thomarna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

int		err0r(char *cmd);
char	*get_path(char **ep, char *cmd);
int		execmd(char **ep, char *av);
int		child1(int *fd, char **av, char **ep);
int		child2(int *fd, char **av, char **ep);
#endif
