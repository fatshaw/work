#include <stdio.h>
#include "mongo.h"

int main ( void ) {

        int status;
        mongo conn[1];
        status = mongo_connect( conn, "127.0.0.1", 27017 );

        if ( status != MONGO_OK ) {
                //poof()
        } else {
		static const char fn[] = "sources.txt";
		FILE *file = fopen ( fn, "r" );
		if ( file != NULL ) {
			char line[512];
			while ( fgets ( line, sizeof line, file ) !=  NULL ) {
				bson b[1];
				bson_init( b );
				bson_append_new_oid( b, "_id" );
				bson_append_string( b, "url", line );
				bson_finish( b );
				mongo_insert( conn, "argang.sources", b );
				bson_destroy( b );
			}
			fclose ( file );

		}
	}
	return 0;
}

