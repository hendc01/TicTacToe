#include "gui.h"
#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "gameTypes.h"
#include "innit.h"
#include "game.h"
#include "levels.h"
#include <string.h>
#include <sodium.h>
#include "score.h"

Vector2 initBoard( board *grid );
void initWindow();
position getClickedCell(Vector2 gridOrigin);
void displayBoard( Vector2 gridOrigin, board grid );
void updateBoard( board *grid, Vector2 gridOrigin, Vector2 selectedCellXY, 
		 roundInfo rf);
void displayResult( roundInfo py );
int runPvP( Vector2 gridOrigin, roundInfo *py, board *grid );
int runPvE( Vector2 gridOrigin, roundInfo *py, board *grid, position *pveM );
Cell displaySymbolDec ( Vector2 gridOrigin );
Cell decideSymbol1( roundInfo *py, Vector2 gridOrigin );
position grid2(Vector2 gridOrigin, char msg1[15], char msg2[15],
	       char tittle[15], int xOring, int yOring);
position pveMenuGUI( Vector2 gridOrigin );
position getClickedMenuCell(Vector2 gridOrigin, int max);
Vector2 getMenuOrigin(int columns, int rows);
position gridInputDraw( Vector2 gridOrigin, char msg[] );
int ipInputText( Vector2 gridOrigin, char ipPort[21], 	int *letterCount);
struct sockaddr_in createSocketInfoGUI(  int ip, int *letterCount, 
				       Vector2 gridOrigin );
int popupMessage(const char *msg);
GameTypes convertLevel( position pt  );
int displayScoreGUI( ScoreInfo *sf );

//Used for spacing the grid on the window
Vector2 getMenuOrigin(int columns, int rows)
{
	int menuWidth = columns * MENU_SIZE;
	int menuHeight = rows * MENU_SIZE;
	
	Vector2 origin = {
		(GetScreenWidth() - menuWidth) / 2,
		(GetScreenHeight() - menuHeight) / 2
	};
	
	return origin;
}

//Used for board initialization and to set up start location
Vector2 initBoard(board *grid)
{
	Vector2 gridOrigin;
	
	for (int r = 0; r < BOARD_SIZE; r++)
	{
		for (int c = 0; c < BOARD_SIZE; c++)
		{
			grid->boardGrid[r][c] = CELL_EMPTY;
		}
	}
	
	int gridWidth = BOARD_SIZE * CELL_SIZE;
	int gridHeight = BOARD_SIZE * CELL_SIZE;
	
	gridOrigin = (Vector2){
		(GetScreenWidth() - gridWidth) / 2,
		(GetScreenHeight() - gridHeight) / 2
	};
	
	return gridOrigin;
}

// Set up the window size and fps
void initWindow(){
	const int screenWidth = 800;
	const int screenHeight = 450;
	
	InitWindow(screenWidth, screenHeight, "TicTacToe");
	SetTargetFPS(60);
	
}

//Used to get the cell clicked for menu gridSize
position getClickedMenuCell(Vector2 gridOrigin, int max)
{
	position ps;
	ps.row = -1;
	ps.collum = -1;
	ps.error = LEVEL_OK;
	
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		Vector2 mouse = GetMousePosition();
		
		int col = (mouse.x - gridOrigin.x) / MENU_SIZE;
		int row = (mouse.y - gridOrigin.y) / MENU_SIZE;
		
		if (col >= 0 && col < max && row == 0)
		{
			ps.row = row;
			ps.collum = col;
		}
	}
	
	return ps;
}

//Get mouse location if left button is clicked, necessary to give the 
// x and y start point "Orgin"
position getClickedCell(Vector2 gridOrigin)
{
	position ps;
	
	ps.row = -1;
	ps.collum = -1;
	ps.error = LEVEL_OK;
	
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		Vector2 mouse = GetMousePosition();	
		int col = (mouse.x - gridOrigin.x) / CELL_SIZE;
		int row = (mouse.y - gridOrigin.y) / CELL_SIZE;
		
		if (col >= 0 && col < BOARD_SIZE &&
		    row >= 0 && row < BOARD_SIZE)
		{
			ps.row = row;
			ps.collum = col;
		}
	}
	
	return ps;
}

