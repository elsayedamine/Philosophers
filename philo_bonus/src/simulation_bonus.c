/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sayed <sayed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:59:40 by sayed             #+#    #+#             */
/*   Updated: 2025/06/17 18:06:21 by sayed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	monitor(t_table *t)
{
	int		status;
	int		i;
	pid_t	pid;

	// just to wait to any child i do -1
	pid = waitpid(-1, &status, 0);
	i = -1;
	if (pid > 0 && WIFEXITED(status) && WEXITSTATUS(status) == 1)
		while (++i < t->nb_philo)
			if (t->philos[i].pid != pid)
				kill(t->philos[i].pid, SIGKILL);
	i = 0;
	while (i < t->nb_philo)
		waitpid(t->philos[i++].pid, NULL, 0);
	sem_close(t->forks);
	sem_close(t->print);
	sem_close(t->death);
	cleanup(t, 0);
}

void	routine(t_philo *p)
{
	while (TRUE)
	{
		if (is_dead_or_full(p))
			break ;
		sem_wait(p->t->forks);
		print_state(p, FORK);
		sem_wait(p->t->forks);
		print_state(p, FORK);
		if (process_eating(p) == FALSE)
			continue ;
		if (process_sleeping(p) == FALSE)
			continue ;
		if (process_thinking(p) == FALSE)
			continue ;
		usleep(1000);
	}
	exit(1);
}

void	check_death_and_full(t_philo *p, long now, int full)
{
	sem_wait(p->t->meal);
	if (now - p->last_meal_time >= p->t->time_to_die)
	{
		sem_wait(p->t->death);
		p->t->someone_died = TRUE;
		sem_post(p->t->death);
		sem_wait(p->t->print);
		printf("%ld %d died\n", get_time() - \
		p->t->start_time, p->t->philos[p->t->i].id);
		sem_post(p->t->print);
		sem_post(p->t->meal);
		exit(1);
	}
	if (p->t->meals_required != -1)
	{
		p->t->i = -1;
		while (++p->t->i < p->t->nb_philo)
			if (p->t->philos[p->t->i].meals_eaten >= \
				p->t->meals_required)
				full++;
		if (full == p->t->nb_philo)
		{
			sem_post(p->t->meal);
			sem_wait(p->t->death);
			p->t->all_full = p->t->nb_philo;
			return (sem_post(p->t->death), exit(0));
		}
	}
	sem_post(p->t->meal);
}

void	*death_monitor(void *philo)
{
	t_philo *p;

	p = (t_philo *)philo;
	while (TRUE)
	{
		sem_wait(p->t->death);
		if (p->t->someone_died || p->t->all_full == p->t->nb_philo)
		{
			sem_post(p->t->death);
			break ;
		}
		sem_post(p->t->death);
		check_death_and_full(p, get_time(), 0);
		usleep(500);
	}
	return (NULL);
}

int simulation(t_table *t)
{
	t->i = 0;
	t->start_time = get_time();
	while (t->i < t->nb_philo)
	{
		t->philos[t->i].last_meal_time = get_time();
		t->philos[t->i].pid = fork();
		if (t->philos[t->i].pid < 0)
			return (perror("fork"), cleanup(t, 0));
		if (t->philos[t->i].pid == 0)
		{
			pthread_create(&t->philos[t->i].death, NULL, \
				death_monitor, &t->philos[t->i]);
			pthread_detach(t->philos[t->i].death);
			routine(&t->philos[t->i]);
		}
		t->i++;
	}
	monitor(t);
	return (TRUE);
}
