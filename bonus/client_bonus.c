/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 03:06:23 by teando            #+#    #+#             */
/*   Updated: 2024/11/12 05:53:45 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static pid_t	parse_input(const int ac, const char *av[])
{
	pid_t	process_id;

	if (ac != 3)
	{
		ft_dprintf(STDERR_FILENO, "Usage: %s [SERVER PID] [MESSAGE]\n", av[0]);
		exit(EXIT_FAILURE);
	}
	if (*av[1] == 0 || !ft_strfunc(av[1], ft_isdigit))
	{
		ft_dprintf(STDERR_FILENO, "Error: PID must be a number!\n");
		exit(EXIT_FAILURE);
	}
	process_id = ft_atoi(av[1]);
	if (process_id <= 100 || process_id >= 9999999)
	{
		ft_dprintf(STDERR_FILENO, "Error: Invalid PID.\n");
		exit(EXIT_FAILURE);
	}
	ft_printf("Send message to PID: %d\n", process_id);
	return (process_id);
}

static int	send_bit(pid_t pid, unsigned char c)
{
	static int	bit = 0;

	while (bit < 8)
	{
		if (((c >> bit) & 1) == 0)
		{
			if (kill(pid, SIGUSR1) == -1)
				return (-1);
		}
		else
		{
			if (kill(pid, SIGUSR2) == -1)
				return (-1);
		}
		bit++;
		usleep(1000);
	}
	bit = 0;
	return (0);
}

static void	send_str(pid_t pid, const char *str)
{
	if (!str)
		return ;
	while (*str)
	{
		if (send_bit(pid, (unsigned char)*str++) == -1)
		{
			ft_dprintf(STDERR_FILENO, "Error: Failed to send message.\n");
			exit(EXIT_FAILURE);
		}
	}
	send_bit(pid, '\0');
}

static void	response_handler(int sig)
{
	if (sig == SIGUSR1)
	{
		return ;
	}
	if (sig == SIGUSR2)
	{
		ft_printf("Sending Completed. Exiting...\n");
		exit(EXIT_SUCCESS);
	}
}

int	main(int ac, char **av)
{
	pid_t	process_id;

	process_id = parse_input(ac, (const char **)av);
	if (signal(SIGUSR1, response_handler) == SIG_ERR)
	{
		ft_dprintf(STDERR_FILENO, "Error: Failed to set up SIGUSR1 handler.\n");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGUSR2, response_handler) == SIG_ERR)
	{
		ft_dprintf(STDERR_FILENO, "Error: Failed to set up SIGUSR2 handler.\n");
		exit(EXIT_FAILURE);
	}
	send_str(process_id, av[2]);
	return (0);
}
