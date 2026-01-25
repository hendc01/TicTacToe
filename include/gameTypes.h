#ifndef GAMETYPES_H
#define GAMETYPES_H

/*Define the game main cell struct*/
typedef enum
{
	LOGIN_OK,
	LOGIN_FAILED,
	LOGIN_ERROR,
	REGISTER_OK,
	REGISTER_ERROR,
	NAME_EXIST,
	DB_FAILED,  
	DELETE_ERROR,
	DELETE_OK,
	USER_NOT_FOUND,
	USER_NOT_DELETED,
}LoginSystem;

typedef enum
{
	SCORE_DB_FAILED,
	SCORE_DB_OK,
	
}ScoreDB;

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
	MOVE_OK = 0,
	MOVE_OCCUPIED = 1,
	MOVE_OUT_RANGE = 2,
} State;

typedef enum
{
	PLAYER_VS_PLAYER = 1,
	PLAYER_VS_MACHINE = 2,
	LEVEL1 = 3,
	LEVEL2 = 4,
	MENU_ERROR = 0,
}GameTypes;

typedef enum
{
	ADM_PVP = 1,
	ADM_PVE = 2,
	ADM_MODE = 3,
	DELETE_ACCOUNT = 4,
	ADD_ACCOUNT = 5,
	EXIT = 6,
}AdmTypes;

typedef enum{
	LEVEL_ERROR = 0,	
	LEVEL_OK = 1,
}LevelError;

typedef enum
{
	LOGIN = 1,
	REGISTER = 2,
}LoginOpt;

typedef enum
{
	PLAYER1_START = 0,
	PLAYER2_START = 1,
	AI_START = 3,
}Player;

typedef enum
{
	STRING_TRUNCATED = 0,
	STRING_ERROR = 1,
	STRING_OK = 2,
}SafeString;

typedef enum
{
	ADM = 0,
	PLAYER = 1,
}Role;

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

typedef struct
{
	int id;
	char userName[17];
	char userPass[17];
	Role userRole;
}userInfo;

typedef struct
{
	int id;
	int wins;
	int losses;
	int draws;
}ScoreInfo;

typedef struct
{
	Cell player1;
	Cell player2;
	Player firstPlayer;
	
}playerSymbol;

#endif

