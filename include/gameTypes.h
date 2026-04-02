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
	LEVEL4 = 6,
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
	LAN = 1,
	LOCAL = 2,
	HOST_GAME = 3,
	ENTER_GAME = 4,
	
}PvPModes;

typedef enum{
	LV1_NO_CELL= 0,	
	LEVEL_OK = 1,
	LEVEL_ERROR = 2,
	ISTHERE_WIN_ERROR = 3,
	
}LevelError;

typedef enum
{
	LOGIN_MENU = -1,
	LOGIN = 1,
	REGISTER = 2,
}LoginOpt;

typedef enum
{
	PLAYER1 = 0,
	PLAYER2 = 1,
	AI = 3,
	BLANK = -1,
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

typedef enum
{
	HW = 0,
	VT = 1,
	D1 = 2,
	D2 = 3,
}WinPt;

typedef enum{
	SERVER = 0,
	CLIENT = 1,
}server;

typedef enum{
	MIN_PLAYER = 0,
	MAX_PLAYER = 1,
}Minimax;

typedef struct
{
	int row;
	int collum;
	Player currentPlayer;
	GameResult isThereWin;
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
	Cell turnCell;
	Player playerTurn;
	Player winnerPy;
	Player losserPy;
	GameResult winnerCell;
	int turn;
}roundInfo;

typedef struct
{
	int win;
	Cell simbol;
}winInfo;

typedef struct{
	char ipAdress[16];
	char portAdress[6];
}serverAdress ;

#endif

