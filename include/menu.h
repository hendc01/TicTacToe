#ifndef MENU_H
#define MENU_H 
#include "gameTypes.h"
GameTypes mainMenu( void );
GameTypes pveMenu( void );
LoginOpt loginMenu( void );
int runLogin( userInfo *user );
#endif
