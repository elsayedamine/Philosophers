/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 02:21:58 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/04 02:22:42 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	leave_forks(t_philo *p)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
	else
	{
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
	}
}

int	take_forks(t_philo *p)
{
	if (is_dead_or_full(p))
		return (FALSE);
	if (p->id % 2 != 0)
	{
		pthread_mutex_lock(p->left_fork);
		print_state(p, FORK);
		pthread_mutex_lock(p->right_fork);
		print_state(p, FORK);
	}
	else
	{
		pthread_mutex_lock(p->right_fork);
		print_state(p, FORK);
		pthread_mutex_lock(p->left_fork);
		print_state(p, FORK);
	}
	if (is_dead_or_full(p))
	{
		leave_forks(p);
		return (FALSE);
	}
	return (TRUE);
}
