/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:59:40 by sayed             #+#    #+#             */
/*   Updated: 2025/06/22 22:49:55 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	check_death(t_philo *p)
{
	if (get_time() - p->last_meal_time >= p->t->time_to_die)
	{
		sem_wait(p->t->death);
		sem_wait(p->t->print);
		printf("%ld %d died\n", get_time() - p->t->start_time, p->id);
		sem_post(p->t->forks);
		sem_post(p->t->forks);
		cleanup(p->t, 0);
		exit(1);
	}
	return (TRUE);
}

void	routine(t_philo *p)
{
	p->last_meal_time = get_time();
	if (p->id % 2)
		usleep(p->t->time_to_eat * 500);
	while (TRUE)
	{
		check_death(p);
		sem_wait(p->t->forks);
		print_state(p, FORK);
		usleep(100);
		sem_wait(p->t->forks);
		print_state(p, FORK);
		if (process_eating(p) == FALSE)
			continue ;
		if (process_sleeping(p) == FALSE)
			continue ;
		if (process_thinking(p) == FALSE)
			continue ;
		usleep(100);
	}
	cleanup(p->t, 0);
	exit(0);
}

void	monitor(t_table *t)
{
	int		status;
	int		i;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	i = -1;
	if (pid > 0 && WIFEXITED(status) && WEXITSTATUS(status) == 1)
		while (++i < t->nb_philo)
			if (t->philos[i].pid > 0)
				kill(t->philos[i].pid, SIGKILL);
	i = 0;
	while (i < t->nb_philo)
		if (t->philos[i++].pid > 0)
			waitpid(t->philos[i - 1].pid, NULL, 0);
	cleanup(t, 0);
	exit(0);
}

int	simulation(t_table *t)
{
	t->i = 0;
	t->start_time = get_time();
	while (t->i < t->nb_philo)
	{
		t->philos[t->i].pid = fork();
		if (t->philos[t->i].pid == 0)
			routine(&t->philos[t->i]);
		if (t->philos[t->i].pid < 0)
			return (perror("fork"), cleanup(t, 0), exit(1), 1);
		t->i++;
	}
	monitor(t);
	return (TRUE);
}
