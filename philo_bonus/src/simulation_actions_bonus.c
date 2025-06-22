/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 11:33:12 by sayed             #+#    #+#             */
/*   Updated: 2025/06/22 22:49:25 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	process_eating(t_philo *p)
{
	check_death(p);
	print_state(p, EAT);
	p->last_meal_time = get_time();
	p->meals_eaten++;
	if (p->t->time_to_die < p->t->time_to_eat)
	{
		usleep(p->t->time_to_die * 1000);
		check_death(p);
		usleep((p->t->time_to_eat - p->t->time_to_die) * 1000);
	}
	else
		usleep(p->t->time_to_eat * 1000);
	sem_post(p->t->forks);
	sem_post(p->t->forks);
	if (p->t->meals_required > 0 && p->meals_eaten >= p->t->meals_required)
	{
		cleanup(p->t, 0);
		exit(0);
	}
	return (TRUE);
}

int	process_sleeping(t_philo *p)
{
	check_death(p);
	print_state(p, SLEEP);
	usleep(p->t->time_to_sleep * 1000);
	return (TRUE);
}

int	process_thinking(t_philo *p)
{
	check_death(p);
	print_state(p, THINK);
	return (TRUE);
}

void	one_philo(t_table *t)
{
	t->start_time = get_time();
	sem_wait(t->forks);
	printf("%ld %d %s\n", get_time() - t->start_time, t->philos[0].id, FORK);
	sem_post(t->forks);
	usleep(t->time_to_die * 1000);
	printf("%ld %d died\n", get_time() - t->start_time, t->philos[0].id);
	cleanup(t, 0);
}

void	print_state(t_philo *p, char *msg)
{
	check_death(p);
	sem_wait(p->t->print);
	printf("%ld %d %s\n", get_time() - p->t->start_time, p->id, msg);
	sem_post(p->t->print);
}
