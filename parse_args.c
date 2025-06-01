/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_numbers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:38:25 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/01 21:51:18 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int n)
{
	if (n >= '0' && n <= '9')
		return (FALSE);
	return (TRUE);
}

int	onlydigit(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (FALSE);
	if (str[0] == '+')
		i++;
	while (str[i] && (ft_isdigit(str[i])))
		i++;
	if (!str[i])
		return (FALSE);
	return (TRUE);
}
// i should add ft_strlen && ft_strncmp
int	myatoi(char *str)
{
	unsigned int	i;
	int				sign;
	long			n;

	i = 0;
	sign = 1;
	n = 0;
	if (!str || !*str || onlydigit(str) == FALSE || strlen(str) > 11)
		return (-1);
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (!strncmp(&str[i], "2147483648", 10) || \
			!strncmp(&str[i], "+2147483648", 11))
		return (-1);
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] <= '9' && str[i] >= '0')
	{
		n = n * 10 + (str[i++] - '0');
		if (n > 2147483648)
			return (-1);
	}
	return ((n != 0) * sign + (n == 0) * -1);
}
