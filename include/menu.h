#ifndef MENU_H
#define MENU_H 
#include "gameTypes.h"
GameTypes mainMenu( void );
GameTypes playerMachine( void );
LoginOpt loginMenu( void );
int runLogin( userInfo *user );
#endif
