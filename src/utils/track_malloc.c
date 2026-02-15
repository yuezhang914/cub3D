/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   track_malloc.c                                        :+:      :+:    :+:*/
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 17:14:14 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/07 17:14:29 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

t_gnode	*track_add_node(t_game *game, void *ptr)
{
	t_gnode	*new_node;

	new_node = (t_gnode *)malloc(sizeof(t_gnode));
	if (!new_node)
		return (NULL);
	new_node->ptr = ptr;
	new_node->next = game->track_head;
	game->track_head = new_node;
	return (new_node);
}

void	*track_malloc(t_game *game, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
		return (NULL);
	if (track_add_node(game, ptr) == NULL)
	{
		free(ptr);
		return (NULL);
	}
	return (ptr);
}
