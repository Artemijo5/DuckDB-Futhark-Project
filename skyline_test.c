#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "duckdb.h"

#include "ftSkyline.h"


int main() {
	struct futhark_context_config *cfg = futhark_context_config_new();
    struct futhark_context *ctx = futhark_context_new(cfg);
    printf("Set up futhark core.\n");

	float points[3*40] = 
		{
			6,1,1, 6.4,0.5,0.2, 5.6,1.4,0.3, 6.5,2.3,0.4,
			1,6,4, 0.6,6.6,0.4, 1.2,5.8,2.2, 2.2,6.2,3, 
			6,12,1, 5,13,1, 6.2,11,1, 5.1,12,1, 
			2,2,6.2, 1,3,6, 1,2,6.1, 2,1,6.1, 
			7,9.5,8, 9,5.5,6, 5.1,5.7,5.2, 8,5,6, 
			12,9,9.5, 15,6,8, 12,6,8, 10,8,6, 
			12,14,9.4, 15,11,7.9, 12,11,7.9, 10,13,6.1, 
			10,4,10, 11,3,11, 10,2,11, 11,1,10, 
			5,5,11, 6,5,10, 6,5.2,12, 7,5.3,12, 
			6,12,12, 7,11,13, 5,11,12, 6,12,10]

		}
	printf("Initialised points.\n");
}