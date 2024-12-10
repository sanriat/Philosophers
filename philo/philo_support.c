/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_support.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damoncad <damoncad@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-04 19:09:57 by damoncad          #+#    #+#             */
/*   Updated: 2024-12-04 19:09:57 by damoncad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	long	result;

	result = 0;
	while (*str)
	{
		result = result * 10 + (*str - '0');
		if (result > 2147483647)
			return (-1);
		str++;
	}
	return ((int)result);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)(tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_data *data, int id, char *status)
{
	pthread_mutex_lock(&data->done_mutex);
	if (!data->done)
	{
		pthread_mutex_lock(&data->write_mutex);
		printf("%lld %d %s\n", get_time() - data->start_time, id, status);
		pthread_mutex_unlock(&data->write_mutex);
	}
	pthread_mutex_unlock(&data->done_mutex);
}

int	ips(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	smart_sleep(long long time, t_data *data, t_philo *philo)
{
	long long	start;
	long long	elapsed;

	start = get_time();
	while (1)
	{
		pthread_mutex_lock(&data->done_mutex);
		if (data->done)
		{
			pthread_mutex_unlock(&data->done_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->done_mutex);
		if (try(data, philo) == 1)
			return ;
		elapsed = get_time() - start;
		if (elapsed >= time)
			return ;
		usleep(100);
	}
}
