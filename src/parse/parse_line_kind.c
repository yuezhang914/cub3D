/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_kind.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:25:49 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 19:42:18 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*skip_spaces(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

bool	is_map_line(char *line)
{
	int	i;

	i = 0;
	if (!line || !line[0])
		return (false);
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '\0')
		return (false);
	if (ft_strchr("01NSEWCPRD", line[i]))
		return (true);
	return (false);
}

static t_line_type	kind_from_word(char *first_word)
{
	if (first_word == NULL)
		return (WRONG);
	if (ft_strcmp(first_word, "NO") == 0)
		return (NORTH);
	if (ft_strcmp(first_word, "SO") == 0)
		return (SOUTH);
	if (ft_strcmp(first_word, "WE") == 0)
		return (WEST);
	if (ft_strcmp(first_word, "EA") == 0)
		return (EAST);
	if (ft_strcmp(first_word, "F") == 0)
		return (FLOOR);
	if (ft_strcmp(first_word, "C") == 0)
		return (CEILING);
	return (WRONG);
}

static t_line_type	config_kind(t_game *game, char *s)
{
	char		**words;
	t_line_type	type;

	words = ft_split(game, s, ' ');
	if (words == NULL)
		return (WRONG);
	type = kind_from_word(words[0]);
	return (type);
}

t_line_type	set_input_line_type(t_game *game, char *line)
{
	char		*s;
	t_line_type	type;

	if (line == NULL)
		return (WRONG);
	s = skip_spaces(line);
	if (*s == '\0')
		return (EMPTY);
	type = config_kind(game, s);
	if (type != WRONG)
		return (type);
	if (is_map_line(s))
		return (MAP);
	return (WRONG);
}
