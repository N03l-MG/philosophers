/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:33:17 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/05 16:56:35 by nmonzon          ###   ########.fr       */
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

typedef struct s_state
{
	bool			has_died;
	pthread_mutex_t	death_mutex;
	int				completed_meals;
}	t_state;

// Philosopher struct
typedef struct s_philo
{
	int				philo_id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			last_meal_time;
	int				eat_counter;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_seat	*seat;
	t_state			*state;
}	t_philo;

// Circular linked list node to form the round table
typedef struct s_seat
{
	t_philo			*philosopher;
	struct s_seat	*next;
}	t_seat;

int		ft_atoi(const char *str);
t_seat	*ft_lstnew(void *content);
long	current_time_ms(void);

#endif
