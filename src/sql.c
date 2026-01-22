#include "sql.h"
#include "sqlite3.h"
#include <stdio.h>

int authInitDB( sqlite3 **db )
{
	int rc;
	*db = NULL;
	sqlite3_stmt *stmt = NULL;
	
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
	const char *sql = 
	"CREATE TABLE IF NOT EXISTS users ("
	"id INTEGER PRIMARY KEY AUTOINCREMENT,"
	"role TEXT NOT NULL,"
	"username TEXT UNIQUE NOT NULL,"
	"password TEXT NOT NULL"
	");";
	
	rc = sqlite3_prepare_v2( *db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{
		printf( "Prepare failed: %s\n", sqlite3_errmsg( *db ) );
		sqlite3_close( *db );
		*db = NULL;
		return -1;
	}
	
	rc = sqlite3_step( stmt );
	sqlite3_finalize( stmt );
	stmt = NULL;
	
	if( rc != SQLITE_DONE )
	{
		printf( "Table creation failed: %s\n", sqlite3_errmsg( *db ) );
		sqlite3_close( *db );
		*db = NULL;
		return -1;
	}
	return 0;
}
