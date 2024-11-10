/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teando <teando@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 03:06:19 by teando            #+#    #+#             */
/*   Updated: 2024/11/10 14:25:55 by teando           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <signal.h>
#include <stddef.h>
#include <unistd.h>

void	display_start_up(void)
{
	int	process_id;

	process_id = getpid();
	ft_printf("■■   ■■ ■■■■ ■■   ■■ ■■■■ ■■■■■■     ■■  ■■      ■■  ■■\n");
	ft_printf("■■■ ■■■  ■■  ■■   ■■  ■■    ■■      ■■■  ■■      ■■ ■■\n");
	ft_printf("      ■            ■                  ■          ■\n");
	ft_printf("■■ ■ ■■  ■■  ■■ ■ ■■  ■■    ■■     ■■■■  ■■      ■■■■\n");
	ft_printf("■■   ■■  ■■  ■■  ■■■  ■■    ■■    ■■■■■  ■■      ■■ ■■\n");
	ft_printf("■■   ■■  ■■  ■■   ■■  ■■    ■■   ■■  ■■  ■■      ■■  ■■\n");
	ft_printf("■■   ■■ ■■■■ ■■   ■■ ■■■■   ■■  ■■   ■■  ■■■■■■■ ■■   ■■\n");
	ft_printf("******************************************* SERVER ******\n");
	ft_printf("PID: %d\n", process_id);
	ft_printf("\n");
	ft_printf("Waiting for a message...\n");
}

int	main(void)
{
	display_start_up();
	write(1, "あ", 1);
	return (0);
}