//Display GUI score
int displayScoreGUI( ScoreInfo *sf ){

	DrawText( "          WIN    LOSSES   DRAWNS", 220, 60, 20, BLACK );
	for( int i = 0; i < 4; i++ ){
		char msg[35];
		snprintf(msg, sizeof(msg), "Level %d: %d       %d       %d", 
			 sf[i].level -2, sf[i].wins, sf[i].losses, sf[i].draws);
		DrawText( msg, 175, 80 * i + 100, 30, BLACK );
	}
	DrawText("Press ENTER to leave", 260, 400, 25, BLACK);
	
	if (IsKeyPressed(KEY_ENTER))
	{
		return 1;
	}
	return 0;
}

//Drawn the box for input boxes(login) and return positon clicked)
position gridInputDraw( Vector2 gridOrigin, char msg[] ){
	position ps;
	ps.row = -1;
	ps.collum = -1;
	ps.error = LEVEL_OK;
	
	Rectangle rect = {
	gridOrigin.x,
	gridOrigin.y,
	375,
	50
	};
	DrawText( msg, 175, 80, 30, BLACK);
	DrawRectangleLinesEx(rect, 1, DARKGRAY);
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		ps = getClickedMenuCell(gridOrigin, 2);
	}
	
	return ps;
	
}
//Creates two squared boxes and return position clicked
position grid2(Vector2 gridOrigin, char msg1[15], char msg2[15],
		  char tittle[15], int xOring, int yOring)
{
	position ps;
	ps.row = -1;
	ps.collum = -1;
	ps.error = LEVEL_OK;
	
	int y = 0;
	
	gridOrigin.x += xOring;
	gridOrigin.y += yOring;
	
	for (int x = 0; x < 2; x++)
	{
		Rectangle rect = {
			gridOrigin.x + (x * MENU_SIZE ),
			gridOrigin.y + (y * MENU_SIZE),
			MENU_SIZE,
			MENU_SIZE
		};
		
		DrawText(tittle, 325, 80, 25, BLACK);
		DrawRectangleLinesEx(rect, 1, DARKGRAY);
		
		if (x == 0)
		{
			DrawTextEx(GetFontDefault(), msg1,
				   (Vector2){ rect.x + 15, rect.y + 36 },
				   25, 1, BLACK);
		}
		else if (x == 1)
		{
			DrawTextEx(GetFontDefault(), msg2,
				   (Vector2){ rect.x + 15, rect.y + 36 },
				   25, 1, BLACK);
		}
	}
	
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		ps = getClickedMenuCell(gridOrigin, 2);
	}
	
	return ps;
}

//Create the PVE options boxes and return the box clicked
position pveMenuGUI( Vector2 gridOrigin ){
	position ps;
	ps.collum = -1;
	int y = 0;
	
	for( int x = 0; x < 5; x++ ){
		Rectangle rect = {
			gridOrigin.x + ( x * MENU_SIZE ),
			gridOrigin.y + ( y * MENU_SIZE ),
			MENU_SIZE,
			MENU_SIZE
		};
		DrawText("Game Mode", 300, 80, 25, BLACK);
		DrawRectangleLinesEx( rect, 1, DARKGRAY );
		if( x == 0 ){
			DrawTextEx(GetFontDefault(), "LEVEL 1",
				   (Vector2){ rect.x + 10, rect.y + 40 },
				   18, 2, BLACK);		
		}
		else if( x== 1 ){
			DrawTextEx( GetFontDefault(), "LEVEL 2",
				   (Vector2){ rect.x + 10, rect.y + 40 },
				   18, 2, BLACK);		
		}
		else if( x== 2 ){
			DrawTextEx( GetFontDefault(), "LEVEL 3",
				   (Vector2){ rect.x + 10, rect.y + 40 },
				   18, 2, BLACK);		
		}
		else if( x== 3 ){
			DrawTextEx( GetFontDefault(), "LEVEL 4",
				   (Vector2){ rect.x + 10, rect.y + 40 },
				   18, 2, BLACK);	
		}
		else if( x== 4 ){
			DrawTextEx( GetFontDefault(), "DISPLAY",
				   (Vector2){ rect.x + 10, rect.y + 40 },
				   18, 2, BLACK);
			
		}
		
	};
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
		ps = getClickedMenuCell( gridOrigin, 5 );
		return ps;
		
	}
	return ps;	
}

