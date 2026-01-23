#ifndef SUPERVISOR_H
#define SUPERVISOR_H
#include "gameTypes.h"
#include "sqlite3.h"
void admModeController( AdmTypes admType, sqlite3 *db );
LoginSystem deleteUser( sqlite3 *db, const char *username  );
LoginSystem runDeleteUser( sqlite3 *db );

#endif 
