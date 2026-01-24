#include "sql.h"
#include "sqlite3.h"
#include <stdio.h>

int authInitDB( sqlite3 **db )
{
	int rc;
	char *err = NULL;
	*db = NULL;
	
	/*openning/creatin the data base*/
	rc = sqlite3_open("users.db", db);
	if ( rc != SQLITE_OK )
	{
		printf("Error opening the DB, %s\n", sqlite3_errmsg(*db));
		sqlite3_close( *db );
		*db = NULL;
		return -1;
	}
	/*Creating a table for the database*/
	sqlite3_exec( *db, "PRAGMA foreign_keys = ON;", NULL, NULL, &err);
	const char *sqlUsers = 
	"CREATE TABLE IF NOT EXISTS users ("
	"id INTEGER PRIMARY KEY AUTOINCREMENT,"
	"role INTEGER NOT NULL,"
	"username TEXT UNIQUE NOT NULL,"
	"password TEXT NOT NULL"
	");";
	const char *sqlScores =
	"CREATE TABLE IF NOT EXISTS scores("
	"id_user INTEGER PRIMARY KEY,"
	"wins INTEGER NOT NULL DEFAULT 0,"
	"losses INTEGER NOT NULL DEFAULT 0,"
	"draws INTEGER NOT NULL DEFAULT 0,"
	"FOREIGN KEY(id_user) REFERENCES users(id) ON DELETE CASCADE"
	");";
	if (sqlite3_exec( *db, sqlUsers, NULL, NULL, &err) != SQLITE_OK)
	{
		printf("SQL error (users): %s\n", err);
		sqlite3_free(err);
		return -1;
	}
	
	if (sqlite3_exec(*db, sqlScores, NULL, NULL, &err) != SQLITE_OK)
	{
		printf("SQL error (scores): %s\n", err);
		sqlite3_free(err);
		return -1;
	}
	return 0;
}
