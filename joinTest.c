#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "duckdb.h"

#include "ftSMJerr.h"
//#include "mylogger.h"
//#include "smjutil.h"

int main() {
	struct futhark_context_config *cfg = futhark_context_config_new();
  	struct futhark_context *ctx = futhark_context_new(cfg);

  	int Rk[10] = {2, 4, 6, 7, 8, 9, 10, 12, 15, 16};
  	int Sk[10] = {3, 4, 5, 9, 11, 12, 13, 14, 16, 17};

  	idx_t numPairs = 0;
  	int* joinedKeys;
  	struct futhark_i64_1d *idxR_ft;
    struct futhark_i64_1d *idxS_ft;

    printf("Performing join...\n");
    /*
  	InnerJoin_joinKeyColumns(
  		ctx,
  		&numPairs,
  		(void**)&joinedKeys,
  		&idxR_ft,
  		&idxS_ft,
  		DUCKDB_TYPE_INTEGER,
  		0,
  		0,
  		(void*)Rk,
  		(void*)Sk,
  		10,
  		10,
  		1,
  		2,
  		256,
  		256
  	);
  	*/
    printf("Wrapping keys into futhark arrays...\n");
	struct futhark_i32_1d *keys1_ft = futhark_new_i32_1d(ctx, Rk, 10);
	struct futhark_i32_1d *keys2_ft = futhark_new_i32_1d(ctx, Sk, 10);
	printf("Preparing output array...\n");
	struct futhark_opaque_joinPairs_int *joinPairs;
	printf("Calling the join function...\n");
	futhark_entry_inner_SMJ_int(ctx, &joinPairs, keys1_ft, keys2_ft, 0, 0,
		2, 1, 256, 256
	);
	printf("Syncing context...\n");
	futhark_context_sync(ctx);
	printf("Projecting result struct...\n");
	futhark_project_opaque_joinPairs_int_ix(ctx, &idxR_ft, joinPairs);
	futhark_project_opaque_joinPairs_int_iy(ctx, &idxS_ft, joinPairs);
	struct futhark_i32_1d *outVs_ft;
	futhark_project_opaque_joinPairs_int_vs(ctx, &outVs_ft, joinPairs);
	printf("Obtaining number of pairs...\n");
	numPairs = futhark_shape_i32_1d(ctx, outVs_ft)[0];
	printf("Unwrapping join values...\n");
	joinedKeys = malloc((numPairs)*(sizeof(int)));
	futhark_values_i32_1d(ctx, outVs_ft, joinedKeys);
	printf("Syncing context...\n");
	futhark_context_sync(ctx);
	printf("Cleanup...\n");
	futhark_free_i32_1d(ctx, keys1_ft);
	futhark_free_i32_1d(ctx, keys2_ft);
	futhark_free_i32_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_int(ctx, joinPairs);
  	printf("Completed join!\n");

  	printf("R keys:\n");
  	for(idx_t i=0;i<10;i++) {
  		printf("%d, ", Rk[i]);
  	}
  	printf("\n");

  	printf("S keys:\n");
  	for(idx_t i=0;i<10;i++) {
  		printf("%d, ", Sk[i]);
  	}
  	printf("\n");

  	printf("Join results:\n");
  	for(idx_t i=0;i<numPairs;i++) {
  		printf("%d, ", joinedKeys[i]);
  	}
  	printf("\n");

  	free(joinedKeys);
    futhark_free_i64_1d(ctx, idxR_ft);
    futhark_free_i64_1d(ctx, idxS_ft);

    futhark_context_free(ctx);
	futhark_context_config_free(cfg);

	return 0;
}

