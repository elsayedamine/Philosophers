/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 21:01:16 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/03 11:17:46 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	destroy_table(t_table *table, int count, int philo_count)
{
	int i;
	int	end;

	i = 0;
	while (i < count)
		pthread_mutex_destroy(&table->forks[i++]);
	free(table->forks);
	if (table->cleanup >= 1)
		pthread_mutex_destroy(&table->print_lock);
	if (table->cleanup >= 2)
		pthread_mutex_destroy(&table->death_lock);
	if (table->cleanup >= 3)
	if (table->cleanup >= 3 && table->philos)
		free(table->philos);
	i = 0;
	if (table->cleanup >= 5)
	{
		end = (table->cleanup > 5) * \
			table->nb_philo + (table->cleanup == 5) + philo_count;
		while (i < philo_count)
			pthread_mutex_destroy(&table->philos[i++].meal_lock);
		i = 0;
		while (i < philo_count)
			pthread_mutex_destroy(&table->philos[i++].eat_lock);
	}
	i = 0;
	if (table->cleanup >= 6)
		while (i < philo_count)
			pthread_join(table->philos[i++].thread_id, NULL);
	return (FALSE);
}

int	init_table(t_table *table, int ac, char **av)
{
	table->i = 0;
	table->cleanup = 0;
	table->all_full = 0;
	table->nb_philo = myatoi(av[1]);
	table->time_to_die = myatoi(av[2]);
	table->time_to_eat = myatoi(av[3]);
	table->time_to_sleep = myatoi(av[4]);
	table->meals_required = myatoi(av[5]);
	if (table->nb_philo <= 0 || table->time_to_die == -1 || \
		table->time_to_eat == -1 || table->time_to_sleep == -1 || \
			(ac == 6 && table->meals_required == -1))
		return (FALSE);
	table->start_time = 0;
	table->someone_died = FALSE;
	table->philos = NULL;
	table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
		return (FALSE);
	while (table->i < table->nb_philo)
		if (pthread_mutex_init(&table->forks[table->i++], NULL))
			return (destroy_table(table, table->i - 1, 0));
	if (pthread_mutex_init(&table->print_lock, NULL))
		return (destroy_table(table, table->nb_philo, 0));
	if (pthread_mutex_init(&table->death_lock, NULL))
		return (table->cleanup++, destroy_table(table, table->nb_philo, 0));
	return (table->cleanup++, TRUE);
}

int	init_philos	(t_table *table)
{	
	table->philos = malloc(sizeof(t_philo) * table->nb_philo);
	if (!table->philos)
		return (destroy_table(table, table->nb_philo, 0), FALSE);
	table->i = 0;
	table->cleanup++;
	while (table->i < table->nb_philo)
	{
		table->philos[table->i].id = table->i + 1;
		table->philos[table->i].meals_eaten = 0;
		table->philos[table->i].last_meal_time = 0;
		table->philos[table->i].left_fork = &table->forks[table->i];
		table->philos[table->i].right_fork = &table->forks[(table->i + 1) % table->nb_philo];
		table->philos[table->i].table = table;
		memset(&table->philos[table->i].eat_lock, 0, sizeof(pthread_mutex_t));
		memset(&table->philos[table->i].meal_lock, 0, sizeof(pthread_mutex_t));
		memset(&table->philos[table->i].last_meal_lock, 0, sizeof(pthread_mutex_t));
		if (pthread_mutex_init(&table->philos[table->i].meal_lock, NULL))
			return (destroy_table(table, table->nb_philo, table->i));
		if (pthread_mutex_init(&table->philos[table->i].eat_lock, NULL))
			return (destroy_table(table, table->nb_philo, table->i));
		if (pthread_mutex_init(&table->meal_check, NULL))
			return (destroy_table(table, table->nb_philo, table->i));
		// adjust cleanup var for this new mutex_init
		table->i++;
	}
	return (TRUE);
}

int	simulation_init(t_table *table, int ac, char **av)
{
	if (init_table(table, ac, av) == FALSE)
		return (FALSE);
	if (init_philos(table) == FALSE)
		return (FALSE);
	table->cleanup++;
	return (TRUE);
}
