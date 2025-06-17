/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_init_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sayed <sayed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:50:12 by sayed             #+#    #+#             */
/*   Updated: 2025/06/17 11:07:19 by sayed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	fill_table(t_table *t, int ac, char **av)
{
	t->i = 0;
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

int	cleanup(t_table *t, int i)
{
	if (i == -1)
		return (sem_unlink("/forks"), sem_unlink("/print"), \
			sem_unlink("/death"), 0);
	if (i == 1)
		return (sem_unlink("/forks"), \
			perror("sem_open /print"), exit(1), FALSE);
	if (i == 2)
		return (sem_unlink("/forks"), sem_unlink("/print"), \
			perror("sem_open /death"), exit(1), FALSE);
	if (i == 3)
		return (sem_unlink("/forks"), sem_unlink("/print"), \
			sem_unlink("/death"), exit(1), FALSE);
	if (i == 4)
		return (sem_unlink("/forks"), sem_unlink("/print"), \
			sem_unlink("/death"), perror("sem_open /meal"), \
				exit(1), FALSE);
	if (i == 0)
		return (sem_unlink("/forks"), sem_unlink("/print"), \
			sem_unlink("/death"), sem_unlink("/meal"), \
				exit(1), free(t->philos), FALSE);
	return (FALSE);
}

int	init_table(t_table *t, int ac, char **av)
{
	(void)ac;
	if (fill_table(t, ac, av) != TRUE)
		return (exit(1), FALSE);
	t->philos = NULL;
	t->pids = NULL;
	cleanup(t, -1);
	t->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, t->nb_philo);
	if (t->forks == SEM_FAILED)
		return (perror("sem_open /forks"), exit(1), FALSE);
	t->print = sem_open("/print", O_CREAT, 0644, 1);
	if (t->print == SEM_FAILED)
		return (cleanup(t, 1));
	t->death = sem_open("/death", O_CREAT, 0644, 1);
	if (t->death == SEM_FAILED)
		return (cleanup(t, 2));
	t->meal = sem_open("/meal", O_CREAT, 0644, 1);
	if (t->meal == SEM_FAILED)
		return (cleanup(t, 4));
	return (TRUE);
}

int	init_philos(t_table *t)
{
	t->philos = malloc(sizeof(t_philo) * t->nb_philo);
	if (!t->philos)
		return (cleanup(t, 3), FALSE);
	t->i = 0;
	while (t->i < t->nb_philo)
	{
		t->philos[t->i].id = t->i + 1;
		t->philos[t->i].meals_eaten = 0;
		t->philos[t->i].last_meal_time = 0;
		t->philos[t->i].pid = 0;
		t->philos[t->i].death = 0;
		t->philos[t->i].t = t;
		t->i++;
	}
	return (TRUE);
}
