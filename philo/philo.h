/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 15:16:55 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/03 11:14:18 by aelsayed         ###   ########.fr       */
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
# define ARG_ERR "Usage:\n\t./philo number_of_philos time_to_die \
time_to_eat time_to_sleep [meals_required]\n"

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
	int				i;
	int				cleanup;
	pthread_t		monitor;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	meal_check;
	t_philo			*philos;
}					t_table;

int		myatoi(char *str);
int		simulation_init(t_table *table, int ac, char **av);
long	get_time(void);
void	print_state(t_philo *philo, char *msg);
void	*monitor(void *table);
void	*routine(void *philo);
int		simulation_play(t_table *table);
int		simulation_init(t_table *table, int ac, char **av);
int		init_philos	(t_table *table);
int		init_table(t_table *table, int ac, char **av);
int		destroy_table(t_table *table, int count, int philo_count);

int		process_eating(t_philo *p);
int		process_sleeping(t_philo *p);
int		process_thinking(t_philo *p);
int		is_dead_or_full(t_philo *p);
int		take_forks(t_philo *p);

#endif