/* TODO add the duckdb lib files to the project folder */

#include <stdio.h>
#include "duckdb.h"
/* TODO create & add proper futhark library -- draft.fut? */

int main() {
	duckdb_database db;
	duckdb_connection con;
	duckdb_open(NULL, &db);
	duckdb_connect(db, &con);

	duckdb_result res;
  /* Create table t1 with 1 key column (at present only logically), 3 payload columns. */
	duckdb_query(con, "CREATE TABLE t1 (k INTEGER, x INTEGER, y INTEGER, z INTEGER);", NULL);
  /* Insert a few random columns -- TODO use duckdb's logic for efficient random inserts */
	duckdb_query(con, "INSERT INTO coords VALUES "
                            "(5, 2, 1, 3), "
                            "(4, 2, 3, 1), "
                            "(9, 4, 5, 2), "
                            "(23, 3, 15, 3)"
                            "(2, 4, 3, 3), "
                            "(14, 12, 7, 8), "
                            "(7, 3, 3, 7), "
                            "(8, 1, 4, 5), "
                            "(3, 4, 5, 6), "
                            "("1, 4, 5, 5")"
                            ";", NULL);
	duckdb_query(con, "SELECT * FROM coords;", &res);

// iterate until result is exhausted
	while (true) {
		duckdb_data_chunk result = duckdb_fetch_chunk(res);
		if (!result) {
			// result is exhausted
			break;
		}

// get the number of rows & columns from the data chunk
		idx_t row_count = duckdb_data_chunk_get_size(result);
		idx_t col_count = duckdb_data_chunk_get_column_count(result);

		duckdb_vector res_col[col_count];
		int32_t *vector_data[col_count];
		for (idx_t col = 0; col < col_count; col++) {
			res_col[col] = duckdb_data_chunk_get_vector(result, col);
			vector_data[col] = (int32_t *) duckdb_vector_get_data(res_col[col]);
		}
    /* TODO process the columns */
    /* and eventually maybe add validity checks? */
		//int32_t* xp = (int32_t *) duckdb_vector_get_data(res_col[0]);
		//int32_t* yp = (int32_t *) duckdb_vector_get_data(res_col[1]);

    /* TODO add futhark logic here */

    duckdb_destroy_data_chunk(&result);

  }
	// clean-up
	duckdb_destroy_result(&res);
	duckdb_disconnect(&con);
	duckdb_close(&db);

	return 0;
}
