/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 03:06:19 by teando            #+#    #+#             */
/*   Updated: 2024/11/12 04:01:07 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum e_errors
{
	SUCCESS,
	ERROR_IN_SIGACTION,
	ERROR_IN_PID,
	ERROR_IN_KILL,
	ERROR_IN_WRITE
}			t_errors;

static void	display_start_up(void)
{
	pid_t	process_id;

	process_id = getpid();
	ft_printf("■■   ■■ ■■■■ ■■   ■■ ■■■■ ■■■■■■     ■■  ■■      ■■  ■■\n");
	ft_printf("■■■ ■■■  ■■  ■■   ■■  ■■    ■■      ■■■  ■■      ■■ ■■\n");
	ft_printf("      ■            ■                  ■\n");
	ft_printf("■■ ■ ■■  ■■  ■■ ■ ■■  ■■    ■■     ■■■■  ■■      ■■■■\n");
	ft_printf("■■   ■■  ■■  ■■  ■■■  ■■    ■■    ■■■■■  ■■      ■■ ■■\n");
	ft_printf("■■   ■■  ■■  ■■   ■■  ■■    ■■   ■■  ■■  ■■      ■■  ■■\n");
	ft_printf("■■   ■■ ■■■■ ■■   ■■ ■■■■   ■■  ■■   ■■  ■■■■■■■ ■■   ■■\n");
	ft_printf("=========================================== SERVER =====\n");
	ft_printf("PID: %d\n", process_id);
	ft_printf("\n");
	ft_printf("Waiting for signal...\n");
}

static void	error_handler(int type)
{
	if (type == ERROR_IN_SIGACTION)
	{
		ft_dprintf(STDERR_FILENO, "Error: SIGACTION FAILURE\n");
		exit(EXIT_FAILURE);
	}
	else if (type == ERROR_IN_PID)
	{
		ft_dprintf(STDERR_FILENO, "Error: Invalid PID\n");
		exit(EXIT_FAILURE);
	}
	else if (type == ERROR_IN_KILL)
	{
		ft_dprintf(STDERR_FILENO, "Error: Failed to send Signal.\n");
		exit(EXIT_FAILURE);
	}
	else if (type == ERROR_IN_WRITE)
	{
		ft_dprintf(STDERR_FILENO, "Error: FAILURE STDOUT\n");
		exit(EXIT_FAILURE);
	}
}

static void	send_response(pid_t pid, int sigtype)
{
	if (sigtype == 1)
	{
		if (kill(pid, SIGUSR1) == -1)
			error_handler(ERROR_IN_KILL);
	}
	else if (sigtype == 2)
	{
		if (kill(pid, SIGUSR2) == -1)
			error_handler(ERROR_IN_KILL);
	}
}

static void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static unsigned char	tmp;
	static int				bit = 1;

	(void)context;
	if (info->si_pid == 0)
		error_handler(ERROR_IN_PID);
	if (signum == SIGUSR2)
		tmp += bit;
	bit *= 2;
	if (bit == 256)
	{
		// ft_printf(" (bit: %d) ", bit);
		// ft_printf("( tmp: %c) ", tmp);
		if (tmp == 0)
			send_response(info->si_pid, 2);
		if (tmp != 0)
			if (write(STDOUT_FILENO, &tmp, 1) == -1)
				error_handler(ERROR_IN_WRITE);
		tmp = 0;
		bit = 1;
	}
	send_response(info->si_pid, 1);
}

int	main(void)
{
	struct sigaction	sig_set;

	display_start_up();
	sig_set.sa_flags = SA_SIGINFO;
	sig_set.sa_sigaction = signal_handler;
	sigemptyset(&sig_set.sa_mask);
	if (sigaction(SIGUSR1, &sig_set, NULL) == -1)
		error_handler(ERROR_IN_SIGACTION);
	if (sigaction(SIGUSR2, &sig_set, NULL) == -1)
		error_handler(ERROR_IN_SIGACTION);
	while (1)
		pause();
	return (0);
}
