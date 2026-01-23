#include "supervisor.h"
#include "gameTypes.h"
#include "sqlite3.h"
#include "inputSafe.h"
#include "stdio.h"
#include "auth.h"
/*Controls the supervisor modes*/
void admModeController( AdmTypes admType, sqlite3 *db )
{
	userInfo user;
	switch ( admType ) 
	{
	case DELETE_ACCOUNT:
		runDeleteUser( db );
		break;
	case ADD_ACCOUNT:
		loginInput( &user );
		authRegister( db, 0, user.userName, user.userPass);
		break;
	case EXIT:
		break;
	default:
		printf( "SUPERVISOR MODE ERROR.\n" );
		break;
	}
}
/* Controller for deleting a user account 
(input, validation, and DB call) */
LoginSystem runDeleteUser( sqlite3 *db )
{
	char confirm;
	LoginSystem rs;
	userInfo user;
	printf( "Delete User\n" );
	printf( "Type the username\n" );
	stringInput( user.userName, sizeof(user.userName) );
	printf("Are you sure you want to delete: %s? (Y/N)", user.userName);
	confirm = getchar();
	if( confirm == 'Y' || confirm == 'y' ) 
	{
		printf("fwefe");
		rs = deleteUser( db, user.userName );
		if( rs == USER_NOT_FOUND )
		{
			return USER_NOT_FOUND;
		}
		return DELETE_OK;
	}
	else if( confirm == 'n' || confirm == 'N'  ) 
	{
		printf( "Input not recognized \n" );
	}
	return DELETE_ERROR;
}


/*Delete a user from the DB users*/
LoginSystem deleteUser( sqlite3 *db, const char *username  )
{
	const char *sql;
	sqlite3_stmt *stmt = NULL;
	
	sql = "DELETE FROM users WHERE username = ?;";
	int	rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{
		printf("Prepare failed: %s\n", sqlite3_errmsg( db ));
		return LOGIN_FAILED;
	}
	sqlite3_bind_text( stmt, 1, username, -1, NULL );
	rc = sqlite3_step( stmt );
	sqlite3_finalize( stmt );
	if( rc != SQLITE_DONE )
	{
		return DELETE_ERROR;
	}
	if ( sqlite3_changes( db ) == 0 )
		return USER_NOT_FOUND;
	
	return DELETE_OK;
}