//Create Menu for PVP and PVE
position createSquare( Vector2 gridOrigin, int yRange, int xRange,
		      char title[50]){
	for( int y = 0; y < yRange; yRange++ ){
		for( int x = 0; x < xRange; xRange++ ){
			Rectangle rect = {
				gridOrigin.x + ( x * CELL_SIZE ),
				gridOrigin.y + ( y * CELL_SIZE ),
				CELL_SIZE,
				CELL_SIZE
			};
			DrawText(title, 300, 80, 25, BLACK);
			DrawRectangleLinesEx( rect, 1, DARKGRAY );
			if( x == 1 ){
				DrawTextEx(GetFontDefault(), "PVP",
					   (Vector2){ rect.x + 15, rect.y + 25 },
					   25, 1, BLACK);	
			}
			else{
				DrawTextEx( GetFontDefault(), "PVE",
					   (Vector2){ rect.x + 15, rect.y + 25 },
					   25, 1, BLACK);	
			}
			
		};	
	}
	
}
//Display SymbolDec Box and getINput
Cell displaySymbolDec ( Vector2 gridOrigin ){
	position ps;
	ps.collum = 3;
	Cell symbol = CELL_EMPTY;
	int y = 0;
	
	for( int x = 0; x < 2; x++ ){
		Rectangle rect = {
			gridOrigin.x + ( x * CELL_SIZE ),
			gridOrigin.y + ( y * CELL_SIZE ),
			CELL_SIZE,
			CELL_SIZE
		};
		DrawText("Decide The Symbol Player 1", 210, 80, 25, BLACK);
		DrawRectangleLinesEx( rect, 1, DARKGRAY );
		if( x == 1 ){
			DrawTextEx(GetFontDefault(), "X",
				   (Vector2){ rect.x + 20, rect.y + 12 },
				   40, 1, BLACK);	
		}
		else{
			DrawTextEx( GetFontDefault(), "O",
				   (Vector2){ rect.x + 20, rect.y + 12 },
				   40, 1, BLACK);	
		}
		
	};
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
		ps = getClickedCell( gridOrigin );
		if( ps.collum == 3 ){
			symbol = CELL_EMPTY;
		}
		else if( ps.collum  == 1 ){
			symbol = CELL_X;
			
		}
		else if( ps.collum == 0 ){
			symbol = CELL_O;
		}
		
	}
	
	return symbol;	
}
//Initialize the roundInfo based on the grid chosen in displaySymbolDec
Cell decideSymbol1( roundInfo *p, Vector2 gridOrigin )
{
	Cell pT;
	pT = displaySymbolDec( gridOrigin );
	if( pT == CELL_X )
	{
		p->playerTurn = PLAYER1;
		p->player1 = CELL_X;
		p->player2 = CELL_O;
		return 1;
	}
	else if( pT == CELL_O  )
	{
		p->playerTurn = PLAYER2;
		p->player2 = CELL_X;
		p->player1 = CELL_O;
		return 1;
	}
	return 0;
}

//Display board with symbols
void displayBoard( Vector2 gridOrigin, board grid ){
	for (int r = 0; r < BOARD_SIZE; r++)
	{
		for (int c = 0; c < BOARD_SIZE; c++)
		{
			Rectangle rect = {
				gridOrigin.x + (c * CELL_SIZE),
				gridOrigin.y + (r * CELL_SIZE),
				CELL_SIZE,
				CELL_SIZE
			};
			DrawRectangleLinesEx(rect, 1, DARKGRAY);
			if (grid.boardGrid[r][c] == CELL_X)
			{
				DrawTextEx(GetFontDefault(), "X",
					   (Vector2){ rect.x + 20, rect.y + 12 },
					   40, 1, BLACK);
			}
			else if (grid.boardGrid[r][c] == CELL_O)
			{
				DrawTextEx(GetFontDefault(), "O",
					   (Vector2){ rect.x + 20, rect.y + 12 },
					   40, 1, BLACK);
			}
			
		}
	}
}


