/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 11:33:12 by sayed             #+#    #+#             */
/*   Updated: 2025/06/18 01:36:32 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

// int	process_eating(t_philo *p)
// {
// 	if (is_dead_or_full(p))
// 	{
// 		sem_post(p->t->forks);
// 		sem_post(p->t->forks);
// 		return (FALSE);
// 	}
// 	print_state(p, EAT);
// 	sem_wait(p->t->meal);
// 	p->last_meal_time = get_time();
// 	p->meals_eaten++;
// 	sem_post(p->t->meal);
// 	usleep(p->t->time_to_eat * 1000);
// 	sem_post(p->t->forks);
// 	sem_post(p->t->forks);
// 	if (p->t->meals_required > 0 && p->meals_eaten >= p->t->meals_required)
// 	{
// 		sem_wait(p->t->meal);
// 		p->t->all_full++;
// 		sem_post(p->t->meal);
// 		exit(0);
// 	}
// 	return (TRUE);
// }

// int	process_sleeping(t_philo *p)
// {
// 	if (is_dead_or_full(p))
// 		return (FALSE);
// 	print_state(p, SLEEP);
// 	usleep(p->t->time_to_sleep * 1000);
// 	return (TRUE);
// }

// int	process_thinking(t_philo *p)
// {
// 	if (is_dead_or_full(p))
// 		return (FALSE);
// 	print_state(p, THINK);
// 	return (TRUE);
// }

int	process_eating(t_philo *p)
{
	check_death(p);  // Check #2: before eating (covers death while holding forks)
	
	print_state(p, EAT);
	p->last_meal_time = get_time();
	p->meals_eaten++;
	
	usleep(p->t->time_to_eat * 1000);
	sem_post(p->t->forks);
	sem_post(p->t->forks);
	
	// Check if finished required meals
	if (p->t->meals_required > 0 && p->meals_eaten >= p->t->meals_required)
		exit(0);
	
	return (TRUE);
}

int	process_sleeping(t_philo *p)
{
	// check_death(p);
	print_state(p, SLEEP);
	usleep(p->t->time_to_sleep * 1000);
	return (TRUE);
}

int	process_thinking(t_philo *p)
{
	// check_death(p);
	print_state(p, THINK);
	return (TRUE);
}

void	print_state(t_philo *p, char *msg)
{
	sem_wait(p->t->print);
	sem_wait(p->t->death);
	printf("%ld %d %s\n", get_time() - p->t->start_time, p->id, msg);
	sem_post(p->t->death);
	sem_post(p->t->print);
}

int	is_dead_or_full(t_philo *p)
{
	int	res;

	sem_wait(p->t->meal);
	sem_wait(p->t->death);
	res = (p->t->someone_died || p->t->all_full == p->t->nb_philo);
	sem_post(p->t->death);
	sem_post(p->t->meal);
	return (res);
}
