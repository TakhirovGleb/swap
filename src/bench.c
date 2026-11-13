/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :::      ::::::::   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 12:00:00 by student           #+#    #+#             */
/*   Updated: 2026/11/13 12:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	strategy(t_data *d, int selected)
{
	putstr(2, "[bench] strategy: ");
	if (d->mode == -1 || d->mode == 3)
		putstr(2, "adaptive / ");
	if (selected == 0)
		putstr(2, "simple O(n^2)\n");
	if (selected == 1)
		putstr(2, "medium O(n*sqrt(n))\n");
	if (selected == 2)
		putstr(2, "complex O(n*log(n))\n");
}

static void	operation_counts(t_data *d)
{
	int	i;

	i = -1;
	while (++i < 11)
	{
		putstr(2, "[bench] ");
		putstr(2, op_name(i));
		putstr(2, ": ");
		putnum(2, d->counts[i]);
		putstr(2, "\n");
	}
}

void	benchmark(t_data *d, int selected)
{
	size_t	total;
	int		i;
	int		percent;

	percent = (int)(d->disorder * 10000 + 0.5);
	putstr(2, "[bench] disorder: ");
	putnum(2, percent / 100);
	putstr(2, ".");
	if (percent % 100 < 10)
		putstr(2, "0");
	putnum(2, percent % 100);
	putstr(2, "%\n");
	strategy(d, selected);
	total = 0;
	i = -1;
	while (++i < 11)
		total += d->counts[i];
	putstr(2, "[bench] total: ");
	putnum(2, total);
	putstr(2, "\n");
	operation_counts(d);
}
