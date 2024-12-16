/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:27:43 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/16 16:27:47 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long duration_ms)
{
	long	start_time;
	long	current_time;

	start_time = current_time_ms();
	while (true)
	{
		current_time = current_time_ms();
		if ((current_time - start_time) >= duration_ms)
			break ;
		usleep(100);
	}
}
