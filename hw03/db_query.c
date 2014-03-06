#include <stdio.h>
#include "mongo.h"

// Takes in the mongo connection, and the name of a movie
// Prints out all of the actors who were in that movie
// Obviously, heavily based on tutorial.c
static void get_actors_from_movie( mongo *conn, char *movieQuery ) {
  bson query[1];
  mongo_cursor cursor[1];

  bson_init( query );
  bson_append_string( query, "movies", movieQuery );
  bson_finish( query );

  mongo_cursor_init( cursor, conn, "db.actors_collection" );
  mongo_cursor_set_query( cursor, query );

  printf( "\nMOVIE BEING QUERIED: %s\n", movieQuery);
  while( mongo_cursor_next( cursor ) == MONGO_OK ) {
    bson_iterator iterator[1];
    if ( bson_find( iterator, mongo_cursor_bson( cursor ), "name" )) {
        printf( "name: %s\n", bson_iterator_string( iterator ) );
    }
  }
}

// Takes in the mongo connection, and the name of an actor
// Prints out all of the movies that actor was in
// Obviously, heavily based on tutorial.c
static void get_movies_from_actor( mongo *conn, char *actorQuery ) {
  bson query[1];
  mongo_cursor cursor[1];

  bson_init( query );
  bson_append_string( query, "actors", actorQuery );
  bson_finish( query );

  mongo_cursor_init( cursor, conn, "db.movies_collection" );
  mongo_cursor_set_query( cursor, query );

  printf( "\nACTOR BEING QUERIED: %s\n", actorQuery);
  while( mongo_cursor_next( cursor ) == MONGO_OK ) {
    bson_iterator iterator[1];
    if ( bson_find( iterator, mongo_cursor_bson( cursor ), "name" )) {
        printf( "name: %s\n", bson_iterator_string( iterator ) );
    }
  }
}

int main() {

  // open mongo
  mongo conn[1];
  int status = mongo_client( conn, "127.0.0.1", 27017 );
  if( status != MONGO_OK ) {
      switch ( conn->err ) {
        case MONGO_CONN_NO_SOCKET:  printf( "no socket\n" ); return 1;
        case MONGO_CONN_FAIL:       printf( "connection failed\n" ); return 1;
        case MONGO_CONN_NOT_MASTER: printf( "not master\n" ); return 1;
      }
  }

  // get those queries!
  get_actors_from_movie(conn, "Beware the Gonzo");
  get_movies_from_actor(conn, "Newman, Griffin");

  // clean up, clean up, everybody everywhere
  // clean up, clean up, everybody do your share
  mongo_destroy( conn );
  return 0;
}

