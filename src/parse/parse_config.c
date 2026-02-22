/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhang2 <yzhang2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:26:08 by yzhang2           #+#    #+#             */
/*   Updated: 2026/02/22 20:30:54 by yzhang2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	is_config_complete(t_game *game)
{
	if (game->north.path && game->south.path && game->west.path
		&& game->east.path && game->floor_color != -1
		&& game->ceiling_color != -1)
		return (true);
	return (false);
}

static void	set_texture_path(t_game *game, t_line_type type, char *line)
{
	char	**words;
	char	**target;
	size_t	len;

	if (type == NORTH)
		target = &game->north.path;
	else if (type == SOUTH)
		target = &game->south.path;
	else if (type == WEST)
		target = &game->west.path;
	else
		target = &game->east.path;
	if (*target != NULL)
		graceful_exit(game, 1, __func__, "Redefinition of texture path.");
	words = ft_split(game, line, ' ');
	if (words[1] == NULL || words[2] != NULL)
		graceful_exit(game, 1, __func__, "Texture line arg number error.");
	len = ft_strlen(words[1]);
	if (len < 5 || ft_strcmp(".xpm", words[1] + len - 4) != 0)
		graceful_exit(game, 1, __func__, "Texture not ending with .xpm.");
	*target = words[1];
}

static bool	read_rgb_value(char *s, int *i, int *out)
{
	int	val;

	val = 0;
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	if (!is_dig(s[*i]))
		return (true);
	while (is_dig(s[*i]))
	{
		val = val * 10 + (s[*i] - '0');
		if (val > 255)
			return (true);
		(*i)++;
	}
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	*out = val;
	return (false);
}

static void	set_color_config(t_game *game, t_line_type type, char *line)
{
	int	*i_target;
	int	i;
	int	r;
	int	g;
	int	b;

	i_target = &game->ceiling_color;
	if (type == FLOOR)
		i_target = &game->floor_color;
	if (*i_target != -1)
		graceful_exit(game, 1, __func__, "Redefinition of color config.");
	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	i++;
	if (read_rgb_value(line, &i, &r) || line[i++] != ',' || read_rgb_value(line,
			&i, &g) || line[i++] != ',' || read_rgb_value(line, &i, &b))
		graceful_exit(game, 1, __func__, "Bad color format.");
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] != '\0')
		graceful_exit(game, 1, __func__, "Bad color format.");
	*i_target = (r << 16) | (g << 8) | b;
}

void	parse_config(t_game *game)
{
	int			i;
	t_line_type	type;
	char		**lines;

	lines = game->cubfile_lines;
	i = 0;
	while (lines[i])
	{
		type = set_input_line_type(game, lines[i]);
		if (type == EMPTY)
			;
		else if (type == FLOOR || type == CEILING)
			set_color_config(game, type, lines[i]);
		else if (type == NORTH || type == SOUTH || type == WEST || type == EAST)
			set_texture_path(game, type, lines[i]);
		else if (type == MAP && !is_config_complete(game))
			graceful_exit(game, 1, __func__, "Map before config complete.");
		else if (type == MAP)
			return ;
		else
			graceful_exit(game, 1, __func__, "Invalid line in config.");
		i++;
	}
	graceful_exit(game, 1, __func__, "No map found.");
}
