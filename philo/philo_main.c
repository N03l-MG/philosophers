/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:33:14 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/16 16:44:24 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_seat	*init_philo(int philo_num, char **args, int goal);
static t_seat	*setup_table(char **args, MUTEX *forks, int goal, int n);
static void		run_philo(t_seat *table, int philo_num, long start_time);

void	leaks(void)
{
	system("leaks philo");
}

int	main(int argc, char *argv[])
{
	t_seat	*table;
	int		meal_goal;
	long	start_time;

	start_time = current_time_ms();
	if (argc == 5 && ft_atoi(argv[1]) != 0)
		meal_goal = -1;
	else if (argc == 6 && ft_atoi(argv[1]) != 0)
		meal_goal = ft_atoi(argv[5]);
	else
	{
		printf("Invalid Arguments: n of phil (min 1), die time, eat time, "
			"sleep time, (optional) eating goal\n");
		return (1);
	}
	table = init_philo(ft_atoi(argv[1]), argv + 1, meal_goal);
	if (!table)
	{
		printf("Failed to initialize philosophers\n");
		return (1);
	}
	run_philo(table, ft_atoi(argv[1]), start_time);
	atexit(leaks);
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
		give_forks(prev, current);
		prev = current;
	}
	give_forks(prev, first);
	return (first);
}

static void	run_philo(t_seat *table, int philo_num, long start_time)
{
	pthread_t		*threads;
	t_routine_args	*args;
	t_seat			*current;
	int				i;

	threads = malloc(sizeof(pthread_t) * philo_num);
	if (!threads)
		return ;
	current = table;
	i = -1;
	while (++i < philo_num)
	{
		args = malloc(sizeof(t_routine_args));
		if (!args)
			return ;
		args->philo = current->philo;
		args->start_time = start_time;
		args->philo_n = philo_num;
		current->args = args;
		pthread_create(&threads[i], NULL, philo_routine, args);
		current = current->next;
	}
	monitoring(table, threads, philo_num);
	free(threads);
}
