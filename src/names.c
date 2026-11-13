/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.c                                            :::      ::::::::   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 12:00:00 by student           #+#    #+#             */
/*   Updated: 2026/11/13 12:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

char	*op_name(int code)
{
	if (code == SA)
		return ("sa");
	if (code == SB)
		return ("sb");
	if (code == SS)
		return ("ss");
	if (code == PA)
		return ("pa");
	if (code == PB)
		return ("pb");
	if (code == RA)
		return ("ra");
	if (code == RB)
		return ("rb");
	if (code == RR)
		return ("rr");
	if (code == RRA)
		return ("rra");
	if (code == RRB)
		return ("rrb");
	return ("rrr");
}
