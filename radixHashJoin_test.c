#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "radixJoin_util.h"

int main() {
	struct futhark_context_config *cfg = futhark_context_config_new();
	struct futhark_context *ctx = futhark_context_new(cfg);

	// R relation
	int R_keys[10] = {5, 6, 4, 9, 2, 8, 4, 5, 2, 1};
	int R_pL1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int R_pL2[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	// S relation
	int S_keys[10] = {12, 4, 13, 65, 43, 2, 9, 13, 12, 0};
	float S_pL1[10] = {0.1, 0.02, 0.3, 0.4, 5, 6.9, 0.007, 0.8, 0.9, 1.0};

	char R_payloads[10*2*sizeof(int)];
	char S_payloads[10*sizeof(float)];

	for(idx_t i=0; i<10; i++) {
		memcpy(R_payloads + i*2*sizeof(int), ((char*)R_pL1) + i*sizeof(int), sizeof(int));
		memcpy(R_payloads + i*2*sizeof(int) + 1*sizeof(int), ((char*)R_pL2) + i*sizeof(int), sizeof(int));
		memcpy(S_payloads + i*sizeof(float), ((char*)S_pL1) + i*sizeof(float), sizeof(float));
	}
	for(idx_t i=0; i<10; i++) {
		printf("%d %d %f\n",
			*((int*)(R_payloads + i*2*sizeof(int))),
			*((int*)(R_payloads + i*2*sizeof(int) + 1*sizeof(int))),
			*((float*)(S_payloads + i*sizeof(float)))
		);
	}
	printf("Prepared keys & payloads...\n");
	printf("\n");

	struct futhark_u8_2d *Rbuff;
	struct futhark_u8_2d *RpL;
	printf("Partitioning R...\n");
	radixPartition_GFTR(
	    ctx,
	    &Rbuff,
	    &RpL,
	    (char*)R_keys,
	    R_payloads,
	    10,
	    sizeof(int),
	    2*sizeof(int),
	    1,
	    2,
	    2,
	    256
    );
    printf("Partitioned R.\n");
    futhark_context_sync(ctx);

    int R_keys_back[10];
    char R_pL_back[10*2*sizeof(char)];
    int R_pL1_back[10];
    int R_pL2_back[10];
    futhark_values_u8_2d(ctx, Rbuff, (char*)R_keys_back);
    futhark_values_u8_2d(ctx, RpL, R_pL_back);
    for(idx_t i=0; i<10; i++) {
    	memcpy(R_pL1_back + i, R_pL_back + 2*i*sizeof(int), sizeof(int));
    	memcpy(R_pL2_back + i, R_pL_back + 2*i*sizeof(int) + 1*sizeof(int), sizeof(int));
    }
    for(idx_t i=0; i<10; i++) {
    	printf("#%ld - %d  %d  %d\n", i, R_keys_back[i], R_pL1_back[i], R_pL2_back[i]);
    }
    // TODO sth seems to be happening to the payloads?
    // TODO INVESTIGATE
    // with futhark funcs...
    printf("\n");
}