/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damoncad <damoncad@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-04 19:15:04 by damoncad          #+#    #+#             */
/*   Updated: 2024-12-04 19:15:04 by damoncad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	death(t_data *data, t_philo *philo, int is_dead, long long current)
{
	pthread_mutex_lock(&data->done_mutex);
	if (!data->done)
	{
		data->done = 1;
		is_dead = 1;
		pthread_mutex_lock(&data->write_mutex);
		printf("%lld %d died\n", current - data->start_time, philo->id);
		pthread_mutex_unlock(&data->write_mutex);
	}
	pthread_mutex_unlock(&data->done_mutex);
	return (is_dead);
}

void	eat(t_philo *philo)
{
	t_data	*data;
	int		first_fork;
	int		second_fork;

	data = philo->data;
	if (data->num_philos == 1)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		print_status(data, philo->id, "has taken a fork");
		smart_sleep(data->time_to_die, data, philo);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		print_status(data, philo->id, "died");
		return ;
	}
	first_fork = philo->left_fork;
	second_fork = philo->right_fork;
	if (philo->id % 2 == 0)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	eating(philo, data, first_fork, second_fork);
}

void	eating(t_philo *philo, t_data *data, int first_fork, int second_fork)
{
	pthread_mutex_lock(&data->forks[first_fork]);
	if (try(data, philo))
	{
		pthread_mutex_unlock(&data->forks[first_fork]);
		return ;
	}
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&data->forks[second_fork]);
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&data->meal_mutex);
	philo->last_meal_time = get_time();
	data->last_meal_times[philo->id - 1] = philo->last_meal_time;
	philo->meals_eaten++;
	pthread_mutex_unlock(&data->meal_mutex);
	print_status(data, philo->id, "is eating");
	smart_sleep(data->time_to_eat, data, philo);
	pthread_mutex_unlock(&data->forks[second_fork]);
	pthread_mutex_unlock(&data->forks[first_fork]);
}

void	ft_sleep(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(data, philo->id, "is sleeping");
	smart_sleep(data->time_to_sleep, data, philo);
}

void	think(t_philo *philo, int in)
{
	long long	time_to_think;

	pthread_mutex_lock(&philo->data->done_mutex);
	if (philo->data->done)
	{
		pthread_mutex_unlock(&philo->data->done_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->done_mutex);
	if (in && philo->id % 2)
	{
		time_to_think = philo->data->time_to_eat / 2;
		print_status(philo->data, philo->id, "is thinking");
		smart_sleep(time_to_think, philo->data, philo);
	}
	else
		print_status(philo->data, philo->id, "is thinking");
}
