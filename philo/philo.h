/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:33:17 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/10 17:19:44 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

# ifndef MUTEX // Because its length was Fing up my norm.
#  define MUTEX pthread_mutex_t
# endif

typedef enum e_actions
{
	DIE,
	FORK,
	EAT,
	THINK,
	SLEEP
}	t_action;

// Philosopher struct
typedef struct s_philo
{
	int				philo_id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			last_meal_time;
	int				eat_counter;
	MUTEX			*left_fork;
	MUTEX			*right_fork;
	struct s_seat	*seat;
}	t_philo;

// Circular linked list node to form the round table
typedef struct s_seat
{
	t_philo			*philo;
	struct s_seat	*next;
	MUTEX			death_mutex;
	bool			has_died;
}	t_seat;

// Util functions
int		ft_atoi(const char *str);
t_seat	*ft_lstnew(void *content);
long	current_time_ms(void);

// Philo functions
t_philo	*create_philo(char **args, int *g_and_i, MUTEX *l_fork, t_seat *seat);
void	give_forks(t_seat *prev, t_seat *current);
void	monitoring(t_seat *table, t_seat *current, pthread_t *threads, int n);
void	*philo_routine(void *arg);

#endif
