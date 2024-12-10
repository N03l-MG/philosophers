/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:33:14 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/10 17:14:26 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_seat	*init_philo(int philo_num, char **args, int goal);
static t_seat	*setup_table(char **args, MUTEX *forks, int goal, int n);
static void		run_philo(t_seat *table, int num_philosophers);

int	main(int argc, char *argv[])
{
	t_seat	*table;
	int		meal_goal;
	long	start_time;

	start_time = current_time_ms();
	if (argc == 5)
		meal_goal = -1;
	else if (argc == 6)
		meal_goal = ft_atoi(argv[5]);
	else
	{
		printf("Invalid Arguments: n of phil, die time, eat time, sleep time,");
		printf(" (optional) eating goal\n");
		return (1);
	}
	table = init_philo(ft_atoi(argv[1]), argv + 1, meal_goal);
	if (!table)
	{
		printf("Failed to initialize philosophers\n");
		return (1);
	}
	run_philo(table, ft_atoi(argv[1]));
	return (0);
}

static t_seat	*init_philo(int philo_num, char **args, int goal)
{
	MUTEX	*forks;
	int		i;

	philo_num = ft_atoi(args[0]);
	forks = malloc(sizeof(MUTEX) * philo_num);
	if (!forks)
		return (NULL);
	i = -1;
	while (++i < philo_num)
		pthread_mutex_init(&forks[i], NULL);
	return (setup_table(args, forks, goal, philo_num));
}

static t_seat	*setup_table(char **args, MUTEX *forks, int goal, int n)
{
	t_seat	*first;
	t_seat	*prev;
	t_seat	*current;
	int		i;
	int		g_i[2];

	i = -1;
	first = NULL;
	prev = NULL;
	while (++i < n)
	{
		g_i[0] = goal;
		g_i[1] = i;
		current = malloc(sizeof(t_seat));
		current->philo = create_philo(args, g_i, &forks[i], current);
		pthread_mutex_init(&current->death_mutex, NULL);
		current->has_died = false;
		if (!first)
			first = current;
		if (prev)
			give_forks(prev, current);
		prev = current;
	}
	if (prev)
		give_forks(prev, first);
	return (first);
}

static void	run_philo(t_seat *table, int philo_num)
{
	pthread_t	*threads;
	t_seat		*current;
	int			i;

	threads = malloc(sizeof(pthread_t) * philo_num);
	if (!threads)
		return ;
	current = table;
	i = -1;
	while (++i < philo_num)
	{
		pthread_create(&threads[i], NULL, philo_routine, current->philo);
		current = current->next;
	}
	while (true)
		monitoring(table, current, threads, philo_num);
}
