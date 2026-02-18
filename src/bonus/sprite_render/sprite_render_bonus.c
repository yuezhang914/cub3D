#include "cub3d.h"

void    collect_sprites(t_game *game)
{
    int i;
    int j;
    int count;

    count = 0;
    // 1. 第一次遍历：数数
    i = -1;
    while (game->map[++i])
    {
        j = -1;
        while (game->map[i][++j])
            if (game->map[i][j] == 'S')
                count++;
    }
    game->sprs.num = count;
    game->sprs.list = malloc(sizeof(t_sprite) * count); // 建议使用你的 track_malloc
    
    // 2. 第二次遍历：记录坐标并从地图抹除
    count = 0;
    i = -1;
    while (game->map[++i])
    {
        j = -1;
        while (game->map[i][++j])
        {
            if (game->map[i][j] == 'S')
            {
                game->sprs.list[count].x = j + 0.5f;
                game->sprs.list[count].y = i + 0.5f;
                game->map[i][j] = '0'; // 变回空地，让射线穿过
                count++;
            }
        }
    }
}