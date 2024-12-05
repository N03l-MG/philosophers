/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:33:14 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/05 17:03:41 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*create_philo(char **args, int goal, int i, pthread_mutex_t *left_fork, t_seat *seat);
t_seat	*init_philo(int philo_num, char **args, int goal);
void	*philo_routine(void *arg);
void	start(t_seat *table, int num_philosophers);

int	main(int argc, char *argv[])
{
	t_seat	*table;
	int		meal_goal;

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
	start(table, ft_atoi(argv[1]));
	return (0);
}

t_philo	*create_philo(char **args, int goal, int i, pthread_mutex_t *left_fork, t_seat *seat)
{
	t_philo	*philosopher;

	philosopher = malloc(sizeof(t_philo));
	if (!philosopher)
		return (NULL);
	philosopher->philo_id = i;
	philosopher->time_to_die = ft_atoi(args[1]);
	philosopher->time_to_eat = ft_atoi(args[2]);
	philosopher->time_to_sleep = ft_atoi(args[3]);
	philosopher->eat_counter = goal;
	philosopher->left_fork = left_fork;
	philosopher->right_fork = NULL;
	philosopher->last_meal_time = current_time_ms();
	philosopher->seat = seat;
	return (philosopher);
}

t_seat	*init_philo(int philo_num, char **args, int goal)
{
	t_seat			*first;
	t_seat			*prev;
	t_seat			*current;
	pthread_mutex_t	*forks;
	int				i;

	philo_num = ft_atoi(args[0]);
	first = NULL;
	prev = NULL;
	forks = malloc(sizeof(pthread_mutex_t) * philo_num);
	if (!forks)
		return (NULL);
	i = -1;
	while (++i < philo_num)
		pthread_mutex_init(&forks[i], NULL);
	i = -1;
	while (++i < philo_num)
	{
		current = malloc(sizeof(t_seat));
		if (!current)
			return (NULL);
		current->philosopher = create_philo(args, goal, i, &forks[i], current);
		if (!current->philosopher)
			return (NULL);
		pthread_mutex_init(&current->death_mutex, NULL);
		current->has_died = false;
		if (!first)
			first = current;
		if (prev)
		{
			prev->next = current;
			current->philosopher->right_fork = prev->philosopher->left_fork;
		}
		prev = current;
	}
	if (prev)
	{
		prev->next = first;
		first->philosopher->right_fork = prev->philosopher->left_fork;
	}
	return (first);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_seat	*seat;
	long	current_time;

	philo = (t_philo *)arg;
	seat = philo->seat;
	while (philo->eat_counter != 0)
	{
		pthread_mutex_lock(&seat->death_mutex);
		if (seat->has_died)
		{
			pthread_mutex_unlock(&seat->death_mutex);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&seat->death_mutex);
		current_time = current_time_ms();
		if (current_time - philo->last_meal_time > philo->time_to_die)
		{
			pthread_mutex_lock(&seat->death_mutex);
			if (!seat->has_died)
			{
				seat->has_died = true;
				printf("%ld %d died\n", current_time, philo->philo_id);
			}
			pthread_mutex_unlock(&seat->death_mutex);
			pthread_exit(NULL);
		}
		printf("%ld %d is thinking\n", current_time, philo->philo_id);
		usleep(1000);
		if (philo->philo_id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
			pthread_mutex_lock(philo->right_fork);
			printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
			pthread_mutex_lock(philo->left_fork);
			printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
		}
		current_time = current_time_ms();
		printf("%ld %d is eating\n", current_time, philo->philo_id);
		philo->last_meal_time = current_time;
		usleep(philo->time_to_eat * 1000);
		if (philo->eat_counter > 0)
			philo->eat_counter--;
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		current_time = current_time_ms();
		printf("%ld %d is sleeping\n", current_time, philo->philo_id);
		usleep(philo->time_to_sleep * 1000);
	}
	return (NULL);
}

void start(t_seat *table, int num_philosophers)
{
	pthread_t *threads;
	t_seat *current;
	int i;

	threads = malloc(sizeof(pthread_t) * num_philosophers);
	if (!threads)
		return ;
	current = table;
	for (i = 0; i < num_philosophers; i++)
	{
		pthread_create(&threads[i], NULL, philo_routine, current->philosopher);
		current = current->next;
	}
	while (1)
	{
		current = table;
		for (i = 0; i < num_philosophers; i++)
		{
			pthread_mutex_lock(&current->death_mutex);
			if (current->has_died)
			{
				pthread_mutex_unlock(&current->death_mutex);
				for (int j = 0; j < num_philosophers; j++)
					pthread_cancel(threads[j]);
				for (int j = 0; j < num_philosophers; j++)
					pthread_join(threads[j], NULL);
				free(threads);
				return ;
			}
			pthread_mutex_unlock(&current->death_mutex);
			current = current->next;
		}
		usleep(1000);
	}
}
