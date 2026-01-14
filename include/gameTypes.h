#ifndef GAMETYPES_H
#define GAMETYPES_H

/*Define the game main cell struct*/
typedef enum 
{
	CELL_EMPTY = 0,
	CELL_O = 1,
	CELL_X = 2,
	
} Cell;

typedef enum {
	RESULT_NOT_WIN = 0,
	RESULT_X_WINS = 1,
	RESULT_O_WINS = 2,
	RESULT_DRAW = 3,
	RESULT_ERROR = 4,
	
} GameResult;

typedef enum
{
	MOVE_OK = 1,
	MOVE_OCCUPIED = 0,
} State;

typedef enum
{
	PLAYER_VS_PLAYER = 1,
	PLAYER_VS_MACHINE = 2,
	LEVEL1 = 3,
	LEVEL2 = 4,
	MENU_ERROR = 0,
}GameTypes;

typedef enum{
	LEVEL1_ERROR = 0,	
	LEVEL_OK = 1,
}LevelError;

typedef struct
{
	int row;
	int collum;
	LevelError error;
	
}position;

/*Define the how the game store data*/
typedef struct 
{
	Cell boardGrid[3][3];	
} board;

#endif

