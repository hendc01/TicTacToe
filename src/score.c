#include "score.h"
#include "sqlite3.h"
#include <stdio.h>
#include "gameTypes.h"

/*
Controls all the function flow necessay to creat a new score row or
to update it.
*/
ScoreDB scoreControler( sqlite3 *db, roundInfo py , 
					   ScoreInfo *roundScore )
{
	ScoreDB result; 
	/*
	Hold the DB information temporary to update it in 
	playerScoreCount()
	*/
	ScoreInfo dbScore = {0};
	/*Store the logged in player primary key id into the first index
	of the ScoreInfo array (player1 and player2) to select in the DB*/
	dbScore.id = roundScore[PLAYER1].id;
	
	ScoreInfo runScore[2] = {0};
	
	result = scoreSelect( db, &dbScore );
	updateRoundInfo( py, runScore );
	
	player1ScoreUpdate( &runScore[PLAYER1], &dbScore, 
					 &roundScore[PLAYER1] );
	/*If scoreUpsert() is used without, updating the second parameter(
	scoreInfo), when updating a score it will currupt the DB data*/
	scoreUpsert( db, roundScore[PLAYER1] );
	return result;
}

/*
  Updates roundInfo for both players by adding +1

  -It requires old information to have already been populate into the
   struct for player1(logged in player), or will corrupt data in the DB

  -RoundInfo contains information of who won, draw or lost the 
   round and playes symbol X or O
   EX: winnerPy = PLAYER1.. 

  -ScoreInfo array uses player enum values as indexes:
   s[PLAYER1], s[PLAYER2]
   allowing direct update of wins/losses/draws for each player.
*/
void updateRoundInfo( roundInfo py, ScoreInfo *roundScore )
{    
	if( py.winnerCell != RESULT_NOT_WIN )
	{

		roundScore[py.winnerPy].wins++;
		roundScore[py.losserPy].losses++;
	}
	else if( py.winnerCell != RESULT_DRAW )
	{
		roundScore[PLAYER2].draws++;
		roundScore[PLAYER1].draws++;	
	}
}

/*
  Combines runScore and dbScore into a update total (win, losses, draws)

  -the result are used to update the db score for the logged in user.
  -dbScore constain the score data for logged in player(player1).
*/
void player1ScoreUpdate( const ScoreInfo *runScore, 
					  const ScoreInfo *dbScore,  ScoreInfo *roundScore )
{
	/*Score controller passed the player1 index, directly instead of
	the array*/
	roundScore->wins = runScore->wins + dbScore->wins;
	roundScore->losses = runScore->losses + dbScore->losses;
	roundScore->draws = runScore->draws + dbScore->draws;
}
/*Populate the scoreInfo struct with data of the user that has 
logged in*/
ScoreDB scoreSelect( sqlite3 *db, ScoreInfo *dbScore )
{
	int rc;
	sqlite3_stmt *stmt = NULL;
	const char *sql;
	sql =
	"SELECT wins, losses, draws FROM scores WHERE id_user = ?;";
	rc = sqlite3_prepare_v2( db, sql, -1, &stmt, NULL );
	if( rc != SQLITE_OK )
	{
		return SCORE_DB_FAILED;
	}
	sqlite3_bind_int( stmt, 1, dbScore->id  );
	
	rc = sqlite3_step( stmt );
	
	
	if( rc == SQLITE_ROW )
	{
		dbScore->wins = sqlite3_column_int( stmt, 0 );
		dbScore->losses = sqlite3_column_int( stmt, 1 );
		dbScore->draws = sqlite3_column_int( stmt, 2 );
		sqlite3_finalize( stmt );
		return SCORE_DB_OK;
	}
	sqlite3_finalize( stmt );
	return SCORE_DB_FAILED;
}
/*Updates or creat a score database row.*/
ScoreDB scoreUpsert(sqlite3 *db, const ScoreInfo s)
{
	int rc;
	sqlite3_stmt *stmt = NULL;
	
	const char *sql =
	"INSERT INTO scores(id_user, wins, losses, draws) VALUES(?,?,?,?) "
	"ON CONFLICT(id_user) DO UPDATE SET "
	"wins=excluded.wins, "
	"losses=excluded.losses, "
	"draws=excluded.draws;";
	
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		printf("Prepare failed: %s\n", sqlite3_errmsg(db));
		return SCORE_DB_FAILED;
	}
	
	sqlite3_bind_int(stmt, 1, s.id);
	sqlite3_bind_int(stmt, 2, s.wins);
	sqlite3_bind_int(stmt, 3, s.losses);
	sqlite3_bind_int(stmt, 4, s.draws);
	
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		printf("Step failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return SCORE_DB_FAILED;
	}
	sqlite3_finalize(stmt);
	return SCORE_DB_OK;
}
