#pragma once

#define WIN_WIDTH 480
#define WIN_HEIGHT 640
#define CAM_WIDTH 7.5
#define CAM_HEIGHT 10.0

#define PLAYER_OFFSET 2.0f
#define FLOOR_HEIGHT 1.25f

enum class GameState
{
	Play = 0, MainMenu = 1, GameOver = 2
};