//Get the input from the keyboard based on the grid displayed
int InputText( Vector2 gridOrigin, char ipPort[12], int *letterCount ) {
	int mouseOnText = 0;
	Rectangle rect = {
	gridOrigin.x,
	gridOrigin.y,
	250,
	50		
	};
	if(CheckCollisionPointRec( GetMousePosition(), rect )){
		mouseOnText = 1;
	}
	else{
		mouseOnText = -1;
	}
	
	if( mouseOnText ){
		SetMouseCursor(MOUSE_CURSOR_IBEAM);
		int key = GetCharPressed();
		
		while( key > 0 ){
			if((key >=32) && (key <= 125) 
			   &&(*letterCount <= 12 -1 )){
				ipPort[*letterCount] = (char)key;
				(*letterCount)++;
				ipPort[*letterCount] = '\0';
			}
			key = GetCharPressed();
		}
		if( IsKeyPressed( KEY_BACKSPACE ) && *letterCount > 0 )
		{
			(*letterCount)--;
			ipPort[*letterCount] = '\0';
		}
		if(IsKeyPressed(KEY_ENTER)){
			return 1;
		}
	}
	else{
		SetMouseCursor( MOUSE_CURSOR_DEFAULT);
	}
	return 0;
}

//Controls the Main Loop of the Whole Gui and Game 
void guiControler(board *grid, ScoreInfo rf[], sqlite3 *db, roundInfo *py)
{
	Vector2 gridOrigin;
	Vector2 pveMenuOrigin;
	
	*(py) = roundInit();
	
	int symbolDecided = 0;
	
	position gameModeDecided;
	gameModeDecided.collum = -1;
	
	int pvpModeDecided = -1;
	
	position pveM;
	pveM.collum = -1;
	
	position c;
	c.collum = -1;
	
	Cell symbol = CELL_EMPTY;
	
	int result = 0;
	int scoreUpdated = 0;
	
	int userCounter = 0;
	int passCounter = 0;
	
	Vector2 textGridO;
	
	int login = 0;
	
	userInfo u;
	strcpy(u.userName, "");
	strcpy(u.userPass, "");
	
	int user = 0;
	int pass = 0;
	
	position loginchoice;
	loginchoice.collum = -1;
	
	int showPopUp = 0;
	char popUpMsg[100] = "";
	
	int loginFailed = 0;
	int close = 0;
	
	initWindow();
	
	gridOrigin = initBoard(grid);
	
	textGridO = gridOrigin;
	textGridO.x -= 100;
	
	// You have 5 PVE boxes now: Level 1-4 + DISPLAY
	pveMenuOrigin = getMenuOrigin(5, 1);
	
	while (!WindowShouldClose())
	{
		BeginDrawing();
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		ClearBackground(WHITE);
		
		// Popup message screen
		if (showPopUp)
		{
			if (popupMessage(popUpMsg))
			{
				showPopUp = 0;
				
				if (close == 1)
				{
					CloseWindow();
					return;
				}
			}
			
			EndDrawing();
			continue;
		}
		
		// LOGIN BLOCK
		if (login == 0)
		{
			if (loginchoice.collum == -1)
			{
				loginchoice = grid2(gridOrigin, "Sign In",
						    "Sign Up", "", 0, 0);
				
				loginFailed = 0;
				
				EndDrawing();
				continue;
			}
			
			// USERNAME INPUT
			if (user == 0)
			{
				gridInputDraw(textGridO, "Username (PRESS ENTER)");
				
				user = InputText(textGridO, u.userName, &userCounter);
				
				DrawText(u.userName,
					 textGridO.x + 10,
					 textGridO.y,
					 30,
					 BLACK);
				
				EndDrawing();
				continue;
			}
			
			// PASSWORD INPUT
			if (pass == 0 && user == 1)
			{
				gridInputDraw(textGridO, "Password (PRESS ENTER)");
				
				pass = InputText(textGridO, u.userPass, &passCounter);
				
				DrawText(u.userPass,
					 textGridO.x + 10,
					 textGridO.y,
					 30,
					 BLACK);
				
				EndDrawing();
				continue;
			}
			
			LoginSystem authResult;
			
			// SIGN IN
			if (loginchoice.collum == 0)
			{
				authResult = authRunGui(&u, db, rf, LOGIN);
				
				if (authResult == LOGIN_OK)
				{
					strcpy(popUpMsg, "Login successful");
					showPopUp = 1;
					login = 1;
				}
				else
				{
					strcpy(popUpMsg, "Login Failed");
					showPopUp = 1;
					
					login = 0;
					user = 0;
					pass = 0;
					
					strcpy(u.userName, "");
					strcpy(u.userPass, "");
					
					userCounter = 0;
					passCounter = 0;
					
					loginchoice.collum = 0;
					loginFailed++;
					
					if (loginFailed == 4)
					{
						loginchoice.collum = -1;
						strcpy(popUpMsg, "Too many attempts");
						showPopUp = 1;
					}
				}
				
				EndDrawing();
				continue;
			}
			
			// SIGN UP
			if (loginchoice.collum == 1)
			{
				char hashed_password[crypto_pwhash_STRBYTES];
				
				crypto_pwhash_str(hashed_password,
						  u.userPass,
						  strlen(u.userPass),
						  crypto_pwhash_OPSLIMIT_INTERACTIVE,
						  crypto_pwhash_MEMLIMIT_INTERACTIVE);
				
				strcpy(u.userPass, hashed_password);
				
				authResult = authRunGui(&u, db, rf, REGISTER);
				
				if (authResult == REGISTER_OK)
				{
					strcpy(popUpMsg, "Account Created");
					showPopUp = 1;
					
					// After registering, send user to sign in
					loginchoice.collum = 0;
				}
				else
				{
					strcpy(popUpMsg, "Username unavailable");
					showPopUp = 1;
					
					// Stay in register path
					loginchoice.collum = 1;
				}
				
				user = 0;
				pass = 0;
				
				strcpy(u.userName, "");
				strcpy(u.userPass, "");
				
				userCounter = 0;
				passCounter = 0;
				
				EndDrawing();
				continue;
			}
		}
		
		// SCOREBOARD DISPLAY BLOCK
		if (pveM.collum == 4)
		{
			retrieveScoreInfo(db, rf);
			
			if (displayScoreGUI(rf))
			{
				pveM.collum = -1;
				gameModeDecided.collum = -1;
			}
			
			EndDrawing();
			continue;
		}
		
		// MAIN MENU BLOCK
		if (gameModeDecided.collum == -1)
		{
			gameModeDecided = grid2(gridOrigin, "PVE",
						"PVP", "GAME MODE", 0, 0);
			
			EndDrawing();
			continue;
		}
		
		// PVE MENU BLOCK
		if (gameModeDecided.collum == 0 && pveM.collum == -1)
		{
			pveM = pveMenuGUI(pveMenuOrigin);
			
			EndDrawing();
			continue;
		}
		
		// TERMINAL STATE MENU: MENU / AGAIN
		if (result == 1)
		{
			position endChoice;
			endChoice.collum = -1;
			
			endChoice = grid2(gridOrigin, "MENU",
					  "AGAIN", "", 20, 240);
			
			displayBoard(gridOrigin, *grid);
			displayResult(*py);
			
			// Back to main menu
			if (endChoice.collum == 0)
			{
				initBoard(grid);
				*(py) = roundInit();
				
				symbol = CELL_EMPTY;
				symbolDecided = 0;
				
				pveM.collum = -1;
				gameModeDecided.collum = -1;
				pvpModeDecided = -1;
				c.collum = -1;
				
				result = 0;
				scoreUpdated = 0;
			}
			
			// Play again
			if (endChoice.collum == 1)
			{
				initBoard(grid);
				*(py) = roundInit();
				
				symbol = CELL_EMPTY;
				symbolDecided = 0;
				
				/*
				This returns to the PVE level menu if it was PVE.
				For PVP, it resets the PVP choice too.
				*/
				pveM.collum = -1;
				pvpModeDecided = -1;
				c.collum = -1;
				
				result = 0;
				scoreUpdated = 0;
			}
			
			EndDrawing();
			continue;
		}
		
		// PVP MENU / PVP GAME BLOCK
		if (gameModeDecided.collum == 1)
		{
			if (pvpModeDecided == -1)
			{
				c = grid2(gridOrigin, "Lan",
					  "Local", "PVP", 0, 0);
				
				if (c.collum != -1)
				{
					pvpModeDecided = 1;
				}
				
				EndDrawing();
				continue;
			}
			
			// LAN selected
			if (c.collum == 0 && c.collum != -1)
			{
				strcpy(popUpMsg, "LAN only in console");
				showPopUp = 1;
				close = 1;
				
				EndDrawing();
				continue;
			}
			
			// Local PVP selected
			else if (c.collum == 1 && c.collum != -1)
			{
				result = runPvP(gridOrigin, py, grid);
				
				if (result == 1)
				{
					c.collum = -1;
					pvpModeDecided = -1;
				}
			}
		}
		else
		{
			// Run PVE mode
			result = runPvE(gridOrigin, py, grid, &pveM);
			
			if (result && !scoreUpdated)
			{
				py->level = convertLevel(pveM);
				scoreControler(db, *py, rf);
				scoreUpdated = 1;
			}
			
		}
		
		// SYMBOL DECIDER FOR PLAYER 1
		if ((symbolDecided == 0 &&
		     gameModeDecided.collum != -1 &&
		     pveM.collum != -1 &&
		     pveM.collum != 4)
		     ||
		     (symbolDecided == 0 &&
		      gameModeDecided.collum != -1 &&
		      c.collum == 1))
		{
			symbol = decideSymbol1(py, gridOrigin);
			
			if (symbol != CELL_EMPTY)
			{
				symbolDecided = 1;
				EndDrawing();
				continue;
			}
			
			EndDrawing();
			continue;
		}
		
		displayBoard(gridOrigin, *grid);
		displayResult(*py);
		
		EndDrawing();
	}
	
	CloseWindow();
}
//Run PVP for GUI
int runPvP( Vector2 gridOrigin, roundInfo *py, board *grid ){
	int turn = 0;
	position ps = getClickedCell(gridOrigin);
	
	if ( ps.row != -1 && py->winnerCell == RESULT_NOT_WIN )
	{
		gameModeControlerGUI( grid, ps, py, &turn );
		
	}
	if (py->winnerCell != RESULT_NOT_WIN)
	{
		return 1;
	}
	else{
		return 0;
	}

}

