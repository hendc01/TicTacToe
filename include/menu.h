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
GameTypes admMenuController( sqlite3 *db );
GameTypes menuControler( userInfo user, sqlite3 *db );
#endif
