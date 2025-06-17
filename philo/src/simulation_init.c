/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sayed <sayed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 13:22:06 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/17 01:22:17 by sayed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	destroy_first_mutexes(t_table *t, int count)
{
	int	i;

	i = 0;
	while (i < count)
		pthread_mutex_destroy(&t->forks[i++]);
	free(t->forks);
	if (t->cleanup >= 1)
		pthread_mutex_destroy(&t->print_lock);
	if (t->cleanup >= 2)
		pthread_mutex_destroy(&t->death_lock);
}

int	destroy_table(t_table *table, int count, int philo_count)
{
	int	i;

	i = 0;
	destroy_first_mutexes(table, count);
	if (table->cleanup >= 3)
	{
		while (i < philo_count)
			pthread_mutex_destroy(&table->philos[i++].meal_lock);
		i = 0;
		while (i < philo_count)
			pthread_mutex_destroy(&table->philos[i++].eat_lock);
		free(table->philos);
	}
	if (table->cleanup >= 4)
		pthread_mutex_destroy(&table->meal_check);
	if (table->cleanup >= 5)
		while (i < philo_count)
			pthread_join(table->philos[i++].thread_id, NULL);
	return (FALSE);
}

int	fill_table(t_table *t, int ac, char **av)
{
	t->i = 0;
	t->cleanup = 0;
	t->all_full = 0;
	t->start_time = 0;
	t->someone_died = FALSE;
	t->nb_philo = myatoi(av[1]);
	if (t->nb_philo > 15000)
		return (write(2, "Excessive Number Of Philosophers !\n", 35));
	t->time_to_die = myatoi(av[2]);
	t->time_to_eat = myatoi(av[3]);
	t->time_to_sleep = myatoi(av[4]);
	t->meals_required = myatoi(av[5]);
	if (t->time_to_die == -1 || t->time_to_eat == -1 || \
		t->time_to_sleep == -1 || (t->meals_required == -1 && ac == 6) || \
		t->nb_philo < 0)
		return (write(2, "Invalid Args Input !\n", 21));
	return (TRUE);
}

int	init_table(t_table *t, int ac, char **av)
{
	(void)ac;
	if (fill_table(t, ac, av) != TRUE)
		return (FALSE);
	t->philos = NULL;
	t->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * t->nb_philo);
	if (!t->forks)
		return (FALSE);
	while (t->i < t->nb_philo)
		if (pthread_mutex_init(&t->forks[t->i++], NULL))
			return (destroy_table(t, t->i - 1, 0));
	t->cleanup++;
	if (pthread_mutex_init(&t->print_lock, NULL))
		return (destroy_table(t, t->nb_philo, 0));
	t->cleanup++;
	if (pthread_mutex_init(&t->death_lock, NULL))
		return (destroy_table(t, t->nb_philo, 0));
	return (t->cleanup++, TRUE);
}

int	init_philos(t_table *t)
{
	t->philos = malloc(sizeof(t_philo) * t->nb_philo);
	if (!t->philos)
		return (destroy_table(t, t->nb_philo, 0), FALSE);
	t->i = 0;
	while (t->i < t->nb_philo)
	{
		t->philos[t->i].id = t->i + 1;
		t->philos[t->i].meals_eaten = 0;
		t->philos[t->i].last_meal_time = 0;
		t->philos[t->i].left_fork = &t->forks[t->i];
		t->philos[t->i].right_fork = &t->forks[(t->i + 1) % t->nb_philo];
		t->philos[t->i].table = t;
		memset(&t->philos[t->i].eat_lock, 0, sizeof(pthread_mutex_t));
		memset(&t->philos[t->i].meal_lock, 0, sizeof(pthread_mutex_t));
		if (pthread_mutex_init(&t->philos[t->i].meal_lock, NULL))
			return (destroy_table(t, t->nb_philo, t->i));
		if (pthread_mutex_init(&t->philos[t->i].eat_lock, NULL))
			return (destroy_table(t, t->nb_philo, t->i));
		t->i++;
	}
	t->cleanup++;
	if (pthread_mutex_init(&t->meal_check, NULL))
		return (destroy_table(t, t->nb_philo, t->i));
	return (t->cleanup++, TRUE);
}
