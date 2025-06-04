/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 13:22:06 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/04 02:13:48 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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

int	init_mutexes(t_table *t)
{
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

int	init_table(t_table *t, int ac, char **av)
{
	(void)ac;
	if (fill_table(t, ac, av) != TRUE)
		return (FALSE);
	t->philos = NULL;
	t->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * t->nb_philo);
	if (!t->forks)
		return (FALSE);
	if (init_mutexes(t) == FALSE)
		return (FALSE);
	return (TRUE);
}
