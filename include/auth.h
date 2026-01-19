#ifndef AUTH_H
#define AUTH_H
#include "sqlite3.h"
#include "gameTypes.h"
int loginInput( userInfo *user );
int authInitDB( sqlite3 **db );
int authRegister( sqlite3 *db, const char *username, 
				 const char *password);
int authLogin( sqlite3 *db, const char *username, const char *password );
int authRun( userInfo *user );

#endif
