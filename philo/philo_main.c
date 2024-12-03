/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:33:14 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/03 16:15:47 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	if (argc == 5)
		init_philo(argv + 1, INT_MAX);
	else if (argc == 6)
		init_philo(argv + 1, ft_atoi(argv[5]));
	else
	{
		printf("Invalid Arguments: n of phil, die time, eat time, sleep time,");
		printf(" (optional) eating goal\n");
		return (1);
	}
	return (0);
}

t_philo	*create_philo(char **args, int goal, int i)
{
	t_philo	*philosopher;

	philosopher = malloc(sizeof(t_philo));
	if (!philosopher)
		return (NULL);
	philosopher->philo_id = i;
	philosopher->time_to_die = ft_atoi(args[1]);
	philosopher->time_to_eat = ft_atoi(args[2]);
	philosopher->time_to_sleep = ft_atoi(args[3]);
	philosopher->goal_counter = goal;
}

void	init_philo(char **args, int goal)
{
	int		philo_num;
	t_seat	*first;
	t_seat	*prev;
	t_seat	*current;
	int		i;

	philo_num = ft_atoi(args[0]);
	first = NULL;
	prev = NULL;
	i = 0;
	while (i <= philo_num)
	{
		current = ft_lstnew(create_philo(args, goal, i));
		if (&current == NULL)
			return ;
		if (!first)
			first = current;
		if (prev)
			prev->next = current;
		prev = current;
		i++;
	}
	if (prev)
		prev->next = first;
}