int runPvE( Vector2 gridOrigin, roundInfo *py, board *grid, position *pveM ){
	position ps;
	ps.row = -1;
	ps.collum = -1;
	ps.error = LEVEL_OK;
	
	if( py->winnerCell == RESULT_NOT_WIN ){
		py->winnerCell = result(grid, py->turn);
		if ( py->winnerCell != RESULT_NOT_WIN ) {
			return 1;
		}
		
		switch ( py->playerTurn )
		{
		case PLAYER1:
			ps = getClickedCell( gridOrigin );
			break;
		case PLAYER2:
			/*Player2 always the AI or Player Who hasnt log in their 
			account (Guest), LevelControler calls the difficult 
			availables in PVE*/
			if( pveM->collum == 0 ){
				ps = levelControler( grid, 3, *py );		
			}
			else if( pveM->collum == 1 ){
				ps = levelControler( grid, 4, *py );		
			}
			else if( pveM->collum == 2 ){
				ps = levelControler( grid, 5, *py );		
			}
			else if( pveM->collum == 3 ){
				ps = levelControler( grid, 6, *py );		
			}
			break;
		default:
			break;
		}
		if (ps.row != -1 && ps.collum != -1)
		{
			gameModeControlerGUI(grid, ps, py, &py->turn);
			if (py->winnerCell != RESULT_NOT_WIN)
			{
				return 1;
			}
		}
	}
	return 0;
}

