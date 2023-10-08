/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 18:22:01 by khanhayf          #+#    #+#             */
/*   Updated: 2023/10/08 13:01:27 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libc.h>

void	ft_putstr(char *s)
{
	int	i = 0;

	if (!s)
		return ;
	while (s[i])
		write (2, &s[i++], 1);
}

void	ft_puterror(char *s1, char *s2)
{
	ft_putstr(s1);
	ft_putstr(s2);
	ft_putstr("\n");
}

int	ft_exec(char **av, int i)
{
	av[i] = NULL;
	execve(av[0], av, NULL);
	ft_puterror("error: cannot execute ", av[0]);
	return (1);
}

int	main(int ac, char **av, char **env)
{
	int	i;
	int	pipe_end[2];

	i = 0;
	av++;
	(void)ac;
	(void)env;
	s_out = dup(1);
	while (av[i])
	{
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (!strcmp(av[0], "cd"))
		{
			if (i != 2)
				ft_puterror("error: cd: bad arguments", NULL);
			else if (chdir(av[1]) < 0)
				ft_puterror("error: cd: cannot change directory to ", av[1]);
		}
		else if (i != 0 && (!av[i] || !strcmp(av[i], ";")))
		{
			if (fork() == 0)
			{
				if (ft_exec(av, i))
					return (1);
			}
			while (waitpid(-1, NULL, 0) != -1)
			;
		}
		else if (i != 0 && !strcmp(av[i], "|"))
		{ 
			pipe(pipe_end);
			if (fork() == 0)
			{
				close(pipe_end[0]);
				dup2(pipe_end[1], 1);
				close(pipe_end[1]);
				if (ft_exec(av, i))
					return (1);
			}
			close (pipe_end[1]);
			dup2(pipe_end[0], 0);
			close(pipe_end[0]);
		}
		if (av[i])
		{
			i++;
			av += i;
			i = 0;
		}
	}
}
