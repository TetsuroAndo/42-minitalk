/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 03:06:23 by teando            #+#    #+#             */
/*   Updated: 2024/11/11 22:20:32 by teando           ###   ########.fr       */
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
	ft_printf("Sending...\n");
	return (process_id);
}

static int	send_bit(pid_t pid, unsigned char c)
{
	static int	bit = 0;

	while (bit < 8)
	{
		if ((c & (1 << bit)) != 0)
		{
			if (kill(pid, SIGUSR1) == -1)
			{
				ft_dprintf(STDERR_FILENO, "Error: Failed to send message.\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (kill(pid, SIGUSR2) == -1)
			{
				ft_dprintf(STDERR_FILENO, "Error: Failed to send message.\n");
				exit(EXIT_FAILURE);
			}
		}
		usleep(100);
		bit++;
	}
	return (0);
}

static void	send_str(pid_t pid, const char *str)
{
	if (!str || !pid)
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

static void	response_handler(int sig, siginfo_t *info, void *context)
{
	(void)context;
	if (info->si_pid == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: Received signal from invalid PID.\n");
		exit(EXIT_FAILURE);
	}
	if (sig == SIGUSR1)
		send_str(info->si_pid, NULL);
	if (sig == SIGUSR2)
	{
		ft_dprintf(STDOUT_FILENO, "Received SIGUSR2 signal. Exiting...\n");
		exit(EXIT_SUCCESS);
	}
}

int	main(int ac, char **av)
{
	struct sigaction	sig_set;
	pid_t				process_id;

	process_id = parse_input(ac, (const char **)av);
	sig_set.sa_flags = SA_SIGINFO;
	sig_set.sa_sigaction = response_handler;
	sigemptyset(&sig_set.sa_mask);
	if (sigaction(SIGUSR1, &sig_set, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, "Error: Failed to set up SIGUSR1 handler.\n");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2, &sig_set, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, "Error: Failed to set up SIGUSR2 handler.\n");
		exit(EXIT_FAILURE);
	}
	send_str(process_id, av[2]);
	while (1)
		pause();
	ft_printf("exiting...\n");
	return (0);
}
