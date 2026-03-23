#ifndef GAME_H
#define GAME_H
#include "gameTypes.h"

roundInfo game ( board *grid, GameTypes gameChoices );
Cell isCellEmpty( const board *grid, int r, int c );
void menu1 ( void );
void gamePVPControler( board *grid, roundInfo *rInfo	);
State gridAlloc( board *grid, int row, int collum, Cell currentPlayer );

position gameInput();
GameResult result( const board *grid, int turn );
void gamePvEControler( board *grid, GameTypes gameMode, 
						   roundInfo *rInfo );
State doMove(board *grid, position ps, int *turn, Cell symbol);

void playerSwitch( Player *playerTurn );
Cell currentPlayerCell( roundInfo r );
roundInfo gameModeControler( board *grid, position ps, roundInfo *py, 
							int *turn);
#endif
