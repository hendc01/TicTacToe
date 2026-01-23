#ifndef MENU_H
#define MENU_H 
#include "gameTypes.h"
#include "sqlite3.h"
AdmTypes admMenu( void );
GameTypes mainMenu( void );
GameTypes pveMenu( void );
LoginOpt loginMenu( void );
AdmTypes admMode( void );
int runLogin( userInfo *user );
void admModeController( AdmTypes admType, sqlite3 *db );
GameTypes admMenuController( sqlite3 *db );
GameTypes menuControler( userInfo user, sqlite3 *db );
#endif
