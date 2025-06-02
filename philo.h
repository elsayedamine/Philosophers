/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 15:16:55 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/02 18:57:42 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

# define FALSE 0
# define TRUE 1
# define EAT "is eating"
# define FORK "has taken a fork"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DIE "died"

struct s_table;

typedef enum e_enum
{
	MALLOC,
	THREAD_INIT,
}	t_err;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_table	*table;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	eat_lock;
	pthread_mutex_t	last_meal_lock;
	// maybe a mutex to protect last_meal_time
}					t_philo;

typedef struct s_table
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	long			start_time;
	int				someone_died;
	int				all_full;
	int				itr;
	int				cleanup;
	pthread_t		monitor;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	meal_check;
	t_philo			*philos;
}					t_table;

int	myatoi(char *str);
int	simulation_init(t_table *table, int ac, char **av);
long	get_time(void);
void	print_state(t_philo *philo, char *msg);
void	*monitor(void *table);
void	*routine(void *arg);
int	simulation(t_table *table);
int	simulation_init(t_table *table, int ac, char **av);
int	init_philos	(t_table *table);
int	init_table(t_table *table, int ac, char **av);
int	destroy_table(t_table *table, int count, int philo_count);

#endif