void displayResult( roundInfo py ){
	if( py.winnerCell == RESULT_NOT_WIN ){
		;
	}
	else if (py.winnerCell == RESULT_X_WINS)
		DrawText("X wins!", 340, 80, 30, BLACK);
	else if (py.winnerCell == RESULT_O_WINS)
		DrawText("O wins!", 340, 80, 30, BLACK);
	else if (py.winnerCell == RESULT_DRAW)
		DrawText("Draw!", 360, 80, 30, BLACK);
}

int popupMessage(const char *msg)
{
	Rectangle rect = {
	180,
	160,
	440,
	120
	};
	
	DrawRectangleRec(rect, WHITE);
	DrawRectangleLinesEx(rect, 2, BLACK);
	
	DrawText(msg, rect.x + 25, rect.y + 30, 22, BLACK);
	DrawText("Press ENTER to continue", rect.x + 80, rect.y + 75, 20, BLACK);
	
	if (IsKeyPressed(KEY_ENTER))
	{
		return 1;
	}
	
	return 0;
}

GameTypes convertLevel( position pt  ){
	if( pt.collum == 0 ){
		return LEVEL1;	
	}
	else if( pt.collum == 1 ){
		return LEVEL2;
	}
	else if( pt.collum == 2 ){
		return LEVEL3;
	}
	else if( pt.collum == 3 ){
		return LEVEL4;
	}
}
//Lan GUI MOde socket functions


