#include "score.h"
#include "sqlite3.h"
#include <stdio.h>
#include "gameTypes.h"

/*
ScoreDB runScore( sqlite3 *db, userInfo user, ScoreInfo s )
{
	int currentPlayer = user.id;
	s.id = currentPlayer;
	scoreAdd( db, s );
}

void scoreCount( GameResult winner, ScoreInfo *s )
{
	if( winner == RESULT_X_WINS )
	{
		s[PLAYER1].wins++;
		s[PLAYER2].losses++;
	}
	else if( winner == RESULT_O_WINS )
	{
		s[PLAYER2].wins++;
		s[PLAYER1].losses++;
	}
	s[PLAYER2].draws++;
	s[PLAYER1].draws++;
}
*/
ScoreDB scoreAdd( sqlite3 *db, ScoreInfo s )
{
	int rc;
	sqlite3_stmt *stmt = NULL;
	const char *sql;
	sql = 
	"INSERT INTO scores(id_user, wins, losses, draws ) VALUES(?,?,?,?);";
	rc =  sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{	
		return SCORE_DB_FAILED;	
	}
	sqlite3_bind_int( stmt, 1, s.id );
	sqlite3_bind_int( stmt, 2, s.wins );
	sqlite3_bind_int( stmt, 2, s.losses );
	sqlite3_bind_int( stmt, 2, s.draws );
	
	rc = sqlite3_step( stmt );
	sqlite3_finalize( stmt );
	stmt = NULL;
	
	if( rc != SQLITE_DONE )
	{
		return SCORE_DB_FAILED;
	}
	return SCORE_DB_OK;
}
