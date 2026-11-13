/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rank.c                                             :::      ::::::::   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 12:00:00 by student           #+#    #+#             */
/*   Updated: 2026/11/13 12:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	check_duplicates(t_data *d)
{
	int	i;
	int	j;

	i = -1;
	while (++i < d->a.n)
	{
		j = i;
		while (++j < d->a.n)
			if (d->a.v[i] == d->a.v[j])
				fail(d);
	}
}

void	rank_input(t_data *d)
{
	int		*ranks;
	int		i;
	int		j;
	double	mistakes;

	check_duplicates(d);
	ranks = malloc(sizeof(int) * d->a.cap);
	if (!ranks)
		fail(d);
	mistakes = 0;
	i = -1;
	while (++i < d->a.n)
	{
		ranks[i] = 0;
		j = -1;
		while (++j < d->a.n)
		{
			ranks[i] += (d->a.v[j] < d->a.v[i]);
			mistakes += (j > i && d->a.v[i] > d->a.v[j]);
		}
	}
	if (d->a.n > 1)
		d->disorder = mistakes / ((double)d->a.n * (d->a.n - 1) / 2);
	free(d->a.v);
	d->a.v = ranks;
